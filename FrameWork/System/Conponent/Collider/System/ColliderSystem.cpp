#include "pch.h"
#include "ColliderSystem.hpp"
#include"Math/Matrix/Matrix.h"
#include"System/Collider/Collision/Collision.hpp"

#include"System/Conponent/Transform/TransformConponent.hpp"
#include"System/Conponent/Collider/ColliderComponent.hpp"
#include"System/Conponent/Rigidbody/RigidbodyComponent.hpp"
#include"System/Collider/CollisionEvents/CollisionEvent.hpp"
#include"System/Conponent/Fbx/FbxMeshConponent.hpp"

/// <summary>
/// 衝突コンポーネントのアタッチ
/// </summary>
void Engine::System::ColliderSystem::NotifyCollision(entt::registry& Reg, entt::entity EntityA, entt::entity EntityB)
{
	//	既にあるから追加するだけ
	if (auto* ev = Reg.try_get<CollisionEvent>(EntityA))
	{
		ev->HitEntitys.push_back(EntityB);
	}
	//	まだないから作成して追加する
	else
	{
		Reg.emplace<CollisionEvent>(EntityA, std::vector{ EntityB });
	}
}

/// <summary>
/// 初期化
/// </summary>
void Engine::System::ColliderSystem::Initialize()
{
	RegisterRule<AABBCollider, AABBCollider>();
	RegisterRule<AABBCollider, OBBCollider>();
}

/// <summary>
/// 中心座標の更新
/// </summary>
/// <param name="Registry"></param>

#if true
void Engine::System::ColliderSystem::Update(entt::registry& Registry)
{
	// FbxComponent も含めて View を作成
	auto view = Registry.view<Transform3D, ColliderComponent>();

	view.each([&](entt::entity entity, Transform3D& trans, ColliderComponent& col)
		{
			if (!col.IsCollisiton) return;

			Math::Vector3 finalPos = trans.Position;

			// --- ボーン追従ロジック ---
			bool boneFound = false;
			FbxComponent* fbx = Registry.try_get<FbxComponent>(entity);
			if (fbx != nullptr)
			{
				// ボーン名が指定されており、かつモデルにそのボーンが存在するか確認
				if (!col.BoneName.empty())
				{
					boneFound = true;
				}
			}




			// 各コライダー型への適用
			if (auto* pAABB = col.GetPtr<AABBCollider>())
			{
				//	そのまま代入
				if (boneFound == true)
				{
					finalPos = fbx->Mesh->GetBoneWorldPosition(col.BoneName);
					finalPos.y = trans.Position.y;
				}

				pAABB->SetCenter(finalPos + col.Offset);

			}
			else if (auto* pOBB = col.GetPtr<OBBCollider>())
			{
				//	こっちでは回転などの計算をする
				Math::Matrix rotMat = trans.Rotation.ToMatrix();
				Math::Vector3 rotatedOffset = Math::Vector3::TransformNormal(col.Offset, rotMat);
				finalPos = trans.Position + rotatedOffset;

				pOBB->SetCenter(finalPos);
				pOBB->SetRotation(trans.Rotation);
			}
		});
}

