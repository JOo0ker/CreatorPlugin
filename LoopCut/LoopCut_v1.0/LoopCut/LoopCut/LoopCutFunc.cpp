#include "LoopCutFunc.h"

//#define SHOW_LOOP

static short match_first_face_times = 0;
std::vector<face_loop*> f_l;
std::vector<face_loop*>::iterator iter_f_l;

mgstatus loop_cut_main(PLUGINTOOLSTRUCT* pt_s, mgrec* v1)
{
	// distinguish between a single face and a loop face
	match_first_face_times = 0;

	// get parent
	pt_s->parent = mgGetParent(mgGetParent(v1));

	// get the nearest group 
	mgrec* group = pt_s->parent;
	while (
		group // mgGetParent(Db) => MG_NULL
		&& mgGetCode(group) != fltGroup)
	{
		group = mgGetParent(group);
	}

	switch (mgGetCode(group))
	{
	case fltGroup:
		mgSendMessage(MMSG_TIP, " The group name is : %1s .", mgGetName(group));
		break;

	default:
		mgSendMessage(MMSG_TIP, "Please make sure that you have a GROUP node.");
		return MG_FALSE;
	}
	pt_s->group = group;

	free_face_loop();
	init_face_loop(v1);
	if (!get_face_loop(pt_s)) return MG_FALSE;

#ifdef SHOW_LOOP
	show_face_loop();
#endif

	get_cut_point(pt_s);
	draw_loop_cut_cst(pt_s, mgconstructcolor::MCCOLOR_RED);

	return MG_TRUE;
}

mgrec* get_loop_vtx(mgrec* v, const loop_dire dire, short* pos)
{
	mgrec* parent = mgGetParent(v);
	const auto total = static_cast<short>(mgCountChild(parent));

	// have more than one vertex
	if (total > 1)
	{
		// matching vertex position
		short m_v_p = 0;
		for (short i = 1; i <= total; i++)
		{
			mgcoord3d v_coord, v_cur;

			v_coord = get_vertex_coord(v);
			v_cur = get_vertex_coord(mgGetChildNth(parent, i));

			if (mgCoord3dEqual(&v_coord, &v_cur))
			{
				switch (dire)
				{
				case PREVIOUS:
					m_v_p = (i - 1) > 0 ? static_cast<short>(i - 1) : total;
					break;
				case NEXT:
					m_v_p = (i + 1) <= total ? static_cast<short>(i + 1) : static_cast <short>(1);
					break;
				}

				if (pos) *pos = m_v_p;
				return mgGetChildNth(parent, m_v_p);
			}
		}
	}
	if (pos) *pos = 1;
	return v;
}

mgbool get_face_loop(const PLUGINTOOLSTRUCT* pt_s)
{
	iter_f_l = f_l.begin();

	// walk the group and get the face loop
	while (true)
	{
		mgWalk(pt_s->group,
			mgw_pre_check_edge_in_polygon,
			mgw_post_check_edge_in_polygon,
			MG_NULL,
			MWALK_ON | MWALK_VERTEX);

		if (match_first_face_times == 2)
		{// got a whole loop
			break;
		}

		switch ((*iter_f_l)->coincide_edge->edge_shared_count)
		{
		case 0:// invalid selected face
			return MG_FALSE;

		case 1:// isolate edge of face
			return MG_TRUE;

		case 2:// standard edge of face
			++iter_f_l;
			break;

		default:// other invalid face
			return MG_FALSE;
		}
	}

	return MG_TRUE;
}

