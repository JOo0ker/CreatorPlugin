#pragma once

#include "ToolHead.h"
//#include "LoopCutFunc.h"

mgstatus e_control_callback(mggui gui, mgcontrolid control_id,
	mgguicallbackreason callback_reason,
	void* user_data, void* call_data);

mgstatus s_control_number_callback_func(mggui control,
	mgcontrolid control_id,
	mgguicallbackreason callback_reason,
	void* user_data,
	void* call_data);

LRESULT CALLBACK mouse_proc(int n_code, WPARAM w_param, LPARAM l_param);

LRESULT CALLBACK keyboard_proc(int n_code, WPARAM w_param, LPARAM l_param);