#pragma once
#include "mgapiall.h"
#include "mgapires.h"
typedef struct PLUGINTOOLSTRUCT
{
	mgplugintool plugintool;
	mggui dialog;
	mgrec* db;
	mgcode mode;
	mgrec* parent;
	mgeditorcontext econtext;
}plugintool_struct;

//static plugintool_struct loop_cut_rec;