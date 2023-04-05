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