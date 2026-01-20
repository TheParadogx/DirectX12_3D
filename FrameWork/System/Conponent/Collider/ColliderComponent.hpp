#pragma once
#include"System/Collider/AABB/AABBCollider.hpp"
#include"System/Collider/Sphere/SphereCollider.hpp"
#include"System/Collider/OBB/OBBCollider.hpp"

#include<typeindex>
#include<memory>
#include<map>
#include<functional>

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

	/// <summary>
	/// 当たり判定用コンポーネント
	/// </summary>
	struct ColliderComponent
	{
		ColliderComponent(std::type_index Type)
			:Type(Type)
		{
		}

		template<typename T>
		static ColliderComponent Create()
		{
			//	インスタンス
			ColliderComponent comp(typeid(T));
			comp.Collider = std::make_shared<T>();
			return comp;
		}

		template<typename T>
		T* GetPtr() const
		{
			if (Type != typeid(T))
			{
				return nullptr;
			}

			return static_cast<T*>(Collider.get());
		}

		const void* GetRaw() const
		{
			return Collider.get();
		}


		//　当たり判定クラスのポインタ
		std::shared_ptr<void> Collider = nullptr;

		//	足元からどれくらいずらすか
		Math::Vector3 Offset = { 0.0f, 0.0f, 0.0f };

		//	型情報
		std::type_index Type;

		//	true:当たり判定をする
		bool IsCollisiton = true;
	};

}