mgbool check_edge_in_polygon(mgrec* p, const face_loop* f_l_t, short* pos)
{
	// the source edge
	const mg_edge* e = f_l_t->coincide_edge;
	mgcoord3d	e_coord[2]{ MG_NULL,MG_NULL,MG_NULL },
				p_v_coord[4]{ MG_NULL,MG_NULL,MG_NULL };

	// the edge that needs to be checked
	e_coord[0] = get_vertex_coord(e->v1);
	e_coord[1] = get_vertex_coord(e->v2);

	// the target polygon
	p_v_coord[0] = get_vertex_coord(mgGetChildNth(p, 1));
	p_v_coord[1] = get_vertex_coord(mgGetChildNth(p, 2));
	p_v_coord[2] = get_vertex_coord(mgGetChildNth(p, 3));
	p_v_coord[3] = get_vertex_coord(mgGetChildNth(p, 4));
	// restore the conform vertex number
	short i = 1;

	// whether the coordinate of ith vertex is the same as the 1st vertex of source edge
	mgbool b_conform = MG_FALSE;

	// iterate through all vertices that are located beneath the target polygon,
	// and compare them with the 1st vertex of the source edge.
	for(; i < 5; i++)
	{
		if (mgCoord3dEqual(&e_coord[0], &p_v_coord[i - 1]))
		{
			b_conform = MG_TRUE;
			break;
		}
	}
	
	// if find a vertex in the target polygon that has the same coordinates as the 1st vertex of the source edge
	if (b_conform)
	{
		// restore the position of coincide edge vertices  (i: from 0 to 3)
		pos[0] = i;

		// get the previous vertex of founded vertex
		mgcoord3d p_vc_near = get_vertex_coord(
			get_loop_vtx(mgGetChildNth(p, pos[0]), PREVIOUS, &pos[1]));

		// compare pre-vertex with 2nd vertex of source edge
		if (mgCoord3dEqual(&e_coord[1], &p_vc_near))
		{
			return MG_TRUE;
		}

		// get the next vertex of founded vertex
		p_vc_near = get_vertex_coord(
			get_loop_vtx(mgGetChildNth(p, pos[0]), NEXT, &pos[1]));

		// compare next-vertex with 2nd vertex of source edge
		if (mgCoord3dEqual(&e_coord[1], &p_vc_near))
		{
			return MG_TRUE;
		}
	}
	return MG_FALSE;
}

mgbool mgw_pre_check_edge_in_polygon(mgrec* db, mgrec* parent, mgrec* rec, void* user_data)
{
	if (mgGetCode(rec) != fltPolygon
		|| mgCountChild(rec) != 4// match an invalid polygon that does not have exactly 4 vertices as its components.
		|| (rec == (*iter_f_l)->face)
		)
	{
		return MG_TRUE;
	}

	short pos[2]{ 0,0 };

	if (check_edge_in_polygon(rec, *iter_f_l, pos))
	{// found a matching edge
		const faceloopcase fl_case = append_face_loop_node(rec, pos);
		switch (fl_case)
		{
		case faceloopcase::unknown:
			break;
		case faceloopcase::normal:
			(*iter_f_l)->coincide_edge->edge_shared_count += 1;
			break;
		case faceloopcase::match_face_first_time:
			break;
		case faceloopcase::match_face_second_time:
			(*iter_f_l)->coincide_edge->edge_shared_count += 1;
			break;
		}

		return MG_TRUE;
	}
	return MG_TRUE;
}

mgbool mgw_post_check_edge_in_polygon(mgrec* db, mgrec* parent, mgrec* rec, void* user_data)
{
	return MG_TRUE;
}

mgcoord3d get_vertex_coord(mgrec* vtx)
{
	if(mgGetCode(vtx) == fltVertex)
	{
		mgcoord3d temp{ MG_NULL,MG_NULL,MG_NULL };
		mgGetVtxCoord(vtx, &temp.x, &temp.y, &temp.z);
		return mgcoord3d{ temp.x, temp.y, temp.z };
	}
	
	return mgCoord3dZero();
}

