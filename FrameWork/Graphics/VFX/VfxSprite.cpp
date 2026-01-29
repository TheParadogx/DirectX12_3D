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

        // 1.0サイズの正方形を作成（頂点順序とUVを厳密に合わせる）
            // 0: 左上
        mVertices[0].Position = { -0.5f,  0.5f, 0.0f };
        mVertices[0].UV = { 0.0f,  0.0f };
        mVertices[0].Color = Color::White();

        // 1: 右上
        mVertices[1].Position = { 0.5f,  0.5f, 0.0f };
        mVertices[1].UV = { 1.0f,  0.0f };
        mVertices[1].Color = Color::White();

        // 2: 左下
        mVertices[2].Position = { -0.5f, -0.5f, 0.0f };
        mVertices[2].UV = { 0.0f,  1.0f };
        mVertices[2].Color = Color::White();

        // 3: 右下
        mVertices[3].Position = { 0.5f, -0.5f, 0.0f };
        mVertices[3].UV = { 1.0f,  1.0f };
        mVertices[3].Color = Color::White();
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

        Renderer* renderer = Renderer::GetInstance();
        renderer->SetVfxPipeline();

        //if (mTexture) mTexture->Set(0);

        // 行列計算（ビルボード対応）
        Math::Matrix world;
        if (mBillboardType != eVfxBillboardType::None) {
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

        Math::Matrix testWVP = Math::Matrix::identity;
        // scale は 1.0 だと小さすぎるかもしれないので 0.5 くらいで
        testWVP = Math::Matrix::Scaling(0.5f, 0.5f, 1.0f);

        VfxConstantBuffer cb;
        cb.matWVP = Math::Matrix::Transpose(testWVP); // 2D表示
        cb.intensity = 1.0f;
        renderer->SetVfxConstantBuffer(0, &cb, sizeof(cb));

        // 頂点カラーの同期（変数をColorに変更）
        for (auto& v : mVertices) {
            v.Color = mBaseColor;
        }

        static constexpr uint16_t Indices[6] = { 0, 1, 2, 1, 3, 2 };
        renderer->DrawVfx(mVertices.data(), 4, Indices, 6);
    }
   }