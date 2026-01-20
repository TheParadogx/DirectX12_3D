#include "pch.h"
#include "SystemManager.hpp"

#include "System/Conponent/Fbx/System/FbxRenderSystem.h"
#include "System/Conponent/Collider/System/ColliderSystem.hpp"
#include"System/Conponent/Rigidbody/System/RigidbodySystem.hpp"

namespace Engine::System
{
	/// <summary>
	/// 可変更新
	/// </summary>
	/// <param name="Reg">ecs管理</param>
	/// <param name="DeltaTime">デルタタイム</param>
	void SystemManager::PreUpdate(entt::registry& Reg, double DeltaTime)
	{
		//	ユーザー定義
		for (const auto& System : mSystems)
		{
			System->MainUpdate(Reg, DeltaTime);
		}

		//	Fbxアニメーション
		System::FbxRenderSystem::Update(Reg, DeltaTime);
	}
	
	/// <summary>
	/// 固定更新
	/// </summary>
	/// <param name="Reg">ecs管理</param>
	/// <param name="DeltaTime">デルタタイム</param>
	void SystemManager::MainUpdate(entt::registry& Reg, double DeltaTime)
	{
		//	ユーザー定義システム
		for (const auto& System : mSystems)
		{
			System->PreUpdate(Reg, DeltaTime);
		}

		//	移動
		System::RigidBodySystem::MainUpdate(Reg, DeltaTime);

		//	当たり判定位置の更新
		System::ColliderSystem::Update(Reg);
		//	当たり判定
		System::ColliderSystem::CheckCollition(Reg);

	}

	/// <summary>
	/// 事後更新
	/// </summary>
	/// <param name="Reg"></param>
	/// <param name="DeltaTime"></param>
	void SystemManager::PostUpdate(entt::registry& Reg, double DeltaTime)
	{
		//	ユーザー定義
		for (const auto& System : mSystems)
		{
			System->PostUpdate(Reg, DeltaTime);
		}

		//	事後更新
		System::ColliderSystem::OnGui(Reg);

		System::ColliderSystem::PostUpdate(Reg);
	}


	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="Reg"></param>
	/// <param name="Alpha"></param>
	void SystemManager::Render(entt::registry& Reg)
	{
		//	FBXの描画
		System::FbxRenderSystem::Render(Reg);
		System::ColliderSystem::Debug(Reg);

		//	描画
		for (const auto& System : mSystems)
		{
			System->Render(Reg);
		}
	}
}