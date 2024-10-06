#include "imgui.h"
#include "script.hpp"
#include "submenu.hpp"
#include "settings.hpp"
#include "player_stats.h"

namespace big
{
	void player_stat::render_menu()
	{
        
	}
    void player_stat::render_submenu()
    {
        g_ui_manager->add_submenu<regular_submenu>("Stats", SubmenuStats, [](regular_submenu* sub)
        {

        });
    }
}