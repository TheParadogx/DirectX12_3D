#include "pch.h"
#include "SkyBox.hpp"

#include "System/Camera/Camera.hpp"
#include "Graphics/Renderer/Renderer.hpp"

Engine::Graphics::SkyBox::SkyBox()
	:mResource(nullptr), mTransformBuffer(nullptr)
{
}

Engine::Graphics::SkyBox::~SkyBox()
{
	this->Release();
}

/// <summary>
/// 作成
/// </summary>
/// <param name="Resource">リソース管理から取得したリソースのポインタ</param>
/// <returns>true:成功</returns>
bool Engine::Graphics::SkyBox::Create(SkyBoxResource* Resource)
{
	if (Resource == nullptr) return false;

	mResource = Resource;

	mTransformBuffer = std::make_unique<ConstantBuffer>();
	return mTransformBuffer->Create(ConstantBufferSize);
}

/// <summary>
/// 解放
/// </summary>
void Engine::Graphics::SkyBox::Release()
{
	mTransformBuffer.reset();
	mResource = nullptr;
}

/// <summary>
/// 描画
/// </summary>
void Engine::Graphics::SkyBox::Render()
{
	if (mResource == nullptr) return;

	//	パイプライン設定
	auto* renderer = Renderer::GetInstance();
	renderer->SetSkyBoxPipeline();

	// 平行移動成分を消去したView行列の作成
	Math::Matrix view = System::Camera::Main->GetView();
	//	行列の移動成分の削除
	view._41 = 0.0f;
	view._42 = 0.0f;
	view._43 = 0.0f;

	//	行列更新
	SkyBoxConstantBufferInfo cb;
	cb.World = Math::Matrix::Transpose(Math::Matrix::identity);
	cb.View = Math::Matrix::Transpose(view); // 移動を消した行列を転置して送る
	cb.Projection = Math::Matrix::Transpose(System::Camera::Main->GetProjection());

	mTransformBuffer->Update(&cb);

	//	テクスチャ設定
	mTransformBuffer->Set(0);
	Texture* tex = mResource->GetTexture();
	if (tex != nullptr)
	{
		tex->Set(1);
	}

	//	描画
	renderer->DrawSkyBox(mResource->GetVertexBufferView(), 36);
}
