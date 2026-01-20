#pragma once
#include"Math/Vector3/Vector3.h"

namespace Engine::System
{
	class SphereCollider;
	class AABBCollider;
	class OBBCollider;

	class Collision
	{
	public:
		/// <summary>
		/// AABB同士の当たり判定
		/// </summary>
		/// <param name="Collider1">オブジェクト１</param>
		/// <param name="Collider2">オブジェクト２</param>
		/// <param name="OutVector">押し戻しの移動ベクトル</param>
		/// <returns>true:接触</returns>
		static bool ComputeAABBCollision(
			const AABBCollider* Collider1,
			const AABBCollider* Collider2,
			Math::Vector3& OutVector);
		
		/// <summary>
		/// 円同士の当たり判定
		/// </summary>
		/// <param name="Collider1">オブジェクト１</param>
		/// <param name="Collider2">オブジェクト２</param>
		/// <param name="OutVector">押し戻しの移動ベクトル</param>
		/// <returns>true:接触</returns>
		static bool ComputeSphepeCollision(
			const SphereCollider* Collider1,
			const SphereCollider* Collider2,
			Math::Vector3& OutVector);


		/// <summary>
		/// AABB同士の当たり判定
		/// </summary>
		/// <param name="Collider1">オブジェクト１</param>
		/// <param name="Collider2">オブジェクト２</param>
		/// <param name="OutVector">押し戻しの移動ベクトル</param>
		/// <returns>true:接触</returns>
		static bool ComputeCollision(
			const AABBCollider* Collider1,
			const AABBCollider* Collider2,
			Math::Vector3& OutVector);

		/// <summary>
		/// 円同士の当たり判定
		/// </summary>
		/// <param name="Collider1">オブジェクト１</param>
		/// <param name="Collider2">オブジェクト２</param>
		/// <param name="OutVector">押し戻しの移動ベクトル</param>
		/// <returns>true:接触</returns>
		static bool ComputeCollision(
			const SphereCollider* Collider1,
			const SphereCollider* Collider2,
			Math::Vector3& OutVector);

		//	OBBとOBBの当たり判定

		/// <summary>
		/// AABBとOBBの当たり判定
		/// </summary>
		/// <param name="aabb"></param>
		/// <param name="obb"></param>
		/// <param name="OutVector">押し戻しのベクトル</param>
		/// <returns>ture:接触</returns>
		static bool ComputeCollision(
			const AABBCollider* aabb,
			const OBBCollider* obb,
			Math::Vector3& OutVector);

		//	AABBと円の当たり判定

	private:

	};
}
