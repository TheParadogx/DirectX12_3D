#pragma once

namespace Engine::System
{
	//	アクターの種別
	struct PlayerTag {};
	struct EnemyTag {};
	struct BasicTag{};
	struct AdvancedTag{};
	struct BossTag{};
	struct StageTag {};
	struct PlayerWeaponTag{};
	struct EnemyWeaponTag {};
	struct DeadTag{};

	//	カテゴリ
	struct InGameTag{};
}