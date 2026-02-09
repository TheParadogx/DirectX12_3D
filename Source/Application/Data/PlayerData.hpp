#pragma once

#include"System/CSV/CsvLoader.hpp"

namespace Engine::System
{
	struct PlayerData
	{
		int id = 0;
		float MaxHP = 0;
		float Attack = 0;
		float MoveSpeed = 0;
		float TurnSpeed = 0.0f;
		float DodgeSpeed = 0;
		int DodgeMax = 0;
		float DodgeCT = 0;

		CSV_BINDABLE(PlayerData, 
			obj.id, obj.MaxHP, obj.Attack, obj.MoveSpeed,
			obj.TurnSpeed, obj.DodgeSpeed, obj.DodgeMax, obj.DodgeCT);

	};
}