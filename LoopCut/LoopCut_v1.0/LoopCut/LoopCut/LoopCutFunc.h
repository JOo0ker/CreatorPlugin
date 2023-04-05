#pragma once

#include "mgapiall.h"
#include "ToolHead.h"

mgstatus loop_cut_main(PLUGINTOOLSTRUCT* pt_s, mgrec* v1);

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
 * \return mgTRUE/mgFalse
 */
mgbool get_face_loop(const PLUGINTOOLSTRUCT* pt_s);

/**
 * \brief check the edge is in the polygon
 * \param p target polygon
 * \param f_l_t face loop node, which contain the checked edge
 * \param pos store the edge's position
 * \return MG_TRUE/MG_FALSE
 */
mgbool check_edge_in_polygon(mgrec* p, const face_loop* f_l_t, short* pos);

/**
 * \brief walk through the polygon to check the edge is in the polygon; pre-walk
 * \param db origin node
 * \param parent rec's parent
 * \param rec current node
 * \param user_data PLUGINTOOLSTRUCT
 * \return MG_TRUE
 */
static mgbool mgw_pre_check_edge_in_polygon(mgrec* db, mgrec* parent, mgrec* rec, void* user_data);

/**
 * \brief walk through the polygon to check the edge is in the polygon; post-walk
 * \param db origin node
 * \param parent rec's parent
 * \param rec current node
 * \param user_data PLUGINTOOLSTRUCT
 * \return MG_TRUE
 */
static mgbool mgw_post_check_edge_in_polygon(mgrec* db, mgrec* parent, mgrec* rec, void* user_data);

/**
 * \brief get the vtx's coordination( mgCoord3d )
 * \param vtx src vtx( mgVertex )
 * \return src vtx's coordination/ mgCoord3dZero()
 */
mgcoord3d get_vertex_coord(mgrec* vtx);

/**
 * \brief append target polygon face loop node to the source face loop
 * \param p target polygon
 * \param pos coincide vertex's position
 * \return face loop case type (normal, match_face_first_time, match_face_second_time, match_previous_face)
 */
faceloopcase append_face_loop_node(mgrec* p, const short* pos);

mgstatus init_face_loop(mgrec* v1);

mgstatus sort_pos(short* pos);

/**
 * \brief free face loop node
 * \return MSTAT_OK
 */
mgstatus free_face_loop();

mgstatus free_cut_points();

/**
 * \brief print face loop node in creator info window
 * \return MG_TRUE/MG_FALSE
 */
mgbool show_face_loop();

/**
 * \brief get the opposite edge's position for polygon with 4 vertex
 * \param pos current edge's position
 * \param anti_pos opposite edge's position
 * \return MG_TRUE
 */
mgbool another_edge_pos(const short* pos, short* anti_pos);


mgbool get_cut_point(const PLUGINTOOLSTRUCT* pt_s);

/**
 * \brief draw the face loop with construction
 * \param pt_s PLUGINTOOLSTRUCT
 * \return MG_TRUE/MG_FALSE
 */
mgbool draw_loop_cut_cst(const PLUGINTOOLSTRUCT* pt_s);

/**
 * \brief draw a edge construction with two vertex
 * \param pt_s PLUGINTOOLSTRUCT
 * \param v10_coord edge construction line's start point
 * \param v20_coord edge construction line's end point
 * \return MG_TRUE/MG_FALSE
 */
mgbool draw_edge_node_cst(const PLUGINTOOLSTRUCT* pt_s, mgcoord3d v10_coord, mgcoord3d v20_coord);

/**
 * \brief cut the face loop and delete all construction
 * \param pt_s PLUGINTOOLSTRUCT
 * \return MG_TRUE/MG_FALSE
 */
mgbool cut_face_loop(const PLUGINTOOLSTRUCT* pt_s);

mgbool split_face(const PLUGINTOOLSTRUCT* pt_s, const face_loop* f_l_n);