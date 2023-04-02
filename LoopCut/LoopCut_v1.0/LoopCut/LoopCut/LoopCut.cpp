﻿#include "LoopCut.h"

#include <cstring>

#include "LoopCutFunc.h"
#include "resource.h"

extern face_loop* f_l;

mgstatus loop_cut_start_func(mgplugintool plugin_tool, void* user_data, void* call_data)
{
	const auto pt_s =static_cast<plugintool_struct*>(mgMalloc(sizeof(plugintool_struct)));

	const auto cb_data = static_cast<mgeditorcallbackrec*>(call_data);

	//Initialize the user data record
	pt_s->db = mgGetActivationDb(cb_data->toolActivation);
	pt_s->plugin_tool = plugin_tool;
	pt_s->dialog = MG_NULL;
	pt_s->econtext = MG_NULL;
	pt_s->mode = mgGetModelingMode(pt_s->db);

	cb_data->dialogRequired = MG_TRUE;
	cb_data->toolData = pt_s;

	return MSTAT_OK;
}

mggui create_dialog_func(mgplugintool plugin_tool, void* tool_data)
{
	const auto pt_s = static_cast<plugintool_struct*>(tool_data);

	pt_s->dialog = mgResourceGetDialog(MG_NULL, *resource_, LOOPCUTDIALOG,
		MGCB_INIT | MGCB_SHOW | MGCB_DESTROY,
		dialog_proc, pt_s);

	return (pt_s->dialog);
}

 mgstatus dialog_proc(mggui dialog, 
						mgdialogid dialog_id, 
						mgguicallbackreason callback_reason, 
						void* user_data, 
						void* call_data)
{
	const auto pt_s = static_cast<plugintool_struct*>(user_data);

	mgeditorcontext editor_context = mgEditorGetContext(dialog);

	switch (callback_reason)
	{
	case MGCB_INIT:
		initialize_control_callbacks(pt_s);
		initialize_dialog(pt_s);
		break;

	case MGCB_DESTROY:
		break;

	default: 
		break;
	}

	return MSTAT_OK;
}

int vertex_func(mgeditorcontext editor_context, mgvertexinputdata* vertex_input_data, void* tool_data)
{
	auto pt_s = static_cast<PLUGINTOOLSTRUCT*>(tool_data);
	constexpr int update_ref = 0;

	const mgmousestate mouse_event = vertex_input_data->mouseEvent;
	const unsigned int keyboard_flags = vertex_input_data->keyboardFlags;
	const unsigned int button_flags = vertex_input_data->buttonFlags;
	const mgcoord3d* this_point = vertex_input_data->thisPoint;
	const mgcoord3d* first_point = vertex_input_data->firstPoint;


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
	const auto pt_s = static_cast<PLUGINTOOLSTRUCT*>(tool_data);
	int update_ref = 0;

	unsigned int keyboard_flags = pick_input_data->keyboardFlags;
	unsigned int button_flags = pick_input_data->buttonFlags;
	const mgselectlist select_list = pick_input_data->pickList;

	mgrec* rec = mgGetNextRecInList(select_list, MG_NULL);
	const mgcode code = mgGetCode(rec);
	switch (code)
	{
	case fltVertex:
		if (mgIsSelectedEdge(rec)
			&& (strcmp( mgGetName(rec), "Construction") != 0))
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
	pt_s.plugin_tool = mgRegisterEditor(
		*plugin, "Loop cut",
		loop_cut_start_func, resource,
		MTA_VERSION, "1,0",
		MTA_PALETTELOCATION, MPAL_MODIFYFACE,
		MTA_PALETTEICON, mgResourceGetPixmap(*resource, LOOPCUTBITMAP),
		MG_NULL);
	if(!pt_s.plugin_tool)
	{
		mgSendMessage(MMSG_ERROR, "mgRegisterEditor failed.");
		return MG_FALSE;
	}

	if(!MSTAT_ISOK(mgEditorSetVertexFunc(pt_s.plugin_tool, vertex_func)))
	{
		mgSendMessage(MMSG_ERROR, "mgEditorSetVertexFunc failed.");
		return MG_FALSE;
	}

	if (!MSTAT_ISOK(mgEditorSetPointFunc(pt_s.plugin_tool, point_func)))
	{
		mgSendMessage(MMSG_ERROR, "mgEditorSetPointFunc failed.");
		return MG_FALSE;
	}

	if (!MSTAT_ISOK(mgEditorSetPickFunc(pt_s.plugin_tool, pick_func)))
	{
		mgSendMessage(MMSG_ERROR, "mgEditorSetPickFunc failed.");
		return MG_FALSE;
	}

	if(!MSTAT_ISOK(mgEditorSetTerminateFunc(pt_s.plugin_tool, terminate_func)))
	{
		mgSendMessage(MMSG_ERROR, "mgEditorSetTerminateFunc failed.");
		return MG_FALSE;
	}

	if(!MSTAT_ISOK(mgEditorSetCreateDialogFunc(pt_s.plugin_tool, create_dialog_func)))
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
	if (f_l) loop_cut_execute(pt_s);
}

static mgstatus generate_callback(mggui gui, 
                           mgcontrolid control_id, 
                           mgguicallbackreason callback_reason,
                           void* user_data, 
                           void* call_data)
{
	const auto pt_s =static_cast<plugintool_struct*>(user_data);
	mgSendMessage(MMSG_STATUS, "generate_callback1");
	generate(pt_s);
	return MSTAT_OK;
}

void initialize_control_callbacks(plugintool_struct* pt_s)
{
	pt_s->econtext = mgEditorGetContext(pt_s->dialog);

	if(pt_s->dialog)
	{
		mggui gui_item;

		//if((gui_item = mgFindGuiById(pt_s->dialog, MGID_OK)))
		//{
		//	mgSetGuiCallback(gui_item, MGCB_ACTIVATE, generate_callback, static_cast<void*>(pt_s));
		//}
	}
}

mgstatus initialize_dialog(plugintool_struct* pt_s)
{
	mgEditorSelectMouseInput(pt_s->econtext, MMSI_PICKINPUT);

	if (!pt_s->dialog)
	{
		return MG_FALSE;
	}
	mggui gui_item;

	if ((gui_item = mgFindGuiById(pt_s->dialog, X_EDIT)))
	{
		mgTextSetDouble(gui_item, 0.f, "%f");
	}
	if ((gui_item = mgFindGuiById(pt_s->dialog, Y_EDIT)))
	{
		mgTextSetDouble(gui_item, 0.f, "%f");
	}
	if ((gui_item = mgFindGuiById(pt_s->dialog, Z_EDIT)))
	{
		mgTextSetDouble(gui_item, 0.f, "%f");
	}

	if ((gui_item = mgFindGuiById(pt_s->dialog, TEXT_EDIT)))
	{
		mgTextSetString(gui_item, "test string");
	}
	return MSTAT_OK;
}

void terminate_func(mgeditorcontext editor_context, mgtoolterminationreason reason, void* tool_data)
{
	const auto pt_s = static_cast<plugintool_struct*>(tool_data);

	switch (reason)
	{
	case MTRM_DONE:
		generate(pt_s);
		break;
	case MTRM_CANCEL:
		mgDeleteAllConstructs(editor_context);
		break;
	default: 
		break;
	}

	// free
	if(pt_s)
	{
		mgFree(pt_s);
	}
}

void loop_cut_execute(plugintool_struct* pt_s)
{
	cut_face_loop(pt_s);
	free_face_loop();
}
