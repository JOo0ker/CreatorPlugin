#pragma once
#include "ToolHead.h"

/**
 * \brief register editor and set editor's functions
 * \param plugin selection plugin handle
 * \param resource resource
 * \param argc ...
 * \param argv ...
 * \return MG_TRUE/MG_FALSE
 */
mgbool init(const mgplugin* plugin, mgresource* resource, int* argc, char* argv[]);

/**
 * \brief start selection tool
 * \param plugin_tool plugin tool handle
 * \param user_data pt_s
 * \param call_data call data...?
 * \return MG_TRUE/MG_FALSE
 */
static mgstatus start_selection(mgplugintool plugin_tool, void* user_data, void* call_data);

static void load_tool_preferences(const plugintool_struct* pt_s);

static mgstatus list_box_callback(mggui gui, mgcontrolid control_id,
	mgguicallbackreason callback_reason,
	void* user_data, void* call_data);

static mgstatus button_callback(mggui gui, mgcontrolid control_id,
	mgguicallbackreason callback_reason,
	void* user_data, void* call_data);

mgbool insert_item(std::string& name, mgreclist list);

std::string transfer_to_string(const char* item_name);

mgbool set_default_item_name(const plugintool_struct* pt_s, mgbool b_reset_count = MG_FALSE);

mgbool free_pt_s();

void exit_selection();