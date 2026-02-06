#pragma once
#include"System/Entity/System/Base/ISystem.hpp"

#include "Application/Components/Enemy/AI/EnemyAIComponent.hpp"
#include "Application/Components/Enemy/Parameter/EnemyParametersComponent.hpp"
#include "System/Conponent/Transform/TransformConponent.hpp"
#include "System/Conponent/Rigidbody/RigidbodyComponent.hpp"
#include "System/Conponent/Fbx/FbxMeshConponent.hpp"

#include "Math/Vector3/Vector3.h"


namespace Engine::System
{
	class EnemyAISystem : public ISystem
	{
		//	攻撃状態に遷移するかどうかの判定
		bool CanAttackState(EnemyAIComponent& AI, EnemyParameters& param, FbxComponent& Fbx,float Distance);

		//	追跡状態に遷移するかどうかの判定

		//	待機状態に遷移するかどうかの判定

		//	回避状態に遷移するかどうかの判定

		/// <summary>
		/// 次の状態を決定するメソッド
		/// </summary>
		/// <returns></returns>
		eEnemyState DetermineNextRequest(entt::registry& Reg,EnemyAIComponent& AI, EnemyParameters& Param,FbxComponent& Fbx,float Distance);

	private:

		/// <summary>
		/// 追跡状態での状態更新
		/// </summary>
		/// <param name="ToPlayer">プレイヤーとの移動ベクトル</param>
		void UpdateChasing(EnemyAIComponent& AI, EnemyParameters& Param, Transform3D& Transform, Rigidbody3D& Rigidbody, const Math::Vector3& ToPlayer,float Distance);

		/// <summary>
		/// 攻撃状態での状態更新
		/// </summary>
		void UpdateAttacking(EnemyAIComponent& AI, EnemyParameters& Param, Transform3D& Transform, const Math::Vector3& ToPlayer, float Distance);

		/// <summary>
		/// 待機状態での状態更新
		/// </summary>
		void UpdateIdle(EnemyAIComponent& AI, EnemyParameters& Param,float DeltaTime);

	private:
		/// <summary>
		///	状態の変更
		/// </summary>
		/// <param name="AI"></param>
		/// <param name="Param"></param>
		/// <param name="fbx"></param>
		void ChangeState(entt::registry& Reg,EnemyAIComponent& AI, EnemyParameters& Param,FbxComponent& fbx,eEnemyState Next);

		/// <summary>
		/// 今の状態の終了（後片付け）
		/// </summary>
		void ExitState(entt::registry& Reg,EnemyAIComponent& AI, EnemyParameters& Param, FbxComponent& fbx);

		/// <summary>
		/// 状態初期化
		/// </summary>
		void InitState(entt::registry& Reg,EnemyAIComponent& AI, EnemyParameters& Param,FbxComponent& fbx);

	public:
		/// <summary>
		/// 状態の終了判定
		/// </summary>
		/// <param name="Reg"></param>
		/// <param name="DeltaTime"></param>
		virtual void PreUpdate(entt::registry& Reg, double DeltaTime)override;
		/// <summary>
		/// 状態遷移
		/// </summary>
		/// <param name="Reg"></param>
		/// <param name="DeltaTime"></param>
		virtual void MainUpdate(entt::registry& Reg, double DeltaTime)override;
		/// <summary>
		/// デバック描画
		/// </summary>
		/// <param name="Reg"></param>
		virtual void Render(entt::registry& Reg) override;

	};
}


