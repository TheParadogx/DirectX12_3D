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
		struct ChaseData
		{
			float DetectionRange = 100.0f;	// õ“G”ÍˆÍ
			float AttackRange = 10.0f;		// UŒ‚”ÍˆÍ
			float MoveSpeed = 10.0f;			// ˆÚ“®‘¬“x
			float RotationSpeed = 5.0f;		// ƒvƒŒƒCƒ„[‚ÉŒü‚­‰ñ“]‘¬“x
		};

		//	ó‘Ô
		eEnemyState State = eEnemyState::Idle;

		//	’ÇÕ—p‚Ìƒf[ƒ^
		ChaseData Chase;

		//	•Ší
		entt::entity Weapon = entt::null;
	};
}