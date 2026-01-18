#pragma once
#include"System/Entity/System/Base/ISystem.hpp"
#include"Application/Components/Player/PlayerState/PlayerStateComponent.hpp"
#include"Application/Components/Player/Input/InputRequestComponent.hpp"
#include"System/Conponent/Fbx/FbxMeshConponent.hpp"


namespace Engine::System
{
	class PlayerStateSystem : public ISystem
	{
		/// <summary>
		/// 走り状態に移行できるかどうかの判定フラグ
		/// </summary>
		/// <returns></returns>
		bool CheckRunRequest(PlayerStateComponent& state);

		/// <summary>
		/// 攻撃状態に移行できるかどうかの判定フラグ
		/// </summary>
		/// <returns></returns>
		bool CheckAttackRequest(PlayerStateComponent& state);

		/// <summary>
		/// 攻撃の終了条件の判定
		/// </summary>
		/// <returns>true:終了</returns>
		bool IsFinishAttack(PlayerStateComponent& State, InputRequestComponent& Req, FbxComponent& Fbx);
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