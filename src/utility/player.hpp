#pragma once
#include "unreal_engine_utility.hpp"

namespace big::player
{
	inline SDK::FVector get_player_coords()
	{
		if (auto character = unreal_engine::get_character())
		{
			if (auto capsule = character->CapsuleComponent)
			{
				return capsule->RelativeLocation;
			}
		}

		return SDK::FVector(0.f, 0.f, 0.f);
	}

	inline SDK::FRotator get_player_rotation()
	{
		if (auto character = unreal_engine::get_character())
		{
			if (auto capsule = character->CapsuleComponent)
			{
				return capsule->RelativeRotation;
			}
		}

		return SDK::FRotator(0.f, 0.f, 0.f);
	}

	inline SDK::FVector get_player_forward()
	{
		if (auto controller = unreal_engine::get_player_controller())
		{
			return controller->GetActorForwardVector();
		}

		return { 0, 0, 0 };
	}

	inline void no_clip(bool activate)
	{
		static bool bActive = false;
		if (activate)
		{
			bActive = true;

			auto root = unreal_engine::get_root_component();
			if (auto self = unreal_engine::get_character())
			{
				if (unreal_engine::is_key_pressed(VK_LSHIFT))
				{
					auto pos = get_player_coords();
					auto rot = get_player_rotation();
					auto forward = root->GetForwardVector();

					pos.Z += 20.f;

					self->K2_KuroTeleportTo(pos, rot);
				}
				else if (unreal_engine::is_key_pressed(VK_LCONTROL))
				{
					auto pos = get_player_coords();
					auto rot = get_player_rotation();
					auto forward = root->GetForwardVector();

					pos.Z -= 20.f;

					self->K2_KuroTeleportTo(pos, rot);
				}

				if (auto chars = unreal_engine::get_character())
				{
					chars->bActorEnableCollision &= ~(0xA);
					if (auto movement = chars->CharacterMovement)
					{
						movement->AirControl += 100.f;
						movement->MaxFlySpeed += 100.f;
						movement->MaxWalkSpeed += 100.f;
						movement->MaxSwimSpeed += 100.f;
						movement->MaxAcceleration += 100.f;
						movement->MaxWalkSpeedCrouched += 100.f;
						movement->MaxCustomMovementSpeed += 100.f;
						movement->MovementMode = SDK::EMovementMode::MOVE_Flying;
					}
				}
			}
		}
		else
		{
			if (bActive)
			{
				if (auto chars = unreal_engine::get_character())
				{
					chars->bActorEnableCollision |= 0xA;
					if (auto movement = chars->CharacterMovement)
					{
						movement->AirControl -= 100.f;
						movement->MaxFlySpeed -= 100.f;
						movement->MaxWalkSpeed -= 100.f;
						movement->MaxSwimSpeed -= 100.f;
						movement->MaxAcceleration -= 100.f;
						movement->MaxWalkSpeedCrouched -= 100.f;
						movement->MaxCustomMovementSpeed -= 100.f;
						movement->MovementMode = SDK::EMovementMode::MOVE_Walking;
					}
				}

				bActive = false;
			}
		}
	}
}