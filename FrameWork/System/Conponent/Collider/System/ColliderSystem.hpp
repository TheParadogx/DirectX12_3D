#pragma once
#include<Plugin/entt/entt.hpp>
#include<utility>
#include<typeindex>
#include<map>
#include"Math/Vector3/Vector3.h"
#include"System/Collider/Collision/Collision.hpp"

namespace Engine::System
{
	/// <summary>
	/// 当たり判定のタグごとの設定
	/// </summary>
	struct CollisionResolution
	{
		bool PushFirst = true;
		bool PushSecond = true;
		bool HitEventFirst = false;
		bool HitEventSecond = false;
	};

	//	当たり判定用の関数のポインタの型情報
	using CollisionFunc = bool(*)(const void*, const void*,Math::Vector3&);

	/// <summary>
	/// 衝突判定と
	/// </summary>
	class ColliderSystem
	{
		/// <summary>
		/// 衝突コンポーネントのアタッチ
		/// </summary>
		static void NotifyCollision(entt::registry& Reg, entt::entity EntityA, entt::entity EntityB);
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		static void Initialize();

		/// <summary>
		/// 中心座標の更新
		/// </summary>
		/// <param name="Registry"></param>
		static void Update(entt::registry& Registry);

		/// <summary>
		/// 当たり判定
		/// </summary>
		static void CheckCollition(entt::registry& Registry);

		/// <summary>
		/// 更新の最後に衝突イベントを全て削除する
		/// </summary>
		/// <param name="Registry"></param>
		static void PostUpdate(entt::registry& Registry);

		/// <summary>
		/// デバック用の当たり判定の表示
		/// </summary>
		static void Debug(entt::registry& Registry);

		/// <summary>
		/// 当たり判定をする組み合わせの追加
		/// </summary>
		/// <param name="Tag1"></param>
		/// <param name="Tag2"></param>
		template<typename T1, typename T2>
		static void AddCollisionPair(const CollisionResolution& Resolution);

		/// <summary>
		/// 当たり判定の型情報のセットとメソッドの追加
		/// </summary>
		/// <typeparam name="C1">型情報1</typeparam>
		/// <typeparam name="C2"></typeparam>
		template<typename C1, typename C2>
		static void RegisterRule();

		/// <summary>
		/// Guiで全オブジェクト操作
		/// </summary>
		static void OnGui(entt::registry& Registry);

	private:
		/// <summary>
		/// タグの組み合わせと設定のコレクション
		/// </summary>
		static inline std::map<std::pair<entt::id_type, entt::id_type>, CollisionResolution> sCollisionResolutions;

		//	K:形状＋形状　V:対応した判定メソッド
		static inline std::map<std::pair<std::type_index, std::type_index>, CollisionFunc> sCheckCollision;
	};

	/// <summary>
	/// 当たり判定の型情報のセットとメソッドの追加
	/// </summary>
	/// <typeparam name="C1">型情報1</typeparam>
	/// <typeparam name="C2"></typeparam>
	template<typename T1, typename T2>
	inline void ColliderSystem::AddCollisionPair(const CollisionResolution& Resolution)
	{
		entt::id_type id1 = entt::type_id<T1>().hash();
		entt::id_type id2 = entt::type_id<T2>().hash();
		sCollisionResolutions[{id1, id2}] = Resolution;
	}

	/// <summary>
	/// 当たり判定の型情報とそれに対応した処理メソッドのテーブル作成
	/// </summary>
	/// <typeparam name="C1"></typeparam>
	/// <typeparam name="C2"></typeparam>
	template<typename C1, typename C2>
	inline void ColliderSystem::RegisterRule()
	{
		sCheckCollision[{typeid(C1), typeid(C2)}] = [](const void* raw1, const void* raw2, Math::Vector3& outVec)->bool
			{
				return Collision::ComputeCollision(static_cast<const C1*>(raw1), static_cast<const C2*>(raw2), outVec);
			};

		if constexpr (!std::is_same_v<C1, C2>)
		{
			sCheckCollision[{typeid(C2), typeid(C1)}] = [](const void* raw1, const void* raw2, Math::Vector3& outVec)->bool
				{
					return Collision::ComputeCollision(static_cast<const C1*>(raw2),static_cast<const C2*>(raw1), outVec);
				};
		}
	}
}

