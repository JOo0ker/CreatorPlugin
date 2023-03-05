#include "LoopCutFunc.h"

mgstatus cast_test_func(mgrec* db)
{
	mgrec* polygon = mgNewRec(fltPolygon);
	mgSetPolyCurrentColor(db, polygon);

	mgrec* vtx[4];

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
	face_loop f_l;

	// v1
	f_l.face = mgGetParent(v1);
	mgcoord3d v1_coord = get_vertex_coord(v1);
	for (f_l.pos[0] = 1; ; f_l.pos[0]++)
	{
		mgcoord3d temp = get_vertex_coord(mgGetChildNth(f_l.face, f_l.pos[0]));
		if(mgCoord3dEqual(&v1_coord, &temp))
		{
			break;
		}
	}

	// v2
	mgrec* v2 = get_loop_vtx(v1, NEXT, &f_l.pos[1]);
	mgcoord3d v2_coord = get_vertex_coord(v2);

	// edge
	mg_edge edge{ v1, v2,0 };
	f_l.coincide_edge[0] = &edge;

	// prev/next/first
	f_l.prev = &f_l;
	f_l.next = &f_l;
	f_l.first = &f_l;


	get_face_loop(pt_s, &f_l);


	mgcoord3d cts_vtx = mgCoord3dLerp(&v1_coord, &v2_coord, 0.5);
	mgrec* cts = mgNewConstructVertex(pt_s->econtext, &cts_vtx);

	mgEditorAppendUndoForCreate(pt_s->econtext, cts);

	return MG_TRUE;
}

mgrec* get_loop_vtx(mgrec* v1, loop_dire dire, short* pos)
{
	mgrec* parent = mgGetParent(v1);
	const int total = mgCountChild(parent);

	short temp = 0;

	if (total <= 1)
	{
		temp = 1;
		return v1;
	}

	mgrec* v2;
	switch (dire)
	{
	case PREVIOUS:
		temp -= 1;
		v2 = mgGetPrevious(v1);
		if (!v2)
		{
			v2 = mgGetChildNth(parent, total);
			temp = total;
		}
		break;
	case NEXT:
		temp += 1;
		v2 = mgGetNext(v1);
		if (!v2)
		{
			v2 = mgGetChild(parent);
			temp = 1;
		}
		break;
	default:
		v2 = v1;
		break;
	}

	if (*pos) *pos = temp;
	return v2;
}

mgbool get_face_loop(PLUGINTOOLSTRUCT* pt_s, face_loop* f_l)
{

	mgrec* group = mgGetParent(f_l->face);
	while (mgGetCode(group) != fltGroup)
	{
		group = mgGetParent(group);
	}
	mgSendMessage(MMSG_STATUS, " The group name is : %1s .", mgGetName(group));

	if(!group)
	{
		mgSendMessage(MMSG_TIP, "Please make sure that you have a GROUP node.");
		return MG_FALSE;
	}

	mgWalk(group, mgw_per_check_edge_in_polygon, mgw_post_check_edge_in_polygon, f_l, MWALK_ON | MWALK_VERTEX);

	return mgbool();
}

mgbool check_edge_in_polygon(face_loop* f_l, mgrec* p)
{
	mgSendMessage(MMSG_STATUS, "==========>Check edge in polygon %1s", mgGetName(p));

	const mg_edge* e = f_l->coincide_edge[0];

	mgcoord3d e_coord[2], p_v_coord[4];

	e_coord[0] = get_vertex_coord(e->v1);
	e_coord[1] = get_vertex_coord(e->v2);

	p_v_coord[0] = get_vertex_coord(mgGetChildNth(p, 1));
	p_v_coord[1] = get_vertex_coord(mgGetChildNth(p, 2));
	p_v_coord[2] = get_vertex_coord(mgGetChildNth(p, 3));
	p_v_coord[3] = get_vertex_coord(mgGetChildNth(p, 4));

	short i = 0;
	mgbool b_conform = MG_FALSE;
	for(; i < 4; i++)
	{
		if (mgCoord3dEqual(&e_coord[0], &p_v_coord[i]))
		{
			b_conform = MG_TRUE;
			break;
		}
	}
	if (b_conform)
	{
		short pos[2] = { i, 0 };

		const mgcoord3d p_vc_pre = get_vertex_coord(get_loop_vtx(mgGetChildNth(p, i+1), PREVIOUS, &pos[1]));
		if (mgCoord3dEqual(&p_vc_pre, &e_coord[1]))
		{
			auto* new_loop_node = new face_loop;
			auto* new_edge = new mg_edge{ mgGetChildNth(p, pos[0]), mgGetChildNth(p, pos[1]) , 1};
			new_loop_node->coincide_edge[0] = new_edge;
			*new_loop_node->pos = *pos;
			new_loop_node->face = p;
			new_loop_node->prev = f_l;
			new_loop_node->next = f_l->first;
			f_l->next = new_loop_node;
			f_l->coincide_edge[1] = new_edge;

			return MG_TRUE;
		}

		const mgcoord3d p_vc_next = get_vertex_coord(get_loop_vtx(mgGetChildNth(p, i + 1), NEXT, &pos[1]));
		if (mgCoord3dEqual(&p_vc_next, &e_coord[1]))
		{
			return MG_TRUE;
		}
	}
	return MG_FALSE;
}

mgbool mgw_per_check_edge_in_polygon(mgrec* db, mgrec* parent, mgrec* rec, void* user_data)
{
	auto f_l = static_cast<face_loop*>(user_data);

	if(mgGetCode(rec) == fltPolygon)
	{
		if (mgCountChild(rec) != 4)
		{
			mgSendMessage(MMSG_STATUS, "Catch the 4+ vtx polygon.");
			return MG_FALSE;
		}
		if(check_edge_in_polygon(f_l, rec))
		{
			f_l->coincide_edge[0]->edge_shared_count += 1;
			mgSendMessage(MMSG_STATUS, "The coincide edge number is : %1d, %2d", f_l->pos[0], f_l->pos[1]);
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
		mgcoord3d temp;
		mgGetVtxCoord(vtx, &temp.x, &temp.y, &temp.z);
		return mgcoord3d{ temp.x, temp.y, temp.z };
	}

	return mgCoord3dZero();
}
