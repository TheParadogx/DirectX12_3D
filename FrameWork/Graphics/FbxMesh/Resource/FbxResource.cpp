#include "pch.h"
#include "FbxResource.hpp"
#include"Graphics/Graphics.hpp"
#include"Graphics/Texture/Texture.hpp"
#include"Graphics/DirectX/DirectX12.hpp"

namespace Engine::Graphics
{
	FbxResource::FbxResource()
		:mMaterials()
		, mBones()
		, mAnimations()
	{
	}
	FbxResource::~FbxResource()
	{
		this->Release();
	}
	/// <summary>
	/// コンバート済みFbxモデルファイルの読み込み
	/// </summary>
	/// <param name="FilePath">ファイルパス XXX.fbx.bin</param>
	/// <returns>true:成功</returns>
	bool FbxResource::Create(const std::filesystem::path& FilePath)
	{
		//	ファイルが存在するかチェック
		if (std::filesystem::exists(FilePath) == false)
		{
			return false;
		}

		//	コンバートしたfbx.binファイルの読込
		bool ret = LoadModelData(FilePath.string());
		if (ret == false)
		{
			return false;
		}

		return true;
	}

	/// <summary>
	/// コンバート済みアニメーション(モーション)ファイルの読み込み
	/// </summary>
	/// <param name="AnimationName">アニメーション名</param>
	/// <param name="AnimationPath">ファイルパス XXX.fbx.anm</param>
	/// <returns>true:成功</returns>
	bool FbxResource::LoadAnimation(const std::string& AnimationName, const std::filesystem::path& AnimationPath)
	{
		if (std::filesystem::exists(AnimationPath) == false)
		{
			return false;
		}

		//	同一アニメーションデータの存在判定
		if (mAnimations.count(AnimationName) > 0)
		{
			return true;
		}

		//	ファイルを開く
		FILE* fp = nullptr;
		const errno_t error = fopen_s(&fp, AnimationPath.string().c_str(), "rb");
		if (error != 0)
		{
			return false;
		}

		Animation anim = {};

		//	総フレームの読込
		fread(&anim.NumFrame, sizeof(int), 1, fp);

		//　ボーン数の読み込み
		int boneCount = 0;
		fread(&boneCount, sizeof(int), 1, fp);

		//	全ボーン・全フレームの行列データのロード
		//	KeyFrame[ボーン数][アニメーションフレーム数]
		anim.KeyFrame.resize(boneCount);
		for (int bone = 0; bone < boneCount; bone++)
		{
			int frameCount = 0;
			fread(&frameCount, sizeof(int), 1, fp);

			//	行列配列を確保してバイナリから流し込む
			anim.KeyFrame[bone].resize(frameCount);
			fread(anim.KeyFrame[bone].data(), sizeof(Math::Matrix), frameCount, fp);
		}

		fclose(fp);
		mAnimations[AnimationName] = std::move(anim);

		return true;
	}

