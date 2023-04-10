#pragma once
#include <Windows.h>
#include <vector>

#include "mgapiall.h"
#include "resource.h"

#pragma region define
#define PREVIOUS ePrevious
#define NEXT eNext
#define MOVE_SENSITIVITY 300
#define SPLIT_NUM_SENSITIVITY 20

#pragma endregion


typedef struct MGEDGE
{
	mgrec* v1;
	mgrec* v2;
	int edge_shared_count;
}mg_edge;

typedef struct CUTPOINTSDATA
{
	mglined cut_points_on_edge[2];
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

#pragma region define_func

#define MGMSG(...) mgSendMessage(MMSG_STATUS, __VA_ARGS__);




#pragma endregion


#pragma region inline_func

#pragma endregion