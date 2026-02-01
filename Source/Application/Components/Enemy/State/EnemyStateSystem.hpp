#pragma once
#include"System/Entity/System/Base/ISystem.hpp"
#include"System/Conponent/Fbx/FbxMeshConponent.hpp"
#include "Application/Components/Enemy/State/EnemyStateComponent.hpp"


namespace Engine::System
{
	class EnemyStateSystem : public ISystem
	{
	public:

		/// <summary>
		/// 状態の終了の判定
		/// </summary>
		/// <param name="Reg"></param>
		/// <param name="DeltaTime"></param>
		void PreUpdate(entt::registry& Reg, double DeltaTime)override;

		/// <summary>
		/// 状態の遷移
		/// </summary>
		/// <param name="Reg"></param>
		/// <param name="DeltaTime"></param>
		void MainUpdate(entt::registry& Reg, double DeltaTime)override;

		/// <summary>
		/// デバックの描画
		/// </summary>
		/// <param name="Reg"></param>
		void Render(entt::registry& Reg)override;

	};

}

