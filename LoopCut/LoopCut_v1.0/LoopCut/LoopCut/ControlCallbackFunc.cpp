#include "ControlCallbackFunc.h"

extern std::vector<face_loop*> f_l;
extern mgbool get_cut_point(const PLUGINTOOLSTRUCT* pt_s);
extern mgbool draw_loop_cut_cst(const PLUGINTOOLSTRUCT* pt_s, const mgconstructcolor color);
extern mgstatus free_cut_points();

mgstatus e_control_callback(mggui gui, mgcontrolid control_id,
	mgguicallbackreason callback_reason,
	void* user_data, void* call_data)
{
	const auto pt_s = static_cast<PLUGINTOOLSTRUCT*>(user_data);
	switch (callback_reason)
	{
	case MGCB_ACTIVATE:
	{
		int i_val;
		if (MSTAT_ISOK(mgTextGetInteger(gui, &i_val))) 
		{
			pt_s->split = i_val > 0 ? i_val : 1;
			mgRefreshDialog(gui);
		}
	}
	break;
	case MGCB_REFRESH:
		free_cut_points();
		get_cut_point(pt_s);
		draw_loop_cut_cst(pt_s, mgconstructcolor::MCCOLOR_RED);
		mgTextSetInteger(gui, pt_s->split, "%d");
		break;
	default:
		break;
	}
	return (MSTAT_OK);
}


mgstatus s_control_number_callback_func(mggui control, mgcontrolid control_id, mgguicallbackreason callback_reason, void* user_data, void* call_data)
{

	return MSTAT_OK;
}