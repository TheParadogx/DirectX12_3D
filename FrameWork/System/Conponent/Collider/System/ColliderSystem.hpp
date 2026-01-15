#pragma once
#include<Plugin/entt/entt.hpp>

namespace Engine::System
{
	class ColliderSystem
	{
	public:
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
		/// デバック用の当たり判定の表示
		/// </summary>
		static void Debug(entt::registry& Registry);

	private:
		//	 AABB
		static void CheckAABBCollition(entt::registry& Registry);
	};
}

