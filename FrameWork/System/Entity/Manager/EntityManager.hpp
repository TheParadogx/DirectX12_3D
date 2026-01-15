#pragma once

#include"System/Singleton/Singleton.hpp"
#include<Plugin/entt/entt.hpp>


namespace Engine::System
{
	/// <summary>
	/// エンティティの生成、破棄などの管理
	/// </summary>
	class EntityManager : public Singleton<EntityManager>
	{
		GENERATE_SINGLETON_BODY(EntityManager);

	public:

		/// <summary>
		/// 初期化
		/// </summary>
		virtual void OnCreate()override;

		/// <summary>
		/// registryへの直接アクセス（Viewを作成できるようにするため）
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] entt::registry& GetRegistry();

		/// <summary>
		/// エンティティの生成
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] entt::entity CreateEntity();

		/// <summary>
		///	永続エンティティの作成 
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] entt::entity CreatePersistentEntity();

		/// <summary>
		/// ローカルオブジェクト（Scene単位）のクリア
		/// </summary>
		void ClearLocalEntities();

		/// <summary>
		/// すべて削除
		/// </summary>
		void AllClear();


	private:
		/// <summary>
		/// オブジェクト、コンポーネント、システムの管理
		/// </summary>
		entt::registry mRegistry;

	};
}

