#include "imgui.h"
#include "script.hpp"
#include "submenu.hpp"
#include "settings.hpp"
#include "player_menu.h"
#include "utility/unreal_engine_utility.hpp"

namespace big
{
    void player_menu::render_submenu()
    {
        g_ui_manager->add_submenu<regular_submenu>("Player", SubmenuPlayer, [](regular_submenu* sub)
        {
            sub->add_option<reguler_option>("Get Entity List", nullptr, [] {
                unreal_engine::get_entity();
            });
        });
    }
}