#include "ControlCallbackFunc.h"

mgstatus e_control_callback_func(mggui gui, mgcontrolid control_id,
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