faceloopcase append_face_loop_node(mgrec* p, const short* pos)
{
	if(p == f_l.front()->face) // the same as first face
	{
		match_first_face_times += 1;

		switch (match_first_face_times)
		{
		case 1:
			return faceloopcase::match_face_first_time;

		case 2:
			return faceloopcase::match_face_second_time;

		default:
			return faceloopcase::unknown;
		}
	}

	// create a new face loop node to restore the face
	const auto new_face_loop_node = new face_loop;

	// create a new edge node to restore edge
	const auto new_edge = new mg_edge;

	// the opposite edge
	short anti_pos[2];
	another_edge_pos(pos, anti_pos);

	new_edge->v1 = mgGetChildNth(p, anti_pos[0]);
	new_edge->v2 = mgGetChildNth(p, anti_pos[1]);
	new_edge->edge_shared_count = 1;

	new_face_loop_node->coincide_edge = new_edge;
	new_face_loop_node->pos[0] = anti_pos[0];
	new_face_loop_node->pos[1] = anti_pos[1];
	new_face_loop_node->face = p;

	sort_pos(new_face_loop_node->pos);

	const int distance = std::distance(f_l.begin(), iter_f_l);

	f_l.push_back(new_face_loop_node);

	iter_f_l = f_l.begin() + distance;

	return faceloopcase::normal;
}

mgstatus init_face_loop(mgrec* v1)
{
	// Create a new face loop.
	const auto f_l_first = new face_loop;

	// Set the face to the parent of the first vertex.
	f_l_first->face = mgGetParent(v1);

	// Find the selected vertex position
	for (f_l_first->pos[0] = 1; ; f_l_first->pos[0]++)
	{
		if(v1 == mgGetChildNth(f_l_first->face, f_l_first->pos[0]))
		{
			break;
		}
	}

	// Get the second vertex and position.		NOTE: the vertex represented by the edge is always the previous vertex.
	mgrec* v2 = get_loop_vtx(v1, NEXT, &f_l_first->pos[1]);

	// Create a new edge.
	const auto edge = new mg_edge;
	edge->v1 = v1;
	edge->v2 = v2;
	edge->edge_shared_count = 1;

	// Set the coincidence edge of the face loop.
	f_l_first->coincide_edge = edge;

	// Add the face loop to the vector of face loops.
	f_l.push_back(f_l_first);

	return MSTAT_OK;
}

mgstatus sort_pos(short* pos)
{
	// Calculate the next position.
	const short next_pos = (pos[0] + 1) > 4 ? static_cast<short>(1) : static_cast<short>(pos[0] + 1) ;

	// If pos[1] is not the next position, swap the values.
	if(pos[1] != next_pos)
	{// exchange the position
		const short temp = pos[0];
		pos[0] = pos[1];
		pos[1] = temp;
	}

	return MSTAT_OK;
}

mgstatus free_face_loop()
{
	// empty input
	if (f_l.empty()) return MG_TRUE;

	auto iter_free = f_l.begin();

	free_cut_points();

	while(iter_free != f_l.end())
	{
		delete (*iter_free)->coincide_edge;
		delete (*iter_free);
		++iter_free;
	}
	f_l.clear();
	return MSTAT_OK;
}

mgstatus free_cut_points()
{
	// empty input
	if (f_l.empty()) return MG_TRUE;

	auto iter_free = f_l.begin();
	while (iter_free != f_l.end())
	{
		delete (*iter_free)->cut_points_data.cut_points_on_edge;
		auto iter_free_line = (*iter_free)->cut_points_data.cut_points->begin();
		while (iter_free_line != (*iter_free)->cut_points_data.cut_points->end())
		{
			delete (*iter_free_line);
			++iter_free_line;
		}
		(*iter_free)->cut_points_data.cut_points->clear();
		delete (*iter_free)->cut_points_data.cut_points;
		++iter_free;
	}
	return MSTAT_OK;
}

mgbool show_face_loop()
{
	mgSendMessage(MMSG_STATUS, "-----Show face loop node-----");
	iter_f_l = f_l.begin();
	while(iter_f_l != f_l.end())
	{
		mgSendMessage(MMSG_STATUS, "Face name: %1s", mgGetName((*iter_f_l)->face));
		MGMSG("pos: %1d, %2d", (*iter_f_l)->pos[0], (*iter_f_l)->pos[1])
		++iter_f_l;
	}
	mgSendMessage(MMSG_STATUS, "------------end------------");
	return MG_TRUE;
}

