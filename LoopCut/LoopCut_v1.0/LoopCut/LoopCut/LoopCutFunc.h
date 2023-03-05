#pragma once

#include "mgapiall.h"
#include "mgType.h"
#include "ToolHead.h"

mgstatus cast_test_func(mgrec* db);

/**
 * \brief select a loop above the selected edge
 * \param pt_s editor struct
 * \param v1 first vertex of the start edge
 * \return MSTAT_OK/ mgFALSE
 */
mgstatus select_loop(PLUGINTOOLSTRUCT* pt_s, mgrec* v1);

/**
 * \brief get the loop vertex around the input vtx
 * \param v1 the center vtx
 * \param dire loop direction
 * \param pos return node's position
 * \return the next/previous vtx
 */
mgrec* get_loop_vtx(mgrec* v1, loop_dire dire, short* pos);

/**
 * \brief get a face loop about the selected edge. NOTE: loop cut only works when the polygon has a shared edge count of TWO
 * \param pt_s editor struct
 * \param f_l face loop
 * \return mgTRUE/mgFalse
 */
mgbool get_face_loop(PLUGINTOOLSTRUCT* pt_s, face_loop* f_l);


mgbool check_edge_in_polygon(face_loop* f_l, mgrec* p);

static mgbool mgw_per_check_edge_in_polygon(mgrec* db, mgrec* parent, mgrec* rec, void* user_data);

static mgbool mgw_post_check_edge_in_polygon(mgrec* db, mgrec* parent, mgrec* rec, void* user_data);


mgcoord3d get_vertex_coord(mgrec* vtx);