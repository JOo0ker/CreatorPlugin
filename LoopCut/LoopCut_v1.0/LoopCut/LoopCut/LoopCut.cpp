#include "LoopCut.h"

#include "LoopCutFunc.h"
#include "resource.h"

mgstatus loop_cut_start_func(mgplugintool plugin_tool, void* user_data, void* call_data)
{
	auto pt_s =static_cast<plugintool_struct*>(mgMalloc(sizeof(plugintool_struct)));

	auto cb_data = static_cast<mgeditorcallbackrec*>(call_data);

	pt_s->db = mgGetActivationDb(cb_data->toolActivation);

		//Initialize the user data record
	pt_s->plugintool = plugin_tool;
	pt_s->dialog = MG_NULL;
	pt_s->econtext = MG_NULL;
	pt_s->mode = mgGetModelingMode(pt_s->db);

	cb_data->dialogRequired = MG_TRUE;
	cb_data->toolData = pt_s;

	return MSTAT_OK;
}

mggui create_dialog_func(mgplugintool pluginTool, void* toolData)
{
	auto pt_s = static_cast<plugintool_struct*>(toolData);

	pt_s->dialog = mgResourceGetDialog(MG_NULL, *resource_, LOOPCUTDIALOG,
		MGCB_INIT | MGCB_SHOW | MGCB_DESTROY,
		DialogProc, pt_s);

	return (pt_s->dialog);

}

 mgstatus DialogProc(mggui dialog, 
						mgdialogid dialog_id, 
						mgguicallbackreason callback_reason, 
						void* user_data, 
						void* call_data)
{
	auto pt_s = static_cast<plugintool_struct*>(user_data);

	mgeditorcontext editor_context = mgEditorGetContext(dialog);

	switch (callback_reason)
	{
	case MGCB_INIT:
		initialize_control_callbacks(pt_s);
		initialize_dialog(pt_s);
		break;

	case MGCB_DESTROY:
		break;
	}

	return MSTAT_OK;
}

int vertex_func(mgeditorcontext editor_context, mgvertexinputdata* v_data, void* t_data)
{
#if 1
	auto pt_s = static_cast<PLUGINTOOLSTRUCT*>(t_data);
	int update_ref = 0;

	const mgmousestate mouse_event = v_data->mouseEvent;
	const unsigned int keyboard_flags = v_data->keyboardFlags;
	const unsigned int button_flags = v_data->buttonFlags;
	const mgcoord3d* this_point = v_data->thisPoint;
	const mgcoord3d* first_point = v_data->firstPoint;


	mgSendMessage(MMSG_STATUS, "The keyboard flag is : %1d", keyboard_flags);
	mgSendMessage(MMSG_STATUS, "The button flag is : %1d", button_flags);
	mgSendMessage(MMSG_STATUS, "The \"this point\" is : %1f,%2f,%3f",
		this_point->x, this_point->y, this_point->z);
	mgSendMessage(MMSG_STATUS, "The \"first point\" is : %1f,%2f,%3f",
		first_point->x, first_point->y, first_point->z);

	switch (mouse_event)
	{
	case MMSS_START:
		mgSendMessage(MMSG_STATUS, "This is a MMSS_START.");
		break;
	case MMSS_CONTINUE:
		mgSendMessage(MMSG_STATUS, "This is a MMSS_CONTINUE.");
		break;
	case MMSS_STOP:
		mgSendMessage(MMSG_STATUS, "This is a MMSS_STOP.");
		break;
	case MMSS_NONE:
		mgSendMessage(MMSG_STATUS, "This is a MMSS_NONE.");
		break;
	}
	return (update_ref);
#endif
}

void point_func(mgeditorcontext editor_context, mgpointinputdata* point_input_data, void* tool_data)
{
	auto pt_s = static_cast<PLUGINTOOLSTRUCT*>(tool_data);
	int update_ref = 0;

	const mgmousestate mouse_event = point_input_data->mouseEvent;
	const unsigned int keyboard_flags = point_input_data->keyboardFlags;
	const unsigned int button_flags = point_input_data->buttonFlags;
	const mgcoord2i* this_point = point_input_data->thisPoint;
	const mgcoord2i* first_point = point_input_data->firstPoint;

	switch (mouse_event)
	{
	case MMSS_START:
		mgSendMessage(MMSG_STATUS, "This is a MMSS_START.");
		break;
	case MMSS_CONTINUE:
		mgSendMessage(MMSG_STATUS, "This is a MMSS_CONTINUE.");
		break;
	case MMSS_STOP:
		mgSendMessage(MMSG_STATUS, "This is a MMSS_STOP.");
		break;
	case MMSS_NONE:
		mgSendMessage(MMSG_STATUS, "This is a MMSS_NONE.");
		break;
	}
}

void pick_func(mgeditorcontext editor_context, mgpickinputdata* pick_input_data, void* tool_data)
{
	auto pt_s = static_cast<PLUGINTOOLSTRUCT*>(tool_data);
	int update_ref = 0;

	unsigned int keyboard_flags = pick_input_data->keyboardFlags;
	unsigned int button_flags = pick_input_data->buttonFlags;
	mgselectlist select_list = pick_input_data->pickList;

	mgrec* rec = mgGetNextRecInList(select_list, MG_NULL);
	const mgcode code = mgGetCode(rec);
	switch (code)
	{
	case fltVertex:
		if (mgIsSelectedEdge(rec))
		{
			mgDeleteAllConstructs(pt_s->econtext);
			select_loop(pt_s, rec);
		}

		break;
	case fltPolygon:

		break;

	case fltObject:
		break;
	case fltGroup:
		break;

	default:
		mgSendMessage(MMSG_TIP, "Unknown pic.");
		break;
	}
	


}

