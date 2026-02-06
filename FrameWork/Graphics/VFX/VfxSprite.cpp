#include "pch.h"
#include "VfxSprite.hpp"
#include "Graphics/Renderer/Renderer.hpp"
#include "Graphics/Texture/Texture.hpp"
#include "System/Camera/Camera.hpp"
#include "Graphics/ConstantBuffer/ConstantBuffer.hpp"
#include "Graphics/DirectX/DirectX12.hpp"
#include "Graphics/DebugRender/DebugRender.hpp"


namespace Engine::Graphics {

	VfxMesh::VfxMesh()
		:mVertexBuffer(nullptr)
		, mTexture(nullptr)
		, mPosition(0,10,0)
		, mRotation(Math::Quaternion::Identity)
		, mScale(1,1,1)
		, mSize(1,1)
		, mPivot(0,0)
		, mColor(Color::White())
		, mIsBillBoard(true)
		, mTimer(0)
	{
	}

	/// <summary>
	/// 作成
	/// </summary>
	/// <returns>true:成功</returns>
	bool VfxMesh::Create(Texture* texture)
	{
		if (texture == nullptr) return false;
		mTexture = texture;

		float w = static_cast<float>(texture->GetWidth());
		float h = static_cast<float>(texture->GetHeight());
		mSize.x = w / h; // 横幅を比率にする (例: 256x64なら 4.0)
		mSize.y = 1.0f;  // 高さを基準 1.0 にする

		bool ret = false;

		//	定数バッファ
		mTransformBuffer = std::make_unique<ConstantBuffer>();
		ret = mTransformBuffer->Create(ConstantBufferSize);
		if (ret == false)
		{
			LOG_ERROR("Failed CreateConstantBuffer.");
			return false;
		}

		//	頂点バッファの作成
		ret = CreateMesh();
		if (ret == false)
		{
			LOG_ERROR("Failed CreateMesh");
			return false;
		}

		return true;
	}

	/// <summary>
	/// 板ポリゴンの生成
	/// </summary>
	/// <returns></returns>
	bool VfxMesh::CreateMesh()
	{
		float left = -0.5f - mPivot.x;
		float right = 0.5f - mPivot.x;
		float bottom = -0.5f - mPivot.y;
		float top = 0.5f - mPivot.y;

		Vertex vertices[] = {
			{ Math::Vector3(left,  bottom, 0.0f), Math::Vector2(0.0f, 1.0f) },
			{ Math::Vector3(left,  top,    0.0f), Math::Vector2(0.0f, 0.0f) },
			{ Math::Vector3(right, bottom, 0.0f), Math::Vector2(1.0f, 1.0f) },
			{ Math::Vector3(right, bottom, 0.0f), Math::Vector2(1.0f, 1.0f) },
			{ Math::Vector3(left,  top,    0.0f), Math::Vector2(0.0f, 0.0f) },
			{ Math::Vector3(right, top,    0.0f), Math::Vector2(1.0f, 0.0f) },
		};

		// --- DX12 リソース作成 ---
		ID3D12Device* device = DirectX::GetInstance()->GetDevice();

		D3D12_HEAP_PROPERTIES heapProps = { D3D12_HEAP_TYPE_UPLOAD };
		D3D12_RESOURCE_DESC resDesc = {};
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Width = sizeof(vertices);
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mVertexBuffer));

		void* pData = nullptr;
		D3D12_RANGE readRange = { 0, 0 }; // CPUからは読み取らない
		mVertexBuffer->Map(0, &readRange, &pData);
		memcpy(pData, vertices, sizeof(vertices));
		mVertexBuffer->Unmap(0, nullptr);

		mVBView.BufferLocation = mVertexBuffer->GetGPUVirtualAddress();
		mVBView.SizeInBytes = sizeof(vertices);
		mVBView.StrideInBytes = sizeof(Vertex);

		return true;
	}

	/// <summary>
	/// 行列の計算やビューボード制御
	/// </summary>
	void VfxMesh::Update(float DeltaTime)
	{
		//	時間の経過などをさせてもいい
		mTimer += DeltaTime;

		if (mIsBillBoard && System::Camera::Main != nullptr) {

			// 自分からカメラへの方向ベクトルを作成
			Math::Vector3 cameraPos = System::Camera::Main->GetPosition();
			Math::Vector3 toCamera = mPosition - cameraPos;

			// その方向を向くクォータニオンを作成
			mRotation = Math::Quaternion::LookRotation(toCamera,Math::Vector3::Up);

			Math::Vector3 finalScale(mScale.x * mSize.x, mScale.y * mSize.y, mScale.z);

			// 行列を更新（サイズと位置を適用）
			mWorldMatrix.Update(mPosition, mRotation, finalScale);
		}
		else {
			Math::Vector3 finalScale(mScale.x * mSize.x, mScale.y * mSize.y, mScale.z);
			mWorldMatrix.Update(mPosition, mRotation, finalScale);
		}

	}

	/// <summary>
	/// 描画
	/// </summary>
	void VfxMesh::Render()
	{
		if (mTexture == nullptr || mVertexBuffer == nullptr) return;
		if (System::Camera::Main == nullptr) return;

		Renderer* renderer = Renderer::GetInstance();
		renderer->SetVfxPipeline();

		//	定数バッファの更新
		VfxConstantBufferInfo cb;
		cb.World = Math::Matrix::Transpose(mWorldMatrix);
		cb.View = Math::Matrix::Transpose(System::Camera::Main->GetView());
		cb.Projection = Math::Matrix::Transpose(System::Camera::Main->GetProjection());
		cb.MeshColor = mColor;
		cb.Timer = mTimer;

		mTransformBuffer->Update(&cb);
		mTransformBuffer->Set(0);

		mTexture->Set(1);

		//	描画
		renderer->DrawVfx(mVBView, 6);

#if _DEBUG
		DebugRender::DrawDebugQuad(mWorldMatrix, mSize.x, mSize.y, Color::Green());
#endif // _DEBUG
	}



}