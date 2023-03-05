#pragma once

#include <mgapiall.h>

#include "ToolHead.h"


static mgresource* resource_;


static mgstatus loop_cut_start_func(mgplugintool plugin_tool,
	void* user_data,
	void* call_data);

static mggui create_dialog_func(mgplugintool pluginTool,
	void* toolData);

static int DialogProc(mggui dialog,
	mgdialogid dialog_id,
	mgguicallbackreason callback_reason,
	void* user_data,
	void* call_data);


static int vertex_func(mgeditorcontext editor_context,
	mgvertexinputdata* v_data,
	void* t_data);

static void point_func(mgeditorcontext editor_context,
	mgpointinputdata* point_input_data,
	void* tool_data);

static void pick_func(mgeditorcontext editor_context,
	mgpickinputdata* pick_input_data,
	void* tool_data);

mgbool init(mgplugin* plugin, mgresource* resource, int* argc, char* argv[]);

void exit_loop_cut(mgplugin plugin);

void generate(plugintool_struct* pt_s);

static mgstatus generate_callback(mggui gui,
	mgcontrolid controlId,
	mgguicallbackreason callbackReason,
	void* userData,
	void* callData);

static void initialize_control_callbacks(plugintool_struct* pt_s);

static mgstatus initialize_dialog(plugintool_struct* pt_s);

static void terminate_func(mgeditorcontext editorContext,
	mgtoolterminationreason reason,
	void* toolData);