mgbool init(mgplugin* plugin, mgresource* resource, int* argc, char* argv[])
{
	resource_ = resource;

	mgbool ini_ok = MG_FALSE;
	plugintool_struct pt_s;
	pt_s.plugintool = mgRegisterEditor(
		*plugin, "Loop cut",
		loop_cut_start_func, resource,
		MTA_VERSION, "1,0",
		MTA_PALETTELOCATION, MPAL_MODIFYFACE,
		MTA_PALETTEICON, mgResourceGetPixmap(*resource, LOOPCUTBITMAP),
		MG_NULL);
	if(!pt_s.plugintool)
	{
		mgSendMessage(MMSG_ERROR, "mgRegisterEditor failed.");
		return MG_FALSE;
	}

	if(!MSTAT_ISOK(mgEditorSetVertexFunc(pt_s.plugintool, vertex_func)))
	{
		mgSendMessage(MMSG_ERROR, "mgEditorSetVertexFunc failed.");
		return MG_FALSE;
	}

	if (!MSTAT_ISOK(mgEditorSetPointFunc(pt_s.plugintool, point_func)))
	{
		mgSendMessage(MMSG_ERROR, "mgEditorSetPointFunc failed.");
		return MG_FALSE;
	}

	if (!MSTAT_ISOK(mgEditorSetPickFunc(pt_s.plugintool, pick_func)))
	{
		mgSendMessage(MMSG_ERROR, "mgEditorSetPickFunc failed.");
		return MG_FALSE;
	}

	if(!MSTAT_ISOK(mgEditorSetTerminateFunc(pt_s.plugintool, terminate_func)))
	{
		mgSendMessage(MMSG_ERROR, "mgEditorSetTerminateFunc failed.");
		return MG_FALSE;
	}

	if(!MSTAT_ISOK(mgEditorSetCreateDialogFunc(pt_s.plugintool, create_dialog_func)))
	{
		mgSendMessage(MMSG_ERROR, "mgEditorSetCreateDialogFunc failed.");
		return MG_FALSE;
	}


	return MG_TRUE;
}

void exit_loop_cut(mgplugin plugin)
{
}

void generate(plugintool_struct* pt_s)
{

}

/**
 * \brief invoke the push ok button
 */
mgstatus generate_callback(mggui gui, 
                           mgcontrolid controlId, 
                           mgguicallbackreason callbackReason,
                           void* userData, 
                           void* callData)
{
	auto pt_s =static_cast<plugintool_struct*>(userData);
	generate(pt_s);
	return MG_TRUE;
}

void initialize_control_callbacks(plugintool_struct* pt_s)
{
	pt_s->econtext = mgEditorGetContext(pt_s->dialog);

	if(pt_s->dialog)
	{
		mggui gui_item;

		//if(gui_item = mgFindGuiById(pt_s->dialog, MGID_OK))
		//{
		//	mgSetGuiCallback(gui_item, MGCB_ACTIVATE, generate_callback, static_cast<void*>(pt_s));
		//}
	}
}

mgstatus initialize_dialog(plugintool_struct* pt_s)
{
	//switch (mouseinputm)
	//{
	//case mouseinputmode::vertex:
	//	mgEditorSelectMouseInput(pt_s->econtext, MMSI_VERTEXINPUT);
	//	break;
	//case mouseinputmode::point:
	//	mgEditorSelectMouseInput(pt_s->econtext, MMSI_POINTINPUT);
	//	break;
	//case mouseinputmode::pick:
	//	mgEditorSelectMouseInput(pt_s->econtext, MMSI_PICKINPUT);
	//	break;
	//default:
	//	break;
	//}
	mgEditorSelectMouseInput(pt_s->econtext, MMSI_PICKINPUT);

	if(pt_s->dialog)
	{
		mggui gui_item;

		if(gui_item = mgFindGuiById(pt_s->dialog, X_EDIT))
		{
			mgTextSetDouble(gui_item, 0.f, "%f");
		}
		if (gui_item = mgFindGuiById(pt_s->dialog, Y_EDIT))
		{
			mgTextSetDouble(gui_item, 0.f, "%f");
		}
		if (gui_item = mgFindGuiById(pt_s->dialog, Z_EDIT))
		{
			mgTextSetDouble(gui_item, 0.f, "%f");
		}

		if (gui_item = mgFindGuiById(pt_s->dialog, TEXT_EDIT))
		{
			mgTextSetString(gui_item, "test string");
		}

	}

	return MSTAT_OK;
}

void terminate_func(mgeditorcontext editorContext, mgtoolterminationreason reason, void* toolData)
{
	auto pt_s = (plugintool_struct*)toolData;

	switch (reason)
	{
	case MTRM_DONE:
		//generate(pt_s);
		break;
	case MTRM_CANCEL:
		mgDeleteAllConstructs(editorContext);
		break;
	}

	// free
	if(pt_s)
	{
		mgFree(pt_s);
	}
}
