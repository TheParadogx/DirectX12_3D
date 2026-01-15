#pragma once
#include<filesystem>
#include<cstdint>
#include<string>
#include<vector>
#include<unordered_map>
#include<memory>
#include<span>

#include<Math/Matrix/Matrix.h>
#include<Math/Vector3/Vector3.h>

#include"Graphics/DirectX/TypeAlias/DirectX12TypeAlias.hpp"

namespace Engine::Graphics
{

	struct SkeletalMeshVertex;
	class Texture;

	class FbxResource
	{
		/// <summary>
		/// マテリアル用構造体
		/// </summary>
		struct Material
		{
			std::string Name;
			std::unique_ptr<Graphics::Texture> Texture;
			uint32_t IndexCount = 0;
			uint32_t StartIndex = 0;
		};

		/// <summary>
		/// ボーン情報
		/// </summary>
		struct BoneInfo
		{
			/// <summary>
			/// ボーン名
			/// </summary>
			std::string Name;

			/// <summary>
			/// 親ボーンのインデックス
			/// </summary>
			int ParentIndex;

			/// <summary>
			/// 基本姿勢のボーン
			/// </summary>
			Math::Matrix BindMatrix;
		};

		/// <summary>
		/// アニメーション用構造体
		/// </summary>
		struct Animation
		{
			int NumFrame;
			std::vector<std::vector<Math::Matrix>> KeyFrame;
		};



	public:
		FbxResource();
		virtual ~FbxResource();

		/// <summary>
		/// コンバート済みFbxモデルファイルの読み込み
		/// </summary>
		/// <param name="FilePath">ファイルパス XXX.fbx.bin</param>
		/// <returns>true:成功</returns>
		bool Create(const std::filesystem::path& FilePath);

		/// <summary>
		/// コンバート済みアニメーション(モーション)ファイルの読み込み
		/// </summary>
		/// <param name="AnimationName">アニメーション名</param>
		/// <param name="AnimationPath">ファイルパス XXX.fbx.anm</param>
		/// <returns>true:成功</returns>
		bool LoadAnimation(const std::string& AnimationName, const std::filesystem::path& AnimationPath);

		/// <summary>
		/// リソースの解放
		/// </summary>
		void Release();

		//	ゲッター
		std::span<const Material> GetMaterials() const;
		std::span<const BoneInfo> GetBones()const;
		const std::unordered_map<std::string, FbxResource::Animation>& GetAnimations() const;
		std::span<const std::vector<Math::Matrix>> GetAnimationKeyFrames(const std::string& name) const;

		D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView()const;
		D3D12_INDEX_BUFFER_VIEW GetIndexBufferView()const;
	private:
		/// <summary>
		/// コンバート済みfbxモデルデータの読み込み
		/// </summary>
		/// <param name="FilePath"></param>
		/// <returns>true:成功</returns>
		bool LoadModelData(const std::filesystem::path& FilePath);

	private:
		/// <summary>
		/// マテリアル情報配列
		/// </summary>
		std::vector<Material> mMaterials;

		/// <summary>
		/// ボーン情報配列
		/// </summary>
		std::vector<BoneInfo> mBones;

		/// <summary>
		/// 読み込んだアニメーション列挙用
		/// </summary>
		std::unordered_map<std::string, Animation> mAnimations;

		/// <summary>
		/// 頂点バッファ
		/// </summary>
		mw::ResourcePtr mVB = nullptr;

		/// <summary>
		/// インデックスバッファ
		/// </summary>
		mw::ResourcePtr mIB = nullptr;


		D3D12_VERTEX_BUFFER_VIEW mVBView = {};
		D3D12_INDEX_BUFFER_VIEW mIBView = {};

	};
}


