#pragma once
#include "Application/Objects/EnemyRank.hpp"
namespace Engine::System
{
	struct InteractableComponent
	{
		float InteractRange = 10.0f;	//	インタラクトできる距離
		EnemyRank Rank = EnemyRank::Basic;	//	どの状態に遷移するか
		bool Talkable = true;			//	離せる状態かどうか
	};

}