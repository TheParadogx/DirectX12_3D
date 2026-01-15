#include "pch.h"
#include "UISprite.hpp"

#include"Graphics/Renderer/Renderer.hpp"
#include"Graphics/Texture/Texture.hpp"
#include"Graphics/Graphics.hpp"
#include"Graphics/Graphics.hpp"
#include"Math/Math.h"
#include"System/Window/Window.hpp"
#include"Graphics/DirectX/DirectX12.hpp"


void Engine::Graphics::UISprite::UpdateVertices(Graphics::SpriteVertex* Vertices)
{
    // Z型で定義（これが最も一般的です）
    // 0: 左上
    Vertices[0].Position = { 0.0f, 0.0f, 0.0f };
    Vertices[0].UV = { 0.0f, 0.0f };

    // 1: 右上 (FillRatioを反映)
    Vertices[1].Position = { mSize.x * mFillRatio, 0.0f, 0.0f };
    Vertices[1].UV = { mFillRatio, 0.0f };

    // 2: 左下
    Vertices[2].Position = { 0.0f, mSize.y, 0.0f };
    Vertices[2].UV = { 0.0f, 1.0f };

    // 3: 右下 (FillRatioを反映)
    Vertices[3].Position = { mSize.x * mFillRatio, mSize.y, 0.0f };
    Vertices[3].UV = { mFillRatio, 1.0f };

    // --- 以下、回転・拡縮・スクリーン変換の共通計算 ---
    const float theta = mAngle * Math::RAD;
    const float s = sinf(theta);
    const float c = cosf(theta);
    float winW = (float)System::Window::GetInstance()->GetWidth();
    float winH = (float)System::Window::GetInstance()->GetHeight();

    for (int i = 0; i < 4; i++) {
        // Pivot/Scale
        Vertices[i].Position.x = (Vertices[i].Position.x - mPivot.x * mSize.x) * mScale.x;
        Vertices[i].Position.y = (Vertices[i].Position.y - mPivot.y * mSize.y) * mScale.y;

        // Rotate
        float rx = Vertices[i].Position.x * c + Vertices[i].Position.y * s;
        float ry = Vertices[i].Position.x * -s + Vertices[i].Position.y * c;


        // Translate & Screen Mapping
        Vertices[i].Position.x = (rx + mPosition.x) * (2.0f / winW) - 1.0f;
        Vertices[i].Position.y = (ry + mPosition.y) * (-2.0f / winH) + 1.0f;
    }
}

/// <summary>
/// 表示
/// </summary>
void Engine::Graphics::UISprite::Render()
{
	Renderer* rederer = Renderer::GetInstance();
	auto cmdList = Graphics::DirectX::GetInstance()->GetCommandList();

	//	パイプラインのセット
	rederer->SetUISpritePipeline();

	//	ヒープをパイプラインへセット
	mTexture->Set(0);

	//	バーの割合を定数バッファでGPUに送る
	cmdList->SetGraphicsRoot32BitConstants(1, 4, &mColor, 0);
	cmdList->SetGraphicsRoot32BitConstants(1, 1, &mFillRatio, 4);

	Graphics::SpriteVertex Vertices[4] = {};
	UpdateVertices(Vertices);
	static constexpr uint16_t Indices[6] =
	{
		0, 1, 2,
		1, 3, 2
	};
	//	描画
	Graphics::Renderer::GetInstance()->DrawSprite(
		Vertices, 4, Indices, 6);

}