#else
void Engine::System::ColliderSystem::Update(entt::registry& Registry)
{
	auto view = Registry.view<Transform3D, ColliderComponent>();

	view.each([](Transform3D& trans, ColliderComponent& col)
		{
			if (!col.IsCollisiton) return;

			// 1. クォータニオンを回転行列に変換
			// Matrix への変換を trans.Rotation.ToMatrix() で行う
			Math::Matrix rotMat = trans.Rotation.ToMatrix();

			// 2. TransformNormal を使用して Offset を回転させる
			// TransformNormal は行列の平行移動成分を無視してベクトルを変換します
			Math::Vector3 rotatedOffset = Math::Vector3::TransformNormal(col.Offset, rotMat);

			// 3. 最終的な中心座標 = 現在の位置 + 回転したオフセット
			Math::Vector3 finalPos = trans.Position + rotatedOffset;

			if (auto* pAABB = col.GetPtr<AABBCollider>())
			{
				pAABB->SetCenter(trans.Position + col.Offset);
			}
			else if (auto* pOBB = col.GetPtr<OBBCollider>())
			{
				pOBB->SetCenter(finalPos);
				// OBB自体の向きも Transform の回転に合わせる
				pOBB->SetRotation(trans.Rotation);
			}
		});
}
#endif // true

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
		view1.iterate(Registry.storage<Transform3D>());

		if (auto* s1 = Registry.storage(tag1))
		{
			view1.iterate(*s1);
		}

		//	Runtime View 1の作成
		entt::runtime_view view2 = {};
		view2.iterate(Registry.storage<ColliderComponent>());
		view2.iterate(Registry.storage<Transform3D>());

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
					continue;
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
						//	座標系のコンポーネントの取得
						auto& transA = Registry.get<Transform3D>(entityA);
						auto& transB = Registry.get<Transform3D>(entityB);

						//	両方押し戻し
						if (Resolution.PushFirst == true && Resolution.PushSecond)
						{
							transA.Position += OutVec * 0.5f;
							transB.Position -= OutVec * 0.5f;
						}
						//	Aだけ
						else if (Resolution.PushFirst == true)
						{
							transA.Position += OutVec;
						}
						//	Bだけ
						else if (Resolution.PushSecond == true)
						{
							transB.Position -= OutVec;
						}

						//	Firstの当たり判定イベント
						if (Resolution.HitEventFirst == true)
						{
							NotifyCollision(Registry, entityA, entityB);
						}

						// Secondの当たり判定イベント
						if (Resolution.HitEventSecond == true)
						{
							NotifyCollision(Registry, entityB, entityA);
						}
					}

				}

			}
		}
	}
	
}

/// <summary>
/// 更新の最後に衝突イベントを全て削除する
/// </summary>
/// <param name="Registry"></param>
void Engine::System::ColliderSystem::PostUpdate(entt::registry& Registry)
{
	Registry.clear<CollisionEvent>();
}

/// <summary>
/// デバック用の当たり判定の表示
/// </summary>
void Engine::System::ColliderSystem::Debug(entt::registry& Registry)
{
	//auto view = Registry.view<AABBColliderComponent>();
	//view.each([](auto& col)
	//{
	//	col.Collider.DebugRender();
	//});
#if _DEBUG

	auto view = Registry.view<Transform3D, ColliderComponent>();
	view.each([](Transform3D& trans, ColliderComponent& col)
		{
			if (!col.IsCollisiton) return;

			// 型が AABB の場合のみ取得して更新
			if (auto* pAABB = col.GetPtr<AABBCollider>())
			{
				pAABB->DebugRender();
			}
			else if (auto* pOBB = col.GetPtr<OBBCollider>())
			{
				pOBB->DebugRender();
			}
		});

#endif // _DEBUG
}

/// <summary>
/// Guiで全オブジェクト操作
/// </summary>
void Engine::System::ColliderSystem::OnGui(entt::registry& Registry)
{
	auto view = Registry.view<ColliderComponent>();

	// ImGui の中で一意のIDを維持するためのカウンタ
	int i = 0;

	view.each([&](entt::entity entity, ColliderComponent& col)
		{
			ImGui::PushID(static_cast<int>(entity)); // エンティティIDをIDスタックに積む

			// ツリーのラベルにエンティティの番号や型名を出すと分かりやすい
			std::string label = "Entity " + std::to_string(static_cast<uint32_t>(entity)) + ": Collider";

			if (ImGui::TreeNode(label.c_str()))
			{
				// 当たり判定の有効/無効
				ImGui::Checkbox("Is Collision", &col.IsCollisiton);

				// Offset の操作 (今回のメイン)
				// .data() で float* を取得できる前提（Math::Vector3 の実装によります）
				// もし .data() がなければ &col.Offset.x を渡します
				ImGui::DragFloat3("Offset", &col.Offset.x, 0.05f);

				// 型情報の表示（デバッグ用）
				ImGui::Text("Type: %s", col.Type.name());

				// 型ごとの固有パラメータ (Sizeなど) も編集したい場合はここに追加
				/*
				if (auto* pAABB = col.GetPtr<AABBCollider>()) {
					 // ImGui::DragFloat3("Size", ...);
				}
				*/

				ImGui::TreePop();
			}

			ImGui::PopID();
			ImGui::Separator(); // 境界線
		});
}
