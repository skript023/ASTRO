#include <gui.hpp>
#include "overlay.h"

namespace big
{
	void overlay::render_overlay()
	{
		ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_FirstUseEver, ImVec2(0.0f, 0.0f));
		ImGui::SetNextWindowBgAlpha(0.5f);

		auto window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

		if (!g_gui.m_opened) window_flags |= ImGuiWindowFlags_NoMouseInputs;

		if (ImGui::Begin("overlay", nullptr, window_flags))
		{
			ImGui::Text("Overlay Info");
		}
		ImGui::End();
	}
}