mgbool another_edge_pos(const short* pos, short* anti_pos)
{
	for(short i = 0; i<2;i++)
	{
		switch (pos[i])
		{
		case 1:
			anti_pos[i] = 3;
			break;
		case 2:
			anti_pos[i] = 4;
			break;
		case 3:
			anti_pos[i] = 1;
			break;
		case 4:
			anti_pos[i] = 2;
			break;
		default: 
			break;
		}
	}

	return MG_TRUE;
}

mgbool get_cut_point(const PLUGINTOOLSTRUCT* pt_s)
{
	// Initialize an iterator for the face loops.
	iter_f_l = f_l.begin();

	// Loop through each face loop and calculate the cut points.
	while (iter_f_l != f_l.end())
	{
		// Loop through each face loop and calculate the cut points.
		if ((*iter_f_l)->coincide_edge->edge_shared_count > 2)
		{
			return MG_FALSE;
		}

		// Loop through each face loop and calculate the cut points.
		short anti_pos[2] = { 0, 0 };
		another_edge_pos((*iter_f_l)->pos, anti_pos);

		// Loop through each face loop and calculate the cut points.
		mgrec* f_v[5]{ nullptr };
		mgcoord3d f_v_coord[5]{ {0,0,0} };

		// Set the first vertex to NULL and get the remaining vertices and coordinates of the face.
		f_v[0] = MG_NULL;
		f_v[1] = mgGetChildNth((*iter_f_l)->face, 1);
		f_v[2] = mgGetChildNth((*iter_f_l)->face, 2);
		f_v[3] = mgGetChildNth((*iter_f_l)->face, 3);
		f_v[4] = mgGetChildNth((*iter_f_l)->face, 4);

		f_v_coord[0] = mgCoord3dZero();
		f_v_coord[1] = get_vertex_coord(f_v[1]);
		f_v_coord[2] = get_vertex_coord(f_v[2]);
		f_v_coord[3] = get_vertex_coord(f_v[3]);
		f_v_coord[4] = get_vertex_coord(f_v[4]);

		// Create a vector to store the cut points and an array to store the cut points on the edge being cut.
		const auto temp_cut_points = new std::vector<mglined*>;
		const auto temp_cut_points_on_edge = new mglined[2];

		temp_cut_points_on_edge[0] = mgMakeLine(&f_v_coord[(*iter_f_l)->pos[0]], &f_v_coord[(*iter_f_l)->pos[1]]);
		temp_cut_points_on_edge[1] = mgMakeLine(&f_v_coord[anti_pos[1]], &f_v_coord[anti_pos[0]]);

		// Loop through the number of splits and calculate the cut points for each iteration.
		for(int i = 0; i < pt_s->split; i++)
		{
			// Calculate the amount to interpolate between the two edges.
			const double amount = (1.0 / (pt_s->split + 1)) * (i + 1);

			// Calculate the amount to interpolate between the two edges.
			auto cut_line = new mglined;
			cut_line->p1 = mgCoord3dLerp(&temp_cut_points_on_edge[0].p1, &temp_cut_points_on_edge[0].p2, amount);
			cut_line->p2 = mgCoord3dLerp(&temp_cut_points_on_edge[1].p1, &temp_cut_points_on_edge[1].p2, amount);
			temp_cut_points->push_back(cut_line);
		}

		// Calculate the amount to interpolate between the two edges.
		(*iter_f_l)->cut_points_data.cut_points = temp_cut_points;
		(*iter_f_l)->cut_points_data.cut_points_on_edge = temp_cut_points_on_edge;
		(*iter_f_l)->offset[total_offset] = 0.0;
		(*iter_f_l)->offset[last_offset] = 0.0;
		(*iter_f_l)->offset[current_offset] = 0.0;

		++iter_f_l;
	}

	return MG_TRUE;
}

