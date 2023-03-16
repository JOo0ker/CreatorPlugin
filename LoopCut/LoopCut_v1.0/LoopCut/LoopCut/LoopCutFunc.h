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
 * \param v the center vtx
 * \param dire loop direction
 * \param pos return node's position
 * \return the next/previous vtx
 */
mgrec* get_loop_vtx(mgrec* v, const loop_dire dire, short* pos);

/**
 * \brief get a face loop about the selected edge. NOTE: loop cut only works when the polygon has a shared edge count of TWO
 * \param pt_s editor struct
 * \param f_l face loop
 * \return mgTRUE/mgFalse
 */
mgbool get_face_loop(PLUGINTOOLSTRUCT* pt_s, face_loop* f_l);


mgbool check_edge_in_polygon(face_loop* f_l, mgrec* p, short* pos);

static mgbool mgw_per_check_edge_in_polygon(mgrec* db, mgrec* parent, mgrec* rec, void* user_data);

static mgbool mgw_post_check_edge_in_polygon(mgrec* db, mgrec* parent, mgrec* rec, void* user_data);


mgcoord3d get_vertex_coord(mgrec* vtx);

faceloopcase insert_face_loop_node(face_loop* f_l, mgrec* p, const short* pos);
mgstatus free_face_loop(face_loop* f_l);

mgbool show_face_fool(const face_loop* f_l);

mgbool another_edge_pos(const short* pos, short* anti_pos);

mgbool match_face_loop_end(const face_loop* f_l);

mgbool draw_loop_cut_cst(PLUGINTOOLSTRUCT* pt_s, const face_loop* f_l);

mgbool draw_edge_node_cst(PLUGINTOOLSTRUCT* pt_s, mgcoord3d v10_coord, mgcoord3d v20_coord);

mgbool cut_face_loop(PLUGINTOOLSTRUCT* pt_s);

mgbool is_collinear();