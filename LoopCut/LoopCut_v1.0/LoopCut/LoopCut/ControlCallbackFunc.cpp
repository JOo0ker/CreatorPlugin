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

LRESULT CALLBACK mouse_proc(int n_code, WPARAM w_param, LPARAM l_param)
{
	//if (n_code >= HC_ACTION)
	//{
	//	switch (w_param)
	//	{
	//	case WM_MBUTTONDOWN:
	//		MGMSG("middle button down")
	//		
	//		if(MSTAT_ISOK(mgEditorSelectMouseInput(pt_s.econtext, MMSI_VERTEXINPUT)))
	//		{
	//			MGMSG("MMSI_VERTEXINPUT")
	//		}
	//		break;

	//	case WM_MBUTTONUP:
	//		MGMSG("middle button up")
	//		if(MSTAT_ISOK(mgEditorSelectMouseInput(pt_s.econtext, MMSI_PICKINPUT)))
	//		{
	//			MGMSG("MMSI_PICKINPUT")
	//		}
	//		break;
	//	}
	//}

	// Pass the event to the next hook procedure
	return CallNextHookEx(nullptr, n_code, w_param, l_param);
}

LRESULT CALLBACK keyboard_proc(int n_code, WPARAM w_param, LPARAM l_param)
{
	//if (n_code == HC_ACTION)
	//{
	//	const auto kbd_struct = reinterpret_cast<KBDLLHOOKSTRUCT*>(l_param);

		//MGMSG("code : %1d", n_code)
		//MGMSG("w_param : %1d", w_param)
		//MGMSG("vkCode : %1d", kbd_struct->vkCode)
		//MGMSG("scanCode : %1d", kbd_struct->scanCode)
		//MGMSG("flags : %1d", kbd_struct->flags)
		//MGMSG("time : %1d", kbd_struct->time)
		//MGMSG("dwExtraInfo : %1d", kbd_struct->dwExtraInfo)

	//	switch (kbd_struct->vkCode)
	//	{
	//	case VK_F7:
	//		MGMSG("VK_F7 func")
	//			if (w_param == WM_KEYDOWN)
	//			{
	//				MGMSG("VK_F7 pressed.")
	//			}

	//		break;
	//	case VK_LCONTROL:
	//		MGMSG("VK_LCONTROL func")
	//			if (w_param == WM_KEYDOWN)
	//			{
	//				MGMSG("VK_LCONTROL pressed.")
	//				if(current_mouse_input != MMSI_POINTINPUT)
	//				{
	//					MGMSG("Set MMSI_POINTINPUT...")
	//					if(MSTAT_ISOK(mgEditorSelectMouseInput(pt_s.econtext, MMSI_POINTINPUT)))
	//					{
	//						MGMSG("MMSI_POINTINPUT set successful.")
	//					}
	//				}
	//			}
	//			else if (w_param == WM_KEYUP)
	//			{
	//				if(current_mouse_input == MMSI_POINTINPUT)
	//				{
	//					MGMSG("Set MMSI_PICKINPUT...")
	//					if(MSTAT_ISOK(mgEditorSelectMouseInput(pt_s.econtext, MMSI_PICKINPUT)))
	//					{
	//						MGMSG("MMSI_PICKINPUT se successful.")
	//					}
	//				}
	//			}
	//		break;
	//	default:
	//		break;
	//	}
	//}

	//if (GetForegroundWindow() == creator_hwnd &&
	//	GetWindowThreadProcessId(GetForegroundWindow(), nullptr) == creator_thread_id)
	//{
	//	return 1;
	//}

	// Pass the event to the next hook procedure
	return CallNextHookEx(nullptr, n_code, w_param, l_param);
}