mgbool draw_loop_cut_cst(const PLUGINTOOLSTRUCT* pt_s, const mgconstructcolor color)
{
	mgDeleteAllConstructs(pt_s->econtext);
	if (f_l.empty()) return MG_FALSE;
	iter_f_l = f_l.begin();

	while(iter_f_l != f_l.end())
	{
		if ((*iter_f_l)->coincide_edge->edge_shared_count > 2)
		{
			return MG_FALSE;
		}

		auto iter_e = (*iter_f_l)->cut_points_data.cut_points->begin();
		
		while (iter_e != (*iter_f_l)->cut_points_data.cut_points->end())
		{
			draw_edge_node_cst(pt_s, (*iter_e)->p1, (*iter_e)->p2, color);
			++iter_e;
		}
		++iter_f_l;
	}

	return MG_TRUE;
}

mgbool draw_edge_node_cst(const PLUGINTOOLSTRUCT* pt_s, mgcoord3d v10_coord, mgcoord3d v20_coord, const mgconstructcolor color)
{
	mgrec* cts_e = mgNewConstructEdge(pt_s->econtext, &v10_coord, &v20_coord);
	mgSetConstructEdgeColor(cts_e, color);

	return cts_e ? MG_TRUE : MG_FALSE;
}

mgbool cut_face_loop(const PLUGINTOOLSTRUCT* pt_s)
{
	iter_f_l = f_l.begin();

	while(iter_f_l != f_l.end())
	{
		if ((*iter_f_l)->coincide_edge->edge_shared_count > 2)
		{
			return MG_FALSE;
		}

		split_face(pt_s, *iter_f_l);

		++iter_f_l;
	}

	return MG_TRUE;
}
mgbool split_face(const PLUGINTOOLSTRUCT* pt_s, const face_loop* f_l_n)
{
	// Create an iterator for the cut points.
	auto iter_e = f_l_n->cut_points_data.cut_points->begin();

	// Initialize an array of coordinates for the vertices of the new polygons.
	short anti_pos[2] = { 0,0 };
	another_edge_pos(f_l_n->pos, anti_pos);
	mgcoord3d f_v_coord[5] = {
		mgCoord3dZero(),
		mgCoord3dZero(),
		mgCoord3dZero(),
		f_l_n->cut_points_data.cut_points_on_edge[0].p1,
		f_l_n->cut_points_data.cut_points_on_edge[1].p1
	};

	// Initialize an array to store the new polygons.
	std::vector<mgrec*> new_polygons;

	// Loop through the cut points and create a new polygon for each one.
	while (iter_e != f_l_n->cut_points_data.cut_points->end())
	{
		// Loop through the cut points and create a new polygon for each one.
		mgrec* new_p = mgDuplicateEx(f_l_n->face, MDUP_DERIVEDNAMES);
		new_polygons.push_back(new_p);

		// Attach the new polygon to the parent database.
		mgAttach(pt_s->parent, new_p);

		// Attach the new polygon to the parent database.
		mgEditorAppendUndoForCreate(pt_s->econtext, new_p);

		// Update the vertex coordinates of the new polygon based on the cut points.
		f_v_coord[1] = f_v_coord[4];
		f_v_coord[2] = f_v_coord[3];
		f_v_coord[3] = (*iter_e)->p1;
		f_v_coord[4] = (*iter_e)->p2;

		for(int i = 1; i < 5; i++)
		{
			mgSetVtxCoord(mgGetChildNth(new_p, i), f_v_coord[i].x, f_v_coord[i].y, f_v_coord[i].z);
		}
		++iter_e;
	}

	// Create one final polygon using the last set of cut points.
	mgrec* new_p = mgDuplicateEx(f_l_n->face, MDUP_DERIVEDNAMES);
	new_polygons.push_back(new_p);
	mgAttach(pt_s->parent, new_p);
	mgEditorAppendUndoForCreate(pt_s->econtext, new_p);

	f_v_coord[1] = f_v_coord[4];
	f_v_coord[2] = f_v_coord[3];
	f_v_coord[3] = f_l_n->cut_points_data.cut_points_on_edge[0].p2;
	f_v_coord[4] = f_l_n->cut_points_data.cut_points_on_edge[1].p2;

	for (int i = 1; i < 5; i++)
	{
		mgSetVtxCoord(mgGetChildNth(new_p, i), f_v_coord[i].x, f_v_coord[i].y, f_v_coord[i].z);
	}

	// Create one final polygon using the last set of cut points.
	mgEditorAppendUndoForDelete(pt_s->econtext, f_l_n->face);

	return MG_TRUE;
}

