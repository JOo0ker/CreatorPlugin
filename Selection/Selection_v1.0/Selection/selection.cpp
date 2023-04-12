#include "selection.h"

mgbool init(const mgplugin* plugin, mgresource* resource, int* argc, char* argv[])
{
	g_resource_ptr = resource;

	g_pt_s = new plugintool_struct;

	g_pt_s->plugin_tool = mgRegisterViewer(
		*plugin, "Selection",
		start_selection, &g_pt_s,
		MTA_VERSION, "1.0",
		MTA_MENULOCATION, MMENU_SELECT,
		MTA_MENULABEL, "Selection...",
		MG_NULL
	);

	return g_pt_s->plugin_tool ? MG_TRUE : MG_FALSE;
}

mgstatus start_selection(mgplugintool plugin_tool, void* user_data, void* call_data)
{
	const auto pt_s = static_cast<plugintool_struct*>(user_data);
	const auto viewer_data = static_cast<mgviewercallbackrec*>(call_data);

	pt_s->db = mgGetActivationDb(viewer_data->toolActivation);

	if(!g_dialog)
	{
		g_dialog = mgResourceGetDialog(MG_NULL, *g_resource_ptr, SELECTIONDIALOG,
			0, 
			MG_NULL, MG_NULL);

		pt_s->edit_current_name_control = mgFindGuiById(g_dialog, Edit_Current_Name);

		if((pt_s->list_box_control = mgFindGuiById(g_dialog, LBox_Selection)))
		{
			mgSetGuiCallback(pt_s->list_box_control, MGCB_ACTIVATE,
				list_box_callback, pt_s);
		}

		mggui gui;
		if((gui = mgFindGuiById(g_dialog, Button_Add_One)))
		{
			mgSetGuiCallback(gui, MGCB_ACTIVATE, button_callback, pt_s);
		}
		if ((gui = mgFindGuiById(g_dialog, Button_Update)))
		{
			mgSetGuiCallback(gui, MGCB_ACTIVATE, button_callback, pt_s);
		}
		if ((gui = mgFindGuiById(g_dialog, Button_Delete_Select)))
		{
			mgSetGuiCallback(gui, MGCB_ACTIVATE, button_callback, pt_s);
		}
		if ((gui = mgFindGuiById(g_dialog, Button_Delete_All)))
		{
			mgSetGuiCallback(gui, MGCB_ACTIVATE, button_callback, pt_s);
		}

		load_tool_preferences(pt_s);
	}

	mgShowDialog(g_dialog);

	return (MSTAT_OK);
}

void load_tool_preferences(const plugintool_struct* pt_s)
{
	set_default_item_name(pt_s);
}

mgstatus list_box_callback(mggui gui, mgcontrolid control_id, mgguicallbackreason callback_reason, void* user_data, void* call_data)
{
	const auto pt_s = static_cast<plugintool_struct*>(user_data);

	char item_name[1024];

	if(MSTAT_ISOK(mgListGetSelectedItemString(pt_s->list_box_control, item_name, sizeof(item_name))))
	{
		const std::vector<selected_node*> temp_vector = g_select_list.at(transfer_to_string(item_name));

		mgDeselectAll(pt_s->db);
		for(const auto iter : temp_vector)
		{
			if((*iter).is_edge)
			{
				mgSelectOneEdge((*iter).rec);
			}
			else
			{
				mgSelectOne((*iter).rec);
			}
		}
	}

	return (MSTAT_OK);
}

