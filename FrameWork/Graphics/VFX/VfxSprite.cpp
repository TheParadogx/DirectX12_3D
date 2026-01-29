#include "pch.h"
#include "VfxSprite.hpp"
#include "Graphics/Renderer/Renderer.hpp"
#include "Graphics/Texture/Texture.hpp"
#include "System/Camera/Camera.hpp"
#include "Graphics/ConstantBuffer/ConstantBuffer.hpp"
#include "Graphics/DirectX/DirectX12.hpp"

namespace Engine::Graphics {

	VfxMesh::VfxMesh()
		:mVertexBuffer(nullptr)
		, mTexture(nullptr)
		, mPosition(0,0,0)
		, mRotation(Math::Quaternion::Identity)
		, mScale(1,1,1)
		, mSize(0,0)
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
		// サイズとピボットから、ローカル空間での 4 頂点を算出
		float left = -mSize.x * (0.5f + mPivot.x);
		float right = mSize.x * (0.5f - mPivot.x);
		float bottom = -mSize.y * (0.5f + mPivot.y);
		float top = mSize.y * (0.5f - mPivot.y);

		// 4頂点で Z 文字を描く順序 (TRIANGLESTRIP 用)
		Vertex vertices[] = {
			{ Math::Vector3(left,  bottom, 0.0f), Math::Vector2(0.0f, 1.0f) }, // 左下
			{ Math::Vector3(left,  top,    0.0f), Math::Vector2(0.0f, 0.0f) }, // 左上
			{ Math::Vector3(right, bottom, 0.0f), Math::Vector2(1.0f, 1.0f) }, // 右下
			{ Math::Vector3(right, top,    0.0f), Math::Vector2(1.0f, 0.0f) }, // 右上
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
		mVertexBuffer->Map(0, nullptr, &pData);
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

		if (mIsBillBoard)
		{
			// 1. 各成分の行列を作成
			Math::Matrix S = Math::Matrix::Scaling(mScale.x, mScale.y, mScale.z);
			Math::Matrix T;
			T.Translate(mPosition); // 既存のTranslateメソッドを使用

			// 2. ビルボード回転行列の作成
			// カメラの右・上・前ベクトルをそのまま回転軸にする
			const auto& right = System::Camera::Main->GetRight();
			const auto& up = System::Camera::Main->GetUp();
			const auto& fwd = System::Camera::Main->GetForward();

			Math::Matrix R = Math::Matrix::identity;
			R._11 = right.x; R._12 = right.y; R._13 = right.z;
			R._21 = up.x;    R._22 = up.y;    R._23 = up.z;
			R._31 = fwd.x;   R._32 = fwd.y;   R._33 = fwd.z;

			// 3. 合成 (自作Mathのオペレータを使用)
			mWorldMatrix = S * R * T;
		}
		else
		{
			// 通常時は Matrix 構造体の Update メソッドにお任せ
			mWorldMatrix.Update(mPosition, mRotation, mScale);
		}

	}

	/// <summary>
	/// 描画
	/// </summary>
	void VfxMesh::Render()
	{
		if (mTexture == nullptr || mVertexBuffer == nullptr) return;

		Renderer* renderer = Renderer::GetInstance();

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
		renderer->DrawVfx(mVBView, 4);
	}
}