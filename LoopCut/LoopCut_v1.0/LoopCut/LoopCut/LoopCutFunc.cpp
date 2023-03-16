#include "LoopCutFunc.h"

#include <cstring>

static short match_first_face_times = 0;
face_loop* f_l;

mgstatus cast_test_func(mgrec* db)
{
	mgrec* polygon = mgNewRec(fltPolygon);
	mgSetPolyCurrentColor(db, polygon);

	mgrec* vtx[4] { MG_NULL };

	vtx[0] = mgNewRec(fltVertex);
	vtx[1] = mgNewRec(fltVertex);
	vtx[2] = mgNewRec(fltVertex);
	vtx[3] = mgNewRec(fltVertex);

	mgSetVtxCoord(vtx[0], 1.0, 1.0, 0.0);
	mgSetVtxCoord(vtx[1], -1.0, 1.0, 0.0);
	mgSetVtxCoord(vtx[2], -1.0, -1.0, 0.0);
	mgSetVtxCoord(vtx[3], 1.0, -1.0, 0.0);

	for (int i = 0; i < 4; i++)
	{
		mgAppend(polygon, vtx[i]);
	}

	unsigned int temp;

	mgGetAttList(polygon, fltPolyCreatorPrimeColor, &temp, MG_NULL);

	mgSendMessage(MMSG_STATUS, "fltPolyCreatorPrimeColor = %1u", temp);

	mgSetAttList(polygon, fltGcLightMode, 2, MG_NULL);

	mgrec* parent = mgGetModelingParent(db);
	mgAttach(parent, polygon);

	return MSTAT_OK;
}

mgstatus select_loop(PLUGINTOOLSTRUCT* pt_s, mgrec* v1)
{
	// distinguish between a single face and a loop face
	match_first_face_times = 0;

	// first face node
	free_face_loop(f_l);
	f_l = static_cast<face_loop*>(mgMalloc(sizeof(face_loop)));

	// v1
	f_l->face = mgGetParent(v1);
	const mgcoord3d v1_coord = get_vertex_coord(v1);

	// find the selected vertex position
	for (f_l->pos[0] = 1; ; f_l->pos[0]++)
	{
		mgcoord3d temp = get_vertex_coord(mgGetChildNth(f_l->face, f_l->pos[0]));
		if(mgCoord3dEqual(&v1_coord, &temp))
		{
			break;
		}
	}

	// v2		NOTE: the vertex represented by the edge is always the previous vertex.
	mgrec* v2 = get_loop_vtx(v1, NEXT, &f_l->pos[1]);

	// edge
	const auto edge = static_cast<mg_edge*>(mgMalloc(sizeof(mg_edge)));
	edge->v1 = v1;
	edge->v2 = v2;
	edge->edge_shared_count = 1;
	f_l->coincide_edge_first = MG_NULL;
	f_l->coincide_edge_second = edge;

	// prev/next/first
	f_l->prev = MG_NULL;
	f_l->next = MG_NULL;
	f_l->first = f_l;

	get_face_loop(pt_s, f_l);
	//show_face_fool(f_l);
	draw_loop_cut_cst(pt_s, f_l);

	return MG_TRUE;
}

mgrec* get_loop_vtx(mgrec* v, loop_dire dire, short* pos)
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
					m_v_p = (i - 1) > 0 ? (i - 1) : total;
					break;
				case NEXT:
					m_v_p = (i + 1) <= total ? (i + 1) : 1;
					break;
				default:
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

mgbool get_face_loop(PLUGINTOOLSTRUCT* pt_s, face_loop* f_l)
{
	// get the nearest group 
	mgrec* group = mgGetParent(f_l->face);
	while (
		group // mgGetParent(Db) => MG_NULL
		&& mgGetCode(group) != fltGroup)
	{
		group = mgGetParent(group);
	}

	switch (mgGetCode(group))
	{
	case fltGroup:
		mgSendMessage(MMSG_STATUS, " The group name is : %1s .", mgGetName(group));
		break;

	default:
		mgSendMessage(MMSG_TIP, "Please make sure that you have a GROUP node.");
		return MG_FALSE;
	}

	// walk the group and get the face loop

	while (true)
	{
		mgWalk(group,
			mgw_per_check_edge_in_polygon,
			mgw_post_check_edge_in_polygon,
			f_l,
			MWALK_ON | MWALK_VERTEX);
		//show_face_fool(f_l);

		if (f_l->first->prev)
		{// got a whole loop
			break;
		}


		switch (f_l->coincide_edge_second->edge_shared_count)
		{
		case 0:// invalid selected face
			return MG_FALSE;

		case 1:// isolate edge of face
			return MG_TRUE;

		case 2:// standard edge of face
			//return MG_TRUE;
			f_l = f_l->next;
			break;

		default:// other invalid face
			return MG_TRUE;
		}
	}

	return MG_TRUE;
}