mgstatus button_callback(mggui gui, mgcontrolid control_id, mgguicallbackreason callback_reason, void* user_data, void* call_data)
{
	const auto pt_s = static_cast<plugintool_struct*>(user_data);

	char item_name[1024];
	std::string name;

	switch (control_id)
	{
	case Button_Add_One:
		if (MSTAT_ISOK(mgTextGetString(pt_s->edit_current_name_control, item_name, sizeof(item_name))))
		{
			name = transfer_to_string(item_name);
			if (!name.empty()
				&& !mgListIsItemInList(pt_s->list_box_control, name.c_str()))
			{
				const auto new_list = mgGetSelectList(pt_s->db);
				if(mgGetRecListCount(new_list) == 0)
				{
					return (MSTAT_OK);
				}
				mgListAddItem(pt_s->list_box_control, name.c_str(), 0, MG_FALSE);
				insert_item(name, new_list);
			}
			set_default_item_name(pt_s);
		}
		break;

	case Button_Update:
		if (MSTAT_ISOK(mgListGetSelectedItemString(pt_s->list_box_control, item_name, sizeof(item_name))))
		{
			name = transfer_to_string(item_name);
			const std::vector<selected_node*> temp_vector = g_select_list.at(name);
			for (const auto iter : temp_vector)
			{
				delete iter;
			}
			g_select_list.erase(name);

			const auto new_list = mgGetSelectList(pt_s->db);
			insert_item(name, new_list);
		}
		break;

	case Button_Delete_Select:
		if (MSTAT_ISOK(mgListGetSelectedItemString(pt_s->list_box_control, item_name, sizeof(item_name))))
		{
			name = transfer_to_string(item_name);
			const std::vector<selected_node*> temp_vector = g_select_list.at(name);
			for (const auto iter : temp_vector)
			{
				delete iter;
			}
			g_select_list.erase(name);
			mgListDeleteSelectedItems(pt_s->list_box_control);
		}
		break;

	case Button_Delete_All:
		if(!g_select_list.empty())
		{
			free_pt_s();
			mgListDeleteAllItems(pt_s->list_box_control);
			set_default_item_name(pt_s, MG_TRUE);
			g_select_list.clear();
		}
		break;

	case MGID_OK:
		if (callback_reason == MGCB_ACTIVATE) {
			mgHideDialog(gui);
		}
		break;

	default: 
		break;
	}
	if ((callback_reason == MGCB_ACTIVATE) && (control_id != MGID_OK))
		mgRefreshDialog(g_dialog);

	return MSTAT_OK;
}

mgbool insert_item(std::string& name, const mgreclist list)
{
	mgrec* rec = mgGetNextRecInList(list, MG_NULL);

	const auto rec_vector = new std::vector<selected_node*>;

	while (rec)
	{
		auto s_n = new selected_node;

		s_n->is_edge = MG_FALSE;
		if(mgIsSelectedEdge(rec))
		{
			s_n->is_edge = MG_TRUE;
		}

		s_n->rec = rec;

		rec_vector->push_back(s_n);
		rec = mgGetNextRecInList(list, MG_NULL);
	}
	mgFreeRecList(list);

	g_select_list.insert(std::make_pair(name, *rec_vector));
	return MG_TRUE;
}

std::string transfer_to_string(const char* item_name)
{
	std::string str = item_name;

	str.erase(std::find_if(
		str.rbegin(),
		str.rend(),
		[](const int ch) {return !std::isspace(ch); }
	).base(), str.end());

	return str;
}

mgbool set_default_item_name(const plugintool_struct* pt_s, const mgbool b_reset_count)
{
	if(b_reset_count)
	{
		g_default_name_count = 0;
	}

	const std::string default_name = DEFAULTNAME + std::to_string(g_default_name_count++);

	if (MSTAT_ISOK(mgTextSetString(pt_s->edit_current_name_control, default_name.c_str())))
	{
		mgRefreshDialog(g_dialog);
		return MG_TRUE;
	}

	return MG_FALSE;
}

mgbool free_pt_s()
{
	for (auto& iter : g_select_list)
	{
		for (const auto s_n : iter.second)
		{
			delete s_n;
		}
		iter.second.clear();
	}
	return MG_TRUE;
}

void exit_selection()
{
	free_pt_s();
	delete g_pt_s;
}
