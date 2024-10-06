#pragma once
#include <pointers.hpp>

namespace big::unreal_engine
{
	inline SDK::ULocalPlayer* get_local_player()
	{
		if (auto engine = g_pointers->m_engine)
		{
			if (auto instance = engine->GameInstance)
			{
				if (auto local_player = instance->LocalPlayers[0])
				{
					return local_player;
				}
			}
		}

		return nullptr;
	}

	inline SDK::APlayerController* get_player_controller()
	{
		if (auto engine = g_pointers->m_engine)
		{
			if (auto ins = engine->GameInstance)
			{
				if (auto LocalPlayer = ins->LocalPlayers[0])
				{
					if (auto plyr_ctrl = LocalPlayer->PlayerController)
					{
						return plyr_ctrl;
					}
				}
			}
		}

		return nullptr;
	}

	inline SDK::ACharacter* get_character()
	{
		if (auto player = unreal_engine::get_local_player())
		{
			if (auto pcontroller = player->PlayerController)
			{
				if (auto character = pcontroller->Character)
				{
					return character;
				}
			}
		}

		return nullptr;
	}

	inline SDK::USceneComponent* get_root_component()
	{
		if (auto player = unreal_engine::get_local_player())
		{
			if (auto pcontroller = player->PlayerController)
			{
				if (auto character = pcontroller->Character)
				{
					return character->RootComponent;
				}
			}
		}

		return nullptr;
	}

	inline void get_entity()
	{
		if (auto world = *g_pointers->m_world)
		{
			for (auto level : world->Levels)
			{
				if (!level)
					continue;

				for (auto actors : level->Actors)
				{
					if (!actors) continue;

					LOG(INFO) << "Actor name " << actors->GetName();

					if (auto root = actors->RootComponent)
					{

					}
				}
			}
		}
	}

	inline bool is_key_pressed(std::uint16_t key)
	{
		if (GetForegroundWindow() == g_pointers->m_hwnd)
		{
			if (GetAsyncKeyState(key) & 0x8000)
			{
				return true;
			}
		}

		return false;
	}

	inline bool is_controller_pressed(std::uint16_t button)
	{
		XINPUT_STATE state;
		// Zero out the state structure
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		// Get the state of the controller (controller 0)
		if (XInputGetState(0, &state) == ERROR_SUCCESS)
		{
			// Check if the specific button is pressed
			return (state.Gamepad.wButtons & button) != 0;
		}

		// Controller is not connected
		return false;
	}
}