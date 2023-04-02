#pragma once
#include <vector>

#include "mgapiall.h"

typedef struct MGEDGE
{
	mgrec* v1;
	mgrec* v2;
	int edge_shared_count;
}mg_edge;

typedef struct FACELOOP
{
	FACELOOP *next, *prev, *first;
	mgbool is_last_node;
	mgrec* face;
	short pos[2];		// coincide vertex's position 
	mg_edge* coincide_edge;
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
	match_previous_face

}faceloopcase;

typedef struct PLUGINTOOLSTRUCT
{
	mgplugintool plugin_tool;
	mggui dialog;
	mgrec* db;
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
static mouseinputmode mouseinputm = mouseinputmode(pick);