mgbool check_edge_in_polygon(face_loop* f_l, mgrec* p, short* pos)
{
	// the source edge
	const mg_edge* e = f_l->coincide_edge_second;
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
	short i = 0;

	// whether the coordinate of ith vertex is the same as the 1st vertex of source edge
	mgbool b_conform = MG_FALSE;

	// iterate through all vertices that are located beneath the target polygon,
	// and compare them with the 1st vertex of the source edge.
	for(; i < 4; i++)
	{
		if (mgCoord3dEqual(&e_coord[0], &p_v_coord[i]))
		{
			b_conform = MG_TRUE;
			break;
		}
	}

	// if find a vertex in the target polygon that has the same coordinates as the 1st vertex of the source edge
	if (b_conform)
	{
		mgbool b_conform2 = MG_FALSE;

		// restore the position of coincide edge vertices  (i: from 0 to 3)
		pos[0] = i + 1;


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

mgbool mgw_per_check_edge_in_polygon(mgrec* db, mgrec* parent, mgrec* rec, void* user_data)
{
	if(mgGetCode(rec) == fltPolygon)
	{
		if (mgCountChild(rec) != 4)
		{// match an invalid polygon that does not have exactly 4 vertices as its components.
			//mgSendMessage(MMSG_STATUS, "Catch the not 4 vtx polygon.");
			return MG_TRUE;
		}

		const auto f_l = static_cast<face_loop*>(user_data);

		short pos[2]{ 0,0 };

		if(check_edge_in_polygon(f_l, rec, pos))
		{// found a matching edge
			faceloopcase fl_case = insert_face_loop_node(f_l, rec, pos);
			switch (fl_case)
			{
			case faceloopcase::unknown:
				break;
			case faceloopcase::normal:
				f_l->coincide_edge_second->edge_shared_count += 1;
				break;
			case faceloopcase::match_face_first_time:
				break;
			case faceloopcase::match_face_second_time:
				f_l->coincide_edge_second->edge_shared_count += 1;
				break;
			case faceloopcase::match_previous_face:
				break;
			default: 
				break;
			}

			return MG_TRUE;
		}
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

faceloopcase insert_face_loop_node(face_loop* f_l, mgrec* p, const short* pos)
{
	if(strcmp(mgGetName(p), mgGetName(f_l->first->face)) == 0) // the same as first face
	{
		match_first_face_times += 1;

		switch (match_first_face_times)
		{
		case 1:
			return faceloopcase::match_face_first_time;

		case 2:
			f_l->first->prev = f_l;
			f_l->next = f_l->first;
			return faceloopcase::match_face_second_time;

		default:
			return faceloopcase::unknown;
		}

	}

	if(f_l->prev
		&& (strcmp(mgGetName(p), mgGetName(f_l->face)) == 0) // the same as previous face
		)
	{
		//mgSendMessage(MMSG_STATUS, "Match the previous face.");
		return faceloopcase::match_previous_face;
	}

	// create a new face loop node to restore the face
	auto* new_face_loop_node = static_cast<face_loop*>(mgMalloc(sizeof(face_loop)));

	// create a new edge node to restore edge
	auto* new_edge = static_cast<mg_edge*>(mgMalloc(sizeof(mg_edge)));

	short anti_pos[2];

	another_edge_pos(pos, anti_pos);

	new_edge->v1 = mgGetChildNth(p, anti_pos[0]);
	new_edge->v2 = mgGetChildNth(p, anti_pos[1]);
	new_edge->edge_shared_count = 1;

	new_face_loop_node->coincide_edge_first = f_l->coincide_edge_second;
	new_face_loop_node->coincide_edge_second = new_edge;
	new_face_loop_node->pos[0] = pos[0];
	new_face_loop_node->pos[1] = pos[1];
	new_face_loop_node->face = p;
	new_face_loop_node->first = f_l->first;
	new_face_loop_node->prev = f_l;
	new_face_loop_node->next = MG_NULL;

	f_l->next = new_face_loop_node;

	return faceloopcase::normal;
}

mgstatus free_face_loop(face_loop* f_l)
{
	// nullptr input
	if (!f_l) return MG_TRUE;

	// free from the beginning
	f_l = f_l->first;

	// restore the current node
	face_loop* temp = MG_NULL;

	// restore the face name of first node
	const char* first_face_name = mgGetName(f_l->face);

	// free all exist node
	while (f_l)
	{
		mgFree(f_l->coincide_edge_first);
		mgFree(f_l->coincide_edge_second);

		temp = f_l;
		if (f_l->next)
		{
			//mgSendMessage(MMSG_STATUS, "f_l->next : %1s.",
			//	mgGetName(f_l->next->face));
			f_l = f_l->next;
		}
		mgFree(temp);

		if (mgGetName(f_l->next->face)
			&& !strcmp(
			mgGetName(f_l->next->face),
			first_face_name))
		{
			break;
		}
			
		break;
	}

	return MG_TRUE;
}

mgbool show_face_fool(const face_loop* f_l)
{
	mgSendMessage(MMSG_STATUS, "-----Show face loop node-----");
	const face_loop* temp_f_l = f_l->first;

	while (temp_f_l)
	{
		mgSendMessage(MMSG_STATUS, "Face name: %1s", mgGetName(temp_f_l->face));

		if (temp_f_l->next)
			mgSendMessage(MMSG_STATUS, "Next name: %1s", mgGetName(temp_f_l->next->face));
		else
			break;

		temp_f_l = temp_f_l->next;

		if (strcmp(mgGetName(temp_f_l->face) , mgGetName(f_l->face)) == 0)
			break;
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

mgbool match_face_loop_end(const face_loop* f_l)
{


	// complete loop
	if (
		strcmp(
			mgGetName(f_l->face),
			mgGetName(f_l->first->face)) == 0
		)
	{
		return MG_TRUE;
	}

	return MG_FALSE;
}

mgbool draw_loop_cut_cst(PLUGINTOOLSTRUCT* pt_s, const face_loop* f_l)
{
	f_l = f_l->first;

	// isolated face
	if(!f_l->next)
	{
		mgrec* v11 = f_l->coincide_edge_second->v1;
		mgcoord3d v11_coord = get_vertex_coord(v11);

		mgrec* v12 = f_l->coincide_edge_second->v2;
		mgcoord3d v12_coord = get_vertex_coord(v12);

		short anti_pos[2];
		another_edge_pos(f_l->pos, anti_pos);
		

		mgrec* v21 = mgGetChildNth(f_l->face, anti_pos[0]);
		mgcoord3d v21_coord = get_vertex_coord(v21);

		mgrec* v22 = mgGetChildNth(f_l->face, anti_pos[1]);
		mgcoord3d v22_coord = get_vertex_coord(v22);

		const mgcoord3d v10_coord = mgCoord3dLerp(&v11_coord, &v12_coord, 0.5);
		const mgcoord3d v20_coord = mgCoord3dLerp(&v21_coord, &v22_coord, 0.5);

		draw_edge_node_cst(pt_s, v10_coord, v20_coord);
		return MG_TRUE;
	}

	// more than one face
	do
	{
		mgrec* v11 = f_l->coincide_edge_second->v1;
		mgcoord3d v11_coord = get_vertex_coord(v11);

		mgrec* v12 = f_l->coincide_edge_second->v2;
		mgcoord3d v12_coord = get_vertex_coord(v12);

		mgrec* v21 = f_l->next->coincide_edge_second->v1;
		mgcoord3d v21_coord = get_vertex_coord(v21);

		mgrec* v22 = f_l->next->coincide_edge_second->v2;
		mgcoord3d v22_coord = get_vertex_coord(v22);

		const mgcoord3d v10_coord = mgCoord3dLerp(&v11_coord, &v12_coord, 0.5);
		const mgcoord3d v20_coord = mgCoord3dLerp(&v21_coord, &v22_coord, 0.5);

		draw_edge_node_cst(pt_s, v10_coord, v20_coord);
		f_l = f_l->next;
	} while (!match_face_loop_end(f_l));

	return MG_TRUE;
}

mgbool draw_edge_node_cst(PLUGINTOOLSTRUCT* pt_s, mgcoord3d v10_coord, mgcoord3d v20_coord)
{

	mgrec* cts_e = mgNewConstructEdge(pt_s->econtext, &v10_coord, &v20_coord);

	//mgEditorAppendUndoForCreate(pt_s->econtext, cts_e);
	return MG_TRUE;
}

mgbool cut_face_loop(PLUGINTOOLSTRUCT* pt_s)
{
	f_l = f_l->first;
	mgSendMessage(MMSG_STATUS, "first face pos : %1d, %2d", f_l->pos[0], f_l->pos[1]);

	return MG_TRUE;
}



mgbool is_collinear()
{



	//mgCoord3dCross()


	return mgbool();
}
