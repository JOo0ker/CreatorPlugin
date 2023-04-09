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

/**
 * \brief initializes the first face loop
 * \param v1 initializes the first face loop
 * \return MSTAT_OK
 */
mgstatus init_face_loop(mgrec* v1);

/**
 * \brief sorts the positions of the selected edge to ensure that pos[1] is always the next vertex position.
 * \param pos An array containing the vertex positions of the selected edge.
 * \return MSTAT_OK
 */
mgstatus sort_pos(short* pos);

/**
 * \brief free face loop node
 * \return MSTAT_OK
 */
mgstatus free_face_loop();

/**
 * \brief free face cut points data
 * \return MSTAT_OK
 */
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

/**
 * \brief calculates the cut points for each face in the loop cut.
 * \param pt_s PLUGINTOOLSTRUCT
 * \return MG_TRUE/MG_FALSE
 */
mgbool get_cut_point(const PLUGINTOOLSTRUCT* pt_s);

/**
 * \brief draw the face loop with construction
 * \param pt_s PLUGINTOOLSTRUCT
 * \param color construction color
 * \return MG_TRUE/MG_FALSE
 */
mgbool draw_loop_cut_cst(const PLUGINTOOLSTRUCT* pt_s,const mgconstructcolor color);

/**
 * \brief draw a edge construction with two vertex
 * \param pt_s PLUGINTOOLSTRUCT
 * \param v10_coord edge construction line's start point
 * \param v20_coord edge construction line's end point
 * \param color construction color
 * \return MG_TRUE/MG_FALSE
 */
mgbool draw_edge_node_cst(const PLUGINTOOLSTRUCT* pt_s, mgcoord3d v10_coord, mgcoord3d v20_coord, mgconstructcolor color);

/**
 * \brief cut the face loop and delete all construction
 * \param pt_s PLUGINTOOLSTRUCT
 * \return MG_TRUE/MG_FALSE
 */
mgbool cut_face_loop(const PLUGINTOOLSTRUCT* pt_s);

/**
 * \brief splits a face into multiple new polygons using the cut points in the provided face_loop structure
 * \param pt_s PLUGINTOOLSTRUCT
 * \param f_l_n face loop struct node
 * \return MG_TRUE/MG_FALSE
 */
mgbool split_face(const PLUGINTOOLSTRUCT* pt_s, const face_loop* f_l_n);


/**
 * \brief  increases the number of splits used in a loop cut
 * \param pt_s PLUGINTOOLSTRUCT
 * \param num The number of splits to add
 * \return MG_TRUE/MG_FALSE
 */
mgbool increase_number_of_split(PLUGINTOOLSTRUCT* pt_s, int num);

/**
 * \brief iterates through a list of objects and applies an offset to their cut points
 * \param pt_s PLUGINTOOLSTRUCT
 * \param delta delta offset by mouse input
 * \return MG_TRUE/MG_FALSE
 */
mgbool move_cut_point(const PLUGINTOOLSTRUCT* pt_s, const double delta);

/**
 * \brief clips a number between a minimum and a maximum value
 * \param value the number to clip
 * \param min the minimum value that the number can take
 * \param max The minimum value that the number can take
 * \return the clipped number
 */
double clip_number(const double value, const double min, const double max);