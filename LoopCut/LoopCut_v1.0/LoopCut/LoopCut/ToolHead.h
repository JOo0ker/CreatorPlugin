#pragma once
#include "mgapiall.h"
#include "mgapires.h"

typedef struct MGEDGE
{
	mgrec* v1;
	mgrec* v2;
	int edge_shared_count;
}mg_edge;

typedef struct face_loop
{
	struct face_loop *next, *prev, *first;

	mgrec* face;
	short pos[2];		// the 2nd coincide vertex's position 
	mg_edge* coincide_edge_first, *coincide_edge_second;
}face_loop;

typedef enum MOUSEINPPUTMODE
{
	vertex = 1,
	point = 2,
	pick = 3
}mouseinputmode;

static mouseinputmode mouseinputm = mouseinputmode(pick);

typedef struct PLUGINTOOLSTRUCT
{
	mgplugintool plugintool;
	mggui dialog;
	mgrec* db;
	mgcode mode;
	mgrec* parent;
	mgeditorcontext econtext;
}plugintool_struct;



#define PREVIOUS ePrevious
#define NEXT eNext

typedef enum
{
	ePrevious,
	eNext,
}loop_dire;