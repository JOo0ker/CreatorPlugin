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

typedef struct FACELOOP
{
	mgrec* face;
	short pos[2];		// coincide vertex's position 
	mg_edge* coincide_edge;
	std::vector<mglined*>* cut_points;
}face_loop;

typedef enum MOUSEINPPUTMODE
{
	vertex = 1,
	point = 2,
	pick = 3
}mouseinputmode;

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

/* Globals */
static mgresource* resource_;
static HHOOK mouse_hook;
static HHOOK keyboardHook;