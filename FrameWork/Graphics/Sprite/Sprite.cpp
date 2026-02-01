#include "pch.h"
#include "Sprite.hpp"

#include"Graphics/Texture/Texture.hpp"
#include"Graphics/Renderer/Renderer.hpp"
#include"System/Window/Window.hpp"
#include"Math/Math.h"

Engine::Graphics::Sprite::Sprite()
	:mTexture(nullptr)
	, mPosition(0,0)
	, mPivot(0,0)
	, mScale(1,1)
	, mSize(200, 200)
	, mAngle(0)
	, mColor({10,10,10,1})
	, mConstantBuffer(nullptr)
{
}

/// <summary>
/// 生成
/// </summary>
/// <param name="Texture">テクスチャのポインタ</param>
/// <returns>true:成功</returns>
bool Engine::Graphics::Sprite::Create(Graphics::Texture* Texture)
{
	if (Texture == nullptr)
	{
		return false;
	}
	mTexture = Texture;

	mSize.x = Texture->GetWidth();
	mSize.y = Texture->GetHeight();

	mConstantBuffer = std::make_unique<ConstantBuffer>();
	mConstantBuffer->Create(sizeof(SpriteConstantBuffer));

	return true;
}

/// <summary>
/// テクスチャ描画
/// </summary>
void Engine::Graphics::Sprite::Render()
{
	Renderer* rederer = Renderer::GetInstance();

	//	パイプラインのセット
	rederer->SetSpritePipeline();

	SpriteConstantBuffer cb;
	cb.WVP = Math::Matrix::identity;
	cb.Color = mColor;
	mConstantBuffer->Update(&cb);

	mConstantBuffer->Set(0);

	//	ヒープをパイプラインへセット
	mTexture->Set(1);

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

void Engine::Graphics::Sprite::UpdateVertices(Graphics::SpriteVertex* Vertices)
{

	//// テスト：画面中央に大きく表示されるはずの数値
	//Vertices[0].Position = { -0.5f,  0.5f, 0.0f }; Vertices[0].UV = { 0,0 }; // 左上
	//Vertices[1].Position = { -0.5f, -0.5f, 0.0f }; Vertices[1].UV = { 0,1 }; // 左下
	//Vertices[2].Position = { 0.5f,  0.5f, 0.0f }; Vertices[2].UV = { 1,0 }; // 右上
	//Vertices[3].Position = { 0.5f, -0.5f, 0.0f }; Vertices[3].UV = { 1,1 }; // 右下

	//	左上
	Vertices[0].Position = { 0.0f, 0.0f, 0.0f };
	Vertices[0].UV.x = 0.0f;
	Vertices[0].UV.y = 0.0f;

	//	左下
	Vertices[1].Position = { 0.0f, mSize.y, 0.0f };
	Vertices[1].UV.x = 0.0f;
	Vertices[1].UV.y = 1.0f;

	//	右上
	Vertices[2].Position = { mSize.x, 0.0f, 0.0f };
	Vertices[2].UV.x = 1.0f;
	Vertices[2].UV.y = 0.0f;

	//	右下
	Vertices[3].Position = { mSize.x, mSize.y, 0.0f };
	Vertices[3].UV.x = 1.0f;
	Vertices[3].UV.y = 1.0f;

	const float theta = mAngle * Math::RAD;
	const float sin = sinf(theta);
	const float cos = cosf(theta);

	for (int i = 0; i < 4; i++)
	{
		// 座標の中心の指定(ローカル座標)と拡縮 
		Vertices[i].Position.x -= mPivot.x * mSize.x;
		Vertices[i].Position.y -= mPivot.y * mSize.y;
		Vertices[i].Position.x *= mScale.x;
		Vertices[i].Position.y *= mScale.y;

		// 座標の回転	
		float x = Vertices[i].Position.x;
		float y = Vertices[i].Position.y;

		Vertices[i].Position.x = x * cos + y * sin;
		Vertices[i].Position.y = x * -sin + y * cos;

		//// 座標の平行移動	
		//x = Vertices[i].Position.x + mPosition.x;
		//y = Vertices[i].Position.y + mPosition.y;

		float finalX = round(Vertices[i].Position.x + mPosition.x);
		float finalY = round(Vertices[i].Position.y + mPosition.y);

		finalX -= 0.5f;
		finalY -= 0.5f;

		Vertices[i].Position.x = finalX * (2.0f / System::Window::GetInstance()->GetWidth()) - 1.0f;
		Vertices[i].Position.y = finalY * (-2.0f / System::Window::GetInstance()->GetHeight()) + 1.0f;
	}

}
