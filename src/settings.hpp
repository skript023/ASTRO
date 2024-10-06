#pragma once
#include "logger.hpp"
#include "imgui.h"

namespace big
{
	class settings {
		nlohmann::json default_options;
		nlohmann::json options;
	public:
		void attempt_save();

		bool load();
	private:
		const char* settings_location = "\\" FOLDER_NAME "\\settings.json";

		bool deep_compare(nlohmann::json& current_settings, const nlohmann::json& default_settings, bool compare_value = false);
		bool save();

		bool write_default_config();
	public:
		struct self
		{
			bool inf_health{ false };
			bool inf_stamina{ false };
			bool inf_ammo{ false };
			bool no_cost_shot{ false };
			bool skill_speed{ false };
			bool no_cd_switch{ false };
			bool rapid_fire{ false };
			bool no_spread{ false };
			bool no_recoil{ false };
			bool godmode{ false };
			bool no_clip{ false };

			float speed{ 0.0f };
			int num_bullet{ 0 };

			NLOHMANN_DEFINE_TYPE_INTRUSIVE(self, inf_health, inf_stamina, inf_ammo, no_cost_shot, skill_speed, no_cd_switch, speed, rapid_fire, num_bullet, no_recoil, no_spread, godmode, no_clip)
		} self;

		struct esp
		{
			bool is_all_entity = false;
			bool is_active = true;
			bool draw_line = true;
			bool draw_name = true;
			bool draw_box = false;
			float global_render_distance[2] = { 0.f, 600.f };
			float tracer_render_distance[2] = { 200.f, 600.f };
			float box_render_distance[2] = { 0.f, 150.f };
			float tracer_draw_position[2] = { 0.5f, 1.f };

			NLOHMANN_DEFINE_TYPE_INTRUSIVE(esp, is_all_entity, is_active, draw_line, draw_name, draw_box)
		} esp;

		struct window
		{
			ImU32 color = 3357612055;
			float gui_scale = 1.f;

			ImFont* font_title = nullptr;
			ImFont* font_sub_title = nullptr;
			ImFont* font_small = nullptr;
			ImFont* font_icon = nullptr;

			bool switched_view = true;
			bool mouse_active = false;

			NLOHMANN_DEFINE_TYPE_INTRUSIVE(window, color, gui_scale, mouse_active)
		} window;

		struct debug
		{
			bool external_console = true;

			NLOHMANN_DEFINE_TYPE_INTRUSIVE(debug, external_console)
		} debug;
	

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(settings, window, self, debug, esp)
	};

	inline settings g_settings{};
}