#include "pch.h"
#include "SocketComponentSystem.hpp"
#include "Math/Matrix3x3/Matrix3x3.h"
#include"System/Conponent/Transform/TransformConponent.hpp"
#include"Application/Components/Socket/SocketComponent.hpp"
#include"System/Conponent/Fbx/FbxMeshConponent.hpp"
#include"Graphics/Renderer/Renderer.hpp"

#include"System/Camera/Camera.hpp"

#include<Plugin/ImGui/ImGuizmo.h>
#include<DirectXMath.h>

/// <summary>
/// 座標位置が確定した親オブジェクトの指定したボーン座標にセット
/// </summary>
void Engine::System::SocketComponentSystem::PostUpdate(entt::registry& Reg, double DeltaTime)
{
    OnGui(Reg);

}

void Engine::System::SocketComponentSystem::Render(entt::registry& Reg)
{

    auto view = Reg.view<Transform3D, SocketComponent>();
    view.each([&](auto entity, auto& trans, auto& socket)
        {
            //	有効な親か判定
            if (Reg.valid(socket.Parent) == false)
            {
                return;
            }

            //	親のfbxコンポーネント取得
            //	なければ終了
            auto* parentFbx = Reg.try_get<FbxComponent>(socket.Parent);
            if (parentFbx == nullptr || parentFbx->Mesh == nullptr)
            {
                return;
            }


            // クォータニオンの正規化（巨大化防止）
                    // 計算に使う前に、必ず長さを1に揃えます
            auto NormalizeQuat = [](Math::Quaternion& q) {
                float lenSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
                if (lenSq > 0.0f) {
                    float len = sqrtf(lenSq);
                    q.x /= len; q.y /= len; q.z /= len; q.w /= len;
                }
                };
            NormalizeQuat(socket.OffsetRot);

            // ボーンの姿勢取得
            Math::Vector3 bonePos = parentFbx->Mesh->GetBoneWorldPosition(socket.BoneName);
            Math::Quaternion boneRot = parentFbx->Mesh->GetBoneWorldRotation(socket.BoneName);
            NormalizeQuat(boneRot); // 親ボーンのデータも念のため正規化

            // 最終的な回転
            Math::Quaternion finalRot =socket.OffsetRot * boneRot;
            NormalizeQuat(finalRot);

            // ベクトル回転関数の安全化
            auto RotateVec = [](const Math::Vector3& v, const Math::Quaternion& q) {
                Math::Vector3 qV(q.x, q.y, q.z);
                // 外積を使った回転
                Math::Vector3 t = Math::Vector3::Cross(qV, v) * 2.0f;
                return v + (t * q.w) + Math::Vector3::Cross(qV, t);
                };

            // --- 座標計算 ---

            // ソケット（持ち手）のワールド座標
            Math::Vector3 worldSocketPos = bonePos + RotateVec(socket.OffsetPos, boneRot);

            // 刀の原点位置の逆算
            Math::Vector3 rotatedPivot = RotateVec(socket.PivotOffset, finalRot);

            trans.Position = worldSocketPos - rotatedPivot;
            trans.Rotation = finalRot;

            // デバッグ表示
            Graphics::Renderer::GetInstance()->DrawGizmo(worldSocketPos, boneRot, 1.0f);
            Graphics::Renderer::GetInstance()->DrawGizmo(trans.Position, trans.Rotation, 0.2f);

        });

}

/// <summary>
/// ImGuiでオフセットをいじるための
/// </summary>
/// <param name="Reg"></param>
void Engine::System::SocketComponentSystem::OnGui(entt::registry& Reg)
{

    // 調整用のウィンドウを作成
    if (ImGui::Begin("Socket Offset Tuner"))
    {
        auto view = Reg.view<SocketComponent>();
        view.each([&](auto entity, auto& socket)
            {
                // エンティティごとにIDを分ける（同じコンポーネントが複数あっても誤作動しないように）
                ImGui::PushID((int)entity);

                ImGui::Text("Target Bone: %s", socket.BoneName.c_str());

                // 1. 位置オフセットの調整 (0.01f単位でドラッグ可能)
                if (ImGui::DragFloat3("Pos Offset", &socket.OffsetPos.x, 0.01f))
                {
                    // 値が変わった時の処理が必要ならここに書く
                }

                // 2. 回転オフセットの調整
                // 本来はクォータニオンですが、調整はオイラー角（度数法）が楽なので変換して出すのも手です。
                // ここでは一旦、直接 4要素をいじれるようにします。
                ImGui::DragFloat4("Rot Offset (Quat)", &socket.OffsetRot.x, 0.005f);

                // 3. ピボット（重心）オフセットの調整
                ImGui::DragFloat3("Pivot Offset", &socket.PivotOffset.x, 0.01f);

                // リセットボタン（必要であれば）
                if (ImGui::Button("Reset All"))
                {
                    socket.OffsetPos = { 0, 0, 0 };
                    socket.OffsetRot = { 0, 0, 0, 1 };
                    socket.PivotOffset = { 0, 0, 0 };
                }

                ImGui::Separator(); // 区切り線
                ImGui::PopID();
            });
    }
    ImGui::End();


}
