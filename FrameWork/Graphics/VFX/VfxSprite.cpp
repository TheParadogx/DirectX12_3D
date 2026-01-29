#include "pch.h"
#include "VfxSprite.hpp"

#include"Graphics/Texture/Texture.hpp"
#include"Graphics/Renderer/Renderer.hpp"
#include "System/Camera/Camera.hpp"
#include"Graphics/DirectX/DirectX12.hpp"

namespace Engine::Graphics
{
	VfxSprite::VfxSprite()
        : mPosition(0, 0, 0)
        , mScale(1, 1, 1)
        , mAngle(0, 0, 0)
        , mBaseColor(Color::White())
        , mUVOffset(0, 0)
        , mScrollSpeed(0, 0)
        , mIntensity(1.0f)
	{

	}

    bool VfxSprite::Create(Texture* texture)
    {
        if (!texture) return false;
        mTexture = texture;

        float hW = texture->GetWidth() * 0.5f;
        float hH = texture->GetHeight() * 0.5f;

        // std::array への頂点データ設定
        mVertices[0] = { {-hW,  hH, 0.0f}, {0.0f, 0.0f}, Color::White() };
        mVertices[1] = { {-hW, -hH, 0.0f}, {0.0f, 1.0f}, Color::White() };
        mVertices[2] = { { hW,  hH, 0.0f}, {1.0f, 0.0f}, Color::White() };
        mVertices[3] = { { hW, -hH, 0.0f}, {1.0f, 1.0f}, Color::White() };

        return true;
    }
    void VfxSprite::Update(float deltaTime)
    {
        // UVスクロール加算
        mUVOffset.x += mScrollSpeed.x * deltaTime;
        mUVOffset.y += mScrollSpeed.y * deltaTime;
    }
    void VfxSprite::Render()
    {
        using namespace System;

        if (Camera::Main == nullptr) return;
        //  カメラ情報の取得
        const Math::Matrix& view = Camera::Main->GetView();
        const Math::Matrix& proj = Camera::Main->GetProjection();

        // パイプライン設定 (VFX用加算合成PSO)
        Renderer* renderer = Renderer::GetInstance();
        renderer->SetVfxPipeline();
        //  コマンドリスト
        auto cmdList = DirectX::GetInstance()->GetCommandList();

        //  テクスチャのセット
        if (mTexture != nullptr) 
        {
            mTexture->Set(1);
        }

        Math::Matrix world;
        if (mBillboardType != eVfxBillboardType::None) {
            // カメラのベクトルを使って正面を向かせる
            Math::Vector3 right = Camera::Main->GetRight();
            Math::Vector3 up = Camera::Main->GetUp();
            Math::Vector3 forward = Camera::Main->GetForward();

            world = Math::Matrix(
                right.x, right.y, right.z, 0.0f,
                up.x, up.y, up.z, 0.0f,
                forward.x, forward.y, forward.z, 0.0f,
                mPosition.x, mPosition.y, mPosition.z, 1.0f
            );
            world = Math::Matrix::Scaling(mScale.x, mScale.y, mScale.z) * world;
        }
        else {
            Math::Matrix mS = Math::Matrix::Scaling(mScale.x, mScale.y, mScale.z);
            Math::Matrix mR = Math::Matrix::YawPitchRoll(mAngle.x, mAngle.y, mAngle.z);
            Math::Matrix mT; mT.Translate(mPosition);
            world = mS * mR * mT;
        }

        // WVP (ワールド・ビュー・プロジェクション)
        Math::Matrix wvp = world * Camera::Main->GetView() * Camera::Main->GetProjection();

        // 4. 定数バッファの構築と転送（VfxPipelineのRootParam 0 は CBV）
        VfxConstantBuffer cb;
        cb.matWVP = Math::Matrix::Transpose(wvp); // GPU用に転置
        cb.uvOffset = mUVOffset;
        cb.intensity = mIntensity;
        cb.padding = 0.0f;

        // UISpriteのようにConstantsで送るには構造体が大きすぎるため、
    // 本来は renderer->SetVfxConstantBuffer(0, &cb, sizeof(cb)); を使いたいですが、
    // まだ未実装であれば、暫定的に以下のように書けます。
        renderer->SetVfxConstantBuffer(0, &cb, sizeof(cb));

        // 5. 頂点データ（色は動的に更新）
        for (auto& v : mVertices) {
            v.Color = mBaseColor;
        }

        // 6. 描画（UISpriteと同じくRenderer::Draw経由で）
        static constexpr uint16_t Indices[6] = { 0, 1, 2, 1, 3, 2 };
        renderer->DrawVfx(mVertices.data(), 4, Indices, 6);
    }
}