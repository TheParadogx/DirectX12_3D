#include "pch.h"
#include "ColliderSystem.hpp"

#include"System/Collider/Collision/Collision.hpp"

#include"System/Conponent/Transform/TransformConponent.hpp"
#include"System/Conponent/Collider/ColliderComponent.hpp"
#include"System/Conponent/Rigidbody/RigidbodyComponent.hpp"

/// <summary>
/// 初期化
/// </summary>
void Engine::System::ColliderSystem::Initialize()
{
	RegisterRule<AABBCollider, AABBCollider>();
}

/// <summary>
/// 中心座標の更新
/// </summary>
/// <param name="Registry"></param>
void Engine::System::ColliderSystem::Update(entt::registry& Registry)
{
	//	四角形の中心座標
	auto view = Registry.view<Transform3D, AABBColliderComponent>();
	view.each([](auto& trans, auto& col)
	{
		if (col.IsCollision == false)
		{
			return;
		}

		//	足元＋浮かせたい分
		col.Collider.SetCenter(trans.Position + col.Offset);
	});
}

void Engine::System::ColliderSystem::CheckAABBCollition(entt::registry& Registry)
{
	// AABBを持つオブジェクトのみを摘出
	auto view = Registry.view<AABBColliderComponent>();

	//	IDの取得
	std::vector<entt::entity> entities(view.begin(), view.end());
	size_t size = entities.size();

	if (size < 2)
	{
		return;
	}

	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = i + 1; j < size; j++)
		{
			//	取得
			entt::entity e1 = entities[i];
			entt::entity e2 = entities[j];
			auto& col1 = view.get<AABBColliderComponent>(e1);
			auto& col2 = view.get<AABBColliderComponent>(e2);

			//	当たり判定を行うかどうか
			if (col1.IsCollision == false || col2.IsCollision == false)
			{
				continue;
			}

			auto* rb1 = Registry.try_get<Rigidbody3D>(e1);
			auto* rb2 = Registry.try_get<Rigidbody3D>(e2);

			//	押し戻し処理をするかどうか
			bool canMove1 = rb1!= nullptr && rb1->IsDynamic();
			bool canMove2 = rb2!= nullptr && rb2->IsDynamic();

			//	両方動かないなら衝突判定は不要
			if (canMove1 == false && canMove2 == false)
			{
				continue;
			}

			Math::Vector3 pushVec = {};
			if (Collision::ComputeAABBCollision(&col1.Collider, &col2.Collider, pushVec))
			{
				//	座標系
				auto* trans1 = Registry.try_get<Transform3D>(e1);
				auto* trans2 = Registry.try_get<Transform3D>(e2);

				//	両方
				if (canMove1 == true && canMove2 == true)
				{
					//	質量を使うほうがいいのですが今回はとりあえず半分にします
					if (trans1 != nullptr)
					{
						trans1->Position += pushVec * 0.5f;
						col1.Collider.SetCenter(trans1->Position + col1.Offset);
					}
					if (trans2 != nullptr)
					{
						trans2->Position -= pushVec * 0.5f;
						col2.Collider.SetCenter(trans2->Position + col2.Offset);
					}

				}
				//	1だけ
				else if (canMove1 == true)
				{
					if (trans1 != nullptr)
					{
						trans1->Position += pushVec;
						col1.Collider.SetCenter(trans1->Position + col1.Offset);
					}
				}
				// 2だけ
				else if (canMove2 == true)
				{
					if (trans2 != nullptr)
					{
						trans2->Position -= pushVec;
						col2.Collider.SetCenter(trans2->Position + col2.Offset);
					}
				}

				//	衝突通知

			}

		}
	}

}

////	AABBの当たり判定
//CheckAABBCollition(Registry);

/// <summary>
/// 当たり判定
/// </summary>
void Engine::System::ColliderSystem::CheckCollition(entt::registry& Registry)
{
	//	当たり判定をするタグのコレクションでループ
	for (auto& [tags,Resolution]: sCollisionResolutions)
	{
		//	タグの取得
		entt::id_type tag1 = tags.first;
		entt::id_type tag2 = tags.second;

		//	Runtime View 1の作成
		entt::runtime_view view1 = {};
		view1.iterate(Registry.storage<ColliderComponent>());

		if (auto* s1 = Registry.storage(tag1))
		{
			view1.iterate(*s1);
		}

		//	Runtime View 1の作成
		entt::runtime_view view2 = {};
		view2.iterate(Registry.storage<ColliderComponent>());

		if (auto* s2 = Registry.storage(tag2))
		{
			view2.iterate(*s2);
		}

		//	タグ１
		for (auto entityA : view1)
		{
			//	タグ２
			for (auto entityB : view2)
			{
				//	同一オブジェクトかどうかの判定
				if (entityA == entityB)
				{
					return;
				}

				//	コンポーネントの取得
				auto& colA = Registry.get<ColliderComponent>(entityA);
				auto& colB = Registry.get<ColliderComponent>(entityB);

				//	衝突フラグ判定
				if (colA.IsCollisiton == false || colB.IsCollisiton == false)
				{
					continue;
				}

				//	衝突判定をする型のテーブル
				auto it = sCheckCollision.find({ colA.Type,colB.Type });
				if (it != sCheckCollision.end())
				{
					Math::Vector3 OutVec = {};
					if (it->second(colA.GetRaw(), colB.GetRaw(), OutVec))
					{
						//	状突している
						auto a = 0;
					}

				}

			}
		}
	}
	//	当たり判定のテーブルに存在する組み合わせ化を見る
	//	あるなら当たり判定

	//	当たっていたら押し戻しをCollisionResolutionの情報から見て押し戻し処理をする
	
}

/// <summary>
/// デバック用の当たり判定の表示
/// </summary>
void Engine::System::ColliderSystem::Debug(entt::registry& Registry)
{
	auto view = Registry.view<AABBColliderComponent>();
	view.each([](auto& col)
	{
		col.Collider.DebugRender();
	});
}
