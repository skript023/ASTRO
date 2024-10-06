#include "esp.h"
#include "submenu.hpp"
#include "utility/draw.hpp"
#include "utility/player.hpp"
#include <gui.hpp>

namespace big
{
    bool is_actor_of_interest(const std::string& name)
    {
        static const std::unordered_set<std::string> keywords = {
            //"BP_BaseItem_C_",
            //"BP_NA",
            //"BP_MO",
            //"BP_ME",
            //"BP_NH_",
            "BP_KuroISMGroup_C_",
            "SceneInteractionActor_C_",
            "BP_Vision_ME",
            "BP_Vision_MO",
            "BP_KuroDestructibleActor_C_",
            "TPrefab_SM_Gat_",
            "TPrefab_SM_Pro_Box_",
            "TPrefab_SM_Pro_WGZS_",
            "BP_InteractedBox_C_",
            "TriggerBox_",
            "TPrefab_SM_Pro_HuanXiangBox_"
        };

        // Check for any keyword in the actor's name
        return std::any_of(keywords.begin(), keywords.end(), [&](const std::string& keyword) {
            return name.find(keyword) != std::string::npos;
        });
    }
    void esp::esp_setting()
    {
        g_ui_manager->add_submenu<regular_submenu>("ESP", SubmenuESP, [](regular_submenu* sub)
        {
            sub->add_option<bool_option<bool>>("Enable ESP", nullptr, &g_settings.esp.is_active);
            sub->add_option<bool_option<bool>>("All Entity ESP", "ESP must be enabled", &g_settings.esp.is_all_entity);
            sub->add_option<bool_option<bool>>("ESP Line", "ESP must be enabled", &g_settings.esp.draw_line);
            sub->add_option<bool_option<bool>>("ESP Name", "ESP must be enabled", &g_settings.esp.draw_name);
            sub->add_option<bool_option<bool>>("ESP Box", "ESP must be enabled", &g_settings.esp.draw_box);
            sub->add_option<number_option<float>>("Max Draw Distance", "ESP must be enabled", &g_settings.esp.global_render_distance[1], 0.f, 1000.f, 0.1f, 1.f);
        });
    }
    void esp::draw_esp()
	{
        if (auto world = *g_pointers->m_world; world && g_settings.esp.is_active)
        {
            // Use a single player controller pointer
            auto ctrl = unreal_engine::get_player_controller();
            if (!ctrl) return;

            // Store the resolution width and height once
            float width = static_cast<float>(g_pointers->m_resolution->x / 2);
            float height = static_cast<float>(g_pointers->m_resolution->y / 2);

            for (auto levels : world->Levels)
            {
                if (!levels) continue;

                for (auto actor : levels->Actors)
                {
                    if (!actor || !actor->RootComponent) continue;

                    std::string name = actor->GetName();
                    if (!g_settings.esp.is_all_entity && !is_actor_of_interest(name)) continue;

                    float distance = player::get_player_coords().GetDistanceToInMeters(actor->RootComponent->RelativeLocation);

                    float multplr = distance > g_settings.esp.global_render_distance[1] ? 0.f : 6.17757f / distance;

                    if (multplr == 0.f || g_settings.esp.global_render_distance[0] > distance) continue;

                    SDK::FVector2D location;
                    if (!ctrl->ProjectWorldLocationToScreen(actor->RootComponent->RelativeLocation, &location, true)) continue;

                    draw::RGBA red = { 255, 0, 0, 255 };
                    draw::RGBA white = { 255, 255, 255, 255 };
                    draw::RGBA green = { 0, 255, 0, 255 };

                    draw::RGBA* lineColor = (distance < 100.f) ? &green : &red;
                    draw::RGBA* textColor = (distance < 100.f) ? &green : &white;
                    draw::RGBA* boxColor = (distance < 100.f) ? &green : &white;

                    if (g_settings.esp.draw_line)
                        draw::draw_line(width, 0, location.X, location.Y, lineColor, 1.f);
                    if (g_settings.esp.draw_name)
                        draw::draw_stroke_text(location.X, location.Y, textColor, std::format("{} [{:.2f}]m", name, distance).c_str());
                    if (g_settings.esp.draw_box)
                        draw::draw_corner_box(location.X - (62.5f * multplr), location.Y - (175.f * multplr), 80.f, 140.f, 2.f, boxColor);
                }
            }
        }
	}
}