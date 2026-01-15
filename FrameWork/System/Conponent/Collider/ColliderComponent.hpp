#pragma once
#include"System/Collider/AABB/AABBCollider.hpp"
#include"System/Collider/Sphere/SphereCollider.hpp"
#include"System/Collider/OBB/OBBCollider.hpp"


namespace Engine::System
{
	//	回転のしない矩形の当たり判定
	struct AABBColliderComponent
	{
		//	足元からどれくらいずらすか
		Math::Vector3 Offset = { 0.0f, 0.0f, 0.0f };
		//	当たり判定オブジェクト
		AABBCollider Collider;
		//	true:押し戻しをしない false:押し戻しをする
		bool IsStatic = false;
		//	当たり判定を行うかどうか
		bool IsCollision = true;
	};

}