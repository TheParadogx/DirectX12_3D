#pragma once
#include<entt/entt.hpp>

namespace Engine::System
{
	//	“G‚Ìó‘Ô
	enum class eEnemyState
	{
		Idle,	//	‘Ò‹@
		Chase,	//	’ÇÕ
		Attack,	//	UŒ‚
		Dead,	//	€–S
	};
	
	/// <summary>
	/// “G‚Ìó‘ÔŠÇ—
	/// </summary>
	struct EnemyStateComponent
	{
		//	ó‘Ô
		eEnemyState State = eEnemyState::Idle;

		//	•Ší
		entt::entity Weapon = entt::null;
	};
}