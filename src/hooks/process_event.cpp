#include "hooking.hpp"
#include "script_mgr.hpp"
#include "utility/unreal_engine_utility.hpp"
#include "SDK/TsAnimNotifyChangeSkillPriority_parameters.hpp"

namespace big
{
	void APIENTRY hooks::process_event(SDK::UObject* _this, SDK::UFunction* function, void* parms)
	{
		if (function->GetFullName() == "Function Engine.ActorComponent.ReceiveTick")
		{
			g_script_mgr.tick();
		}
		if (function->GetFullName() == "Function TsAnimNotifyChangeSkillPriority.TsAnimNotifyChangeSkillPriority_C.K2_Notify")
		{
			auto params = static_cast<SDK::Params::TsAnimNotifyChangeSkillPriority_C_K2_Notify*>(parms);


		}
		
		return g_hooking->get_original<process_event>()(_this, function, parms);
	}
}