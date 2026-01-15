#pragma once
#include"System/Entity/System/Base/ISystem.hpp"

namespace Engine::System
{
	class PlayerStateSystem : public ISystem
	{
		/// <summary>
		/// 走り状態に移行できるかどうかの判定フラグ
		/// </summary>
		/// <returns></returns>
		bool CheckRunRequest();
	public:
		/// <summary>
		/// 前回情報の保持
		/// </summary>
		virtual void PreUpdate(entt::registry& Reg, double DeltaTime)override;

		/// <summary>
		/// 状態の遷移
		/// </summary>
		virtual void MainUpdate(entt::registry& Reg, double DeltaTime)override;

		/// <summary>
		/// 今の状態からアニメーションの設定と更新
		/// </summary>
		virtual void PostUpdate(entt::registry& Reg, double DeltaTime) override;

	};
}