mgbool increase_number_of_split(PLUGINTOOLSTRUCT* pt_s, const int num)
{
	// Get the GUI element that displays the number of splits.
	const mggui gui = mgFindGuiById(pt_s->dialog, EControl_Number);  // NOLINT(misc-misplaced-const)

	// Update the number of splits based on the input value.
	pt_s->split = num > 0 ? num : 1;

	// Update the number of splits based on the input value.
	if (MSTAT_ISOK(mgRefreshDialog(gui)))
	{
		return MG_TRUE;
	}

	// If the dialog refresh failed, return MG_FALSE.
	return MG_FALSE;
}

mgbool move_cut_point(const PLUGINTOOLSTRUCT* pt_s, const double delta)
{
	auto iter = f_l.begin();

	while(iter != f_l.end())
	{
		// Apply an offset to the current object's offsets.
		(*iter)->offset[current_offset] = delta / 1000.0;

		// If the offset is close to zero, set the last offset to zero.
		if (abs((delta - 0.0)) < 0.0000001)
		{
			(*iter)->offset[last_offset] = 0.0;
		}

		// Calculate the final offset based on the current, total, and last offsets.
		const double final_offset =
			clip_number((*iter)->offset[total_offset] + (*iter)->offset[current_offset] - (*iter)->offset[last_offset],
				-1.0 / (pt_s->split + 1), 
				1.0 / (pt_s->split + 1));

		// Calculate the delta offset and update the current, total, and last offsets.
		const double delta_offset = final_offset - (*iter)->offset[total_offset];
		(*iter)->offset[total_offset] = final_offset;
		(*iter)->offset[last_offset] = (*iter)->offset[current_offset];

		// Iterate through the cut points of the current object.
		auto iter_c_p = (*iter)->cut_points_data.cut_points->begin();
		while (iter_c_p != (*iter)->cut_points_data.cut_points->end())
		{
			// Get the vector between the cut points.
			mgvectord original_edge_vector1 = mgMakeVectord(
				&(*iter)->cut_points_data.cut_points_on_edge[0].p1,
				&(*iter)->cut_points_data.cut_points_on_edge[0].p2);
			mgvectord original_edge_vector2 = mgMakeVectord(
				&(*iter)->cut_points_data.cut_points_on_edge[1].p1,
				&(*iter)->cut_points_data.cut_points_on_edge[1].p2);

			// Convert the normalized vector to a coordinate.
			mgcoord3d normalize_coord = mgVectordToCoord3d(&original_edge_vector1);
			mgcoord3d normalize_coord2 = mgVectordToCoord3d(&original_edge_vector2);

			// Calculate the offset for the first cut point.
			mgcoord3d p1_offset = mgCoord3dMultiply(&normalize_coord, delta_offset);
			mgcoord3d p2_offset = mgCoord3dMultiply(&normalize_coord2, delta_offset);

			// Calculate the offset for the first cut point.
			(*iter_c_p)->p1 = mgCoord3dAdd(&(*iter_c_p)->p1, &p1_offset);
			(*iter_c_p)->p2 = mgCoord3dAdd(&(*iter_c_p)->p2, &p2_offset);

			// Move to the next cut point.
			++iter_c_p;
		}
		// Move to the next cut point.
    	++iter;
	}

	// Draw a loop cut using the modified cut points, colored red.
	draw_loop_cut_cst(pt_s, mgconstructcolor::MCCOLOR_RED);

	return MG_TRUE;
}

double clip_number(const double value, const double min, const double max)
{
	// If the value is less than the minimum, return the minimum.
	if (value < min) {
		return min;
	}
	// If the value is greater than the maximum, return the maximum.
	if (value > max) {
		return max;
	}
	// Otherwise, return the original value.
	return value;
}
