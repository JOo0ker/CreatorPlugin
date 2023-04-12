#pragma once

#include <algorithm>
#include <cctype>
#include <map>
#include <vector>
#include <string>
#include <mgapiall.h>
#include "resource.h"

#pragma region define
#define LISTREF "select_list"
#define DEFAULTNAME "Item-"

#pragma endregion


#pragma region sturct

typedef struct SELECTEDNODE
{
	mgbool is_edge;
	mgrec* rec;
}selected_node;

typedef struct PLUGINTOOLSTRUCT
{
	mgplugintool plugin_tool;
	mggui list_box_control;
	mggui edit_current_name_control;
	mgrec* db;
	mgeditorcontext econtext;
}plugintool_struct;

#pragma endregion


#pragma region global
/* Globals */
static plugintool_struct* g_pt_s;
static mgresource* g_resource_ptr = nullptr;
static mggui g_dialog = MG_NULL;
static std::map<std::string, std::vector<selected_node*>> g_select_list;
static unsigned int g_default_name_count = 0;

#pragma endregion

#pragma region define_func

#define MGMSG(...) mgSendMessage(MMSG_STATUS, __VA_ARGS__);




#pragma endregion