	/// <summary>
	/// コンバート済みfbxモデルデータの読み込み
	/// 例外を吐いても丸め込むしかないので速度重視でFILE* とfreadを使います
	/// </summary>
	/// <param name="FilePath"></param>
	/// <returns>true:成功</returns>
	bool FbxResource::LoadModelData(const std::filesystem::path& FilePath)
	{
		//	ファイルを開く
		FILE* fp = nullptr;
		const errno_t error = fopen_s(&fp, FilePath.string().c_str(), "rb");
		if (error != 0)
		{
			return false;
		}

		Graphics::DirectX* directX = Graphics::DirectX::GetInstance();
		ID3D12Device* device = directX->GetDevice();

		mw::CmdAllocatorPtr tempAlloc;
		device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&tempAlloc));
		mw::CmdListPtr tempCmdList;
		device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, tempAlloc.Get(), nullptr, IID_PPV_ARGS(&tempCmdList));

		std::filesystem::path directory = FilePath.parent_path();

		//	ヘッダー情報の読込
		int meshCount = 0, polygonCount = 0, vertexCount = 0;
		fread(&meshCount, sizeof(int), 1, fp);
		fread(&polygonCount, sizeof(int), 1, fp);
		fread(&vertexCount, sizeof(int), 1, fp);

		//	頂点データの読込（一時配列）
		//	GPUスキニング用構造体で読み込む
		std::vector<SkeletalMeshVertex> allVertices(vertexCount);
		for (int i = 0; i < vertexCount; i++)
		{
			auto& v = allVertices[i];
			fread(&v.Position, sizeof(Math::Vector3), 1, fp);
			fread(&v.UV, sizeof(Math::Vector2), 1, fp);
			fread(&v.Normal, sizeof(Math::Vector3), 1, fp);
			fread(v.Bone.data(), sizeof(int) * v.Bone.size(), 1, fp);
			fread(v.Weight.data(), sizeof(int) * v.Weight.size(), 1, fp);
		}

		//	インデックスデータの読み込み
		int indexCount = 0;
		fread(&indexCount, sizeof(int), 1, fp);
		std::vector<uint32_t> allIndices(indexCount);
		for (int i = 0; i < indexCount; i++)
		{
			int index = 0;
			fread(&index, sizeof(int), 1, fp);
			allIndices[i] = static_cast<uint32_t>(index);
		}

		//	マテリアル情報の読込
		int materialCount = 0;
		fread(&materialCount, sizeof(int), 1, fp);
		mMaterials.reserve(materialCount);

		uint32_t currentIndexOffset = 0;
		for (int i = 0; i < materialCount; i++)
		{
			Material readMaterial = {};

			//	マテリアル名
			int nameLen = 0;
			fread(&nameLen, sizeof(int), 1, fp);
			readMaterial.Name.resize(nameLen);
			fread(readMaterial.Name.data(), 1, nameLen, fp);

			//	テクスチャパス
			int texPathLen = 0;
			fread(&texPathLen, sizeof(int), 1, fp);
			std::string texturePath = {};
			texturePath.resize(texPathLen);
			fread(texturePath.data(), 1, texPathLen, fp);

			//	テクスチャのロード
			readMaterial.Texture = std::make_unique<Texture>();
			readMaterial.Texture->Create(directory / "Texture" / texturePath);

			//	ポリゴン数からインデックス範囲を確定
			int matPolygonCount = 0;
			fread(&matPolygonCount, sizeof(int), 1, fp);
			readMaterial.IndexCount = matPolygonCount * 3;
			readMaterial.StartIndex = currentIndexOffset;

			currentIndexOffset += readMaterial.IndexCount;
			mMaterials.push_back(std::move(readMaterial));
		}

		//	ボーン情報の読み込み
		int boneCount = 0;
		fread(&boneCount, sizeof(int), 1, fp);
		mBones.reserve(boneCount);
		for (int i = 0; i < boneCount; i++)
		{
			BoneInfo info = {};

			//	ボーン名
			int nameLen = 0;
			fread(&nameLen, sizeof(int), 1, fp);
			info.Name.resize(nameLen);
			fread(info.Name.data(), 1, nameLen, fp);

			//	親ボーンインデックス
			fread(&info.ParentIndex, sizeof(int), 1, fp);
			//	初期ポーズ用行列
			fread(&info.BindMatrix, sizeof(Math::Matrix), 1, fp);

			mBones.push_back(std::move(info));
		}

		fclose(fp);

		//	頂点バッファ（Default）
		const UINT vbSize = static_cast<UINT>(allVertices.size() * sizeof(SkeletalMeshVertex));

		D3D12_RESOURCE_DESC vbDesc = {};
		vbDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		vbDesc.Width = vbSize;
		vbDesc.Height = 1;
		vbDesc.DepthOrArraySize = 1;
		vbDesc.MipLevels = 1;
		vbDesc.SampleDesc.Count = 1;
		vbDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		D3D12_HEAP_PROPERTIES defaultHeap = {};
		defaultHeap.Type = D3D12_HEAP_TYPE_DEFAULT;

		device->CreateCommittedResource(
			&defaultHeap,
			D3D12_HEAP_FLAG_NONE,
			&vbDesc,
			D3D12_RESOURCE_STATE_COMMON, // コピー先として作成
			nullptr,
			IID_PPV_ARGS(&mVB)
		);

		//	中継用アップロードヒープ
		mw::ResourcePtr uploadVB;
		D3D12_HEAP_PROPERTIES uploadHeap = {};
		uploadHeap.Type = D3D12_HEAP_TYPE_UPLOAD;

		HRESULT hr =
			device->CreateCommittedResource(
				&uploadHeap,
				D3D12_HEAP_FLAG_NONE,
				&vbDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&uploadVB)
			);

		//	CPUからアップロードヒープに書き込み
		void* vbData = nullptr;
		uploadVB->Map(0, nullptr, &vbData);
		memcpy(vbData, allVertices.data(), vbSize);
		uploadVB->Unmap(0, nullptr);

		// GPU内部で Upload -> Default へ高速コピー
		tempCmdList->CopyBufferRegion(mVB.Get(), 0, uploadVB.Get(), 0, vbSize);

		// 状態遷移バリア（コピー先から頂点バッファとして使用可能へ）
		D3D12_RESOURCE_BARRIER vbBarrier = {};
		vbBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		vbBarrier.Transition.pResource = mVB.Get();
		vbBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
		vbBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
		vbBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		tempCmdList->ResourceBarrier(1, &vbBarrier);

		//	インデックスバッファ
		const UINT ibSize = static_cast<UINT>(allIndices.size() * sizeof(uint32_t));
		D3D12_RESOURCE_DESC ibDesc = vbDesc; // 基本設定を流用
		ibDesc.Width = ibSize;

		device->CreateCommittedResource(
			&defaultHeap,
			D3D12_HEAP_FLAG_NONE,
			&ibDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&mIB)
		);

		mw::ResourcePtr uploadIB;
		device->CreateCommittedResource(
			&uploadHeap,
			D3D12_HEAP_FLAG_NONE,
			&ibDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&uploadIB)
		);

		void* ibData = nullptr;
		uploadIB->Map(0, nullptr, &ibData);
		memcpy(ibData, allIndices.data(), ibSize);
		uploadIB->Unmap(0, nullptr);

		tempCmdList->CopyBufferRegion(mIB.Get(), 0, uploadIB.Get(), 0, ibSize);

		D3D12_RESOURCE_BARRIER ibBarrier = {};
		ibBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		ibBarrier.Transition.pResource = mIB.Get();
		ibBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
		ibBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_INDEX_BUFFER;
		ibBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		tempCmdList->ResourceBarrier(1, &ibBarrier);

		tempCmdList->Close();
		ID3D12CommandList* lists[] = { tempCmdList.Get()};
		directX->GetCommandQueue()->ExecuteCommandLists(1, lists);

		// 転送完了を待つ（ロード時なので確実に完了させる）
		directX->WaitForGPU();

		mVBView.BufferLocation = mVB->GetGPUVirtualAddress();
		mVBView.SizeInBytes = vbSize;
		mVBView.StrideInBytes = sizeof(SkeletalMeshVertex);

		mIBView.BufferLocation = mIB->GetGPUVirtualAddress();
		mIBView.SizeInBytes = ibSize;
		mIBView.Format = DXGI_FORMAT_R32_UINT;

		return true;
	}

	/// <summary>
	/// リソースの解放
	/// </summary>
	void FbxResource::Release()
	{
		mMaterials.clear();

		mBones.clear();
		mAnimations.clear();

		mVB.Reset();
		mIB.Reset();
	}

	//	マテリアル
	std::span<const FbxResource::Material> FbxResource::GetMaterials() const {
		return mMaterials;
	}

	//	ボーン
	std::span<const FbxResource::BoneInfo> FbxResource::GetBones() const {
		return mBones;
	}

	//	アニメーション全体
	const std::unordered_map<std::string, FbxResource::Animation>& FbxResource::GetAnimations() const {
		return mAnimations;
	}

	//	特定のアニメーション
	std::span<const std::vector<Math::Matrix>> FbxResource::GetAnimationKeyFrames(const std::string& name) const
	{
		if (mAnimations.count(name)) {
			return mAnimations.at(name).KeyFrame;
		}
		return {};
	}
	D3D12_VERTEX_BUFFER_VIEW FbxResource::GetVertexBufferView() const
	{
		return mVBView;
	}
	D3D12_INDEX_BUFFER_VIEW FbxResource::GetIndexBufferView() const
	{
		return mIBView;
	}
}