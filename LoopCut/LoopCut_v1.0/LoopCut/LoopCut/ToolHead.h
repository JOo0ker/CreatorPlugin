#pragma once
#include <Windows.h>
#include <vector>

#include "mgapiall.h"
#include "resource.h"

typedef struct MGEDGE
{
	mgrec* v1;
	mgrec* v2;
	int edge_shared_count;
}mg_edge;

typedef struct CUTPOINTSDATA
{
	mglined* cut_points_on_edge;
	std::vector<mglined*>* cut_points;
}cut_points_data;

typedef struct FACELOOP
{
	mgrec* face;
	short pos[2];		// coincide vertex's position 
	mg_edge* coincide_edge;
	double offset[3];// 0: total offset; 1: last offset; 2: current offset
	cut_points_data cut_points_data;
}face_loop;

typedef enum FACLELOOPCASE
{
	unknown,
	normal,
	match_face_first_time,
	match_face_second_time,
}faceloopcase;

typedef struct PLUGINTOOLSTRUCT
{
	int split;
	mgplugintool plugin_tool;
	mggui dialog;
	mggui e_control;
	mggui s_control;
	mgrec* db;
	mgrec* group;
	mgrec* parent;
	mgcode mode;
	mgeditorcontext econtext;
}plugintool_struct;

#define PREVIOUS ePrevious
#define NEXT eNext

typedef enum
{
	ePrevious,
	eNext,
}loop_dire;

typedef enum
{
	total_offset,
	last_offset,
	current_offset
}offset_type;

/* Globals */
static mgmouseinputtype current_mouse_input = MMSI_NOINPUT;
static mgresource* resource_;
static HHOOK mouse_hook;
static HHOOK keyboardHook;
static HWND creator_hwnd;
static DWORD creator_thread_id;


#pragma region define_func

#define MGMSG(...) mgSendMessage(MMSG_STATUS, __VA_ARGS__);




#pragma region


#pragma region inline_func

#pragma endregion