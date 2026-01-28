#include "pch.h"
#include "SkyBoxResource.hpp"

#include "Graphics/Texture/Texture.hpp"
#include "Graphics/Graphics.hpp"
#include "Graphics/DirectX/DirectX12.hpp"


namespace Engine::Graphics
{
	SkyBoxResource::~SkyBoxResource()
	{
		Release();
	}

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="CubeMapPath">キューブマップテクスチャのパス</param>
	/// <returns>true:成功</returns>
	bool SkyBoxResource::Create(const std::filesystem::path& CubeMapPath)
	{
		bool ret = false;
		//	メッシュ
		ret = this->CreateMesh();
		if (ret == false)
		{
			LOG_ERROR("Failed CreateMesh.");
			return false;
		}
		//	テクスチャ
		mSkyTexture = std::make_unique<Texture>();
		ret = mSkyTexture->Create(CubeMapPath);
		if (ret == false)
		{
			LOG_ERROR("Failed CreateCubeTexture.");
			return false;
		}
		return true;
	}

	/// <summary>
	/// 立方体メッシュの作成
	/// </summary>
	/// <returns>true:成功</returns>
	bool SkyBoxResource::CreateMesh()
	{
		// 立方体の頂点データ定義（Triangle List / 1辺の長さ 2.0）
		// 座標そのものがサンプリングベクトルになるため、中心は(0,0,0)
		std::vector<SkyBoxVertex> vertices = {
			// 前面 (Z+)
			{{-1.0f,  1.0f,  1.0f}}, {{ 1.0f,  1.0f,  1.0f}}, {{ 1.0f, -1.0f,  1.0f}},
			{{-1.0f,  1.0f,  1.0f}}, {{ 1.0f, -1.0f,  1.0f}}, {{-1.0f, -1.0f,  1.0f}},
			// 背面 (Z-)
			{{ 1.0f,  1.0f, -1.0f}}, {{-1.0f,  1.0f, -1.0f}}, {{-1.0f, -1.0f, -1.0f}},
			{{ 1.0f,  1.0f, -1.0f}}, {{-1.0f, -1.0f, -1.0f}}, {{ 1.0f, -1.0f, -1.0f}},
			// 上面 (Y+)
			{{-1.0f,  1.0f, -1.0f}}, {{ 1.0f,  1.0f, -1.0f}}, {{ 1.0f,  1.0f,  1.0f}},
			{{-1.0f,  1.0f, -1.0f}}, {{ 1.0f,  1.0f,  1.0f}}, {{-1.0f,  1.0f,  1.0f}},
			// 下面 (Y-)
			{{-1.0f, -1.0f,  1.0f}}, {{ 1.0f, -1.0f,  1.0f}}, {{ 1.0f, -1.0f, -1.0f}},
			{{-1.0f, -1.0f,  1.0f}}, {{ 1.0f, -1.0f, -1.0f}}, {{-1.0f, -1.0f, -1.0f}},
			// 左面 (X-)
			{{-1.0f,  1.0f, -1.0f}}, {{-1.0f,  1.0f,  1.0f}}, {{-1.0f, -1.0f,  1.0f}},
			{{-1.0f,  1.0f, -1.0f}}, {{-1.0f, -1.0f,  1.0f}}, {{-1.0f, -1.0f, -1.0f}},
			// 右面 (X+)
			{{ 1.0f,  1.0f,  1.0f}}, {{ 1.0f,  1.0f, -1.0f}}, {{ 1.0f, -1.0f, -1.0f}},
			{{ 1.0f,  1.0f,  1.0f}}, {{ 1.0f, -1.0f, -1.0f}}, {{ 1.0f, -1.0f,  1.0f}}
		};

		//	取得
		Graphics::DirectX* directX = Graphics::DirectX::GetInstance();
		auto* device = directX->GetDevice();

		//	VBサイズ
		const UINT vbSize = static_cast<UINT>(vertices.size() * sizeof(SkyBoxVertex));

		//	バッファ作成
		D3D12_HEAP_PROPERTIES defaultHeap = {};
		defaultHeap.Type = D3D12_HEAP_TYPE_DEFAULT;
		D3D12_RESOURCE_DESC vbDesc = {};
		vbDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		vbDesc.Width = vbSize;
		vbDesc.Height = 1;
		vbDesc.DepthOrArraySize = 1;
		vbDesc.MipLevels = 1;
		vbDesc.SampleDesc.Count = 1;
		vbDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// Default Heap リソース
		device->CreateCommittedResource(
			&defaultHeap, D3D12_HEAP_FLAG_NONE, &vbDesc,
			D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&mVB));

		//	upload heapリソース
		mw::ResourcePtr uploadVB;
		D3D12_HEAP_PROPERTIES uploadHeap = {};
		uploadHeap.Type = D3D12_HEAP_TYPE_UPLOAD;
		device->CreateCommittedResource(
			&uploadHeap, D3D12_HEAP_FLAG_NONE, &vbDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uploadVB));

		//	データコピー
		void* mappedPtr = nullptr;
		uploadVB->Map(0, nullptr, &mappedPtr);
		memcpy(mappedPtr, vertices.data(), vbSize);
		uploadVB->Unmap(0, nullptr);

		//	GPUコピーコマンド
		mw::CmdAllocatorPtr tempAlloc;
		device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&tempAlloc));
		mw::CmdListPtr tempCmdList;
		device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, tempAlloc.Get(), nullptr, IID_PPV_ARGS(&tempCmdList));

		tempCmdList->CopyBufferRegion(mVB.Get(), 0, uploadVB.Get(), 0, vbSize);

		//	バリア処理
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = mVB.Get();
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		tempCmdList->ResourceBarrier(1, &barrier);

		//	実行
		tempCmdList->Close();
		ID3D12CommandList* lists[] = { tempCmdList.Get() };
		directX->GetCommandQueue()->ExecuteCommandLists(1, lists);
		directX->WaitForGPU();

		// view設定
		mVBView.BufferLocation = mVB->GetGPUVirtualAddress();
		mVBView.SizeInBytes = vbSize;
		mVBView.StrideInBytes = sizeof(SkyBoxVertex);

		return true;
	}

	/// <summary>
	/// リソースの解放
	/// </summary>
	void SkyBoxResource::Release()
	{
		mVB.Reset();
		mSkyTexture.reset();
	}

	D3D12_VERTEX_BUFFER_VIEW SkyBoxResource::GetVertexBufferView()const
	{
		return mVBView;
	}
	Texture* SkyBoxResource::GetTexture() const
	{
		return mSkyTexture.get();
	}
}