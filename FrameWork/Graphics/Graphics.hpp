#pragma once
#include<array>
#include"Graphics/Color/Color.hpp"
#include"Math/Vector3/Vector3.h"
#include"Math/Vector2/Vector2.h"
#include"System/Memory/Memory.hpp"

namespace Engine::Graphics
{
	/// <summary>
/// 1フレームに使用出来る頂点バッファ量
/// </summary>
	static constexpr size_t PerMaxVertexBufferSize = System::Memory::MiB(64);

	/// <summary>
	/// 1フレームに使用出来るインデックスバッファ量
	/// </summary>
	static constexpr size_t PerMaxIndexBufferSize = System::Memory::MiB(64 + 32);


	/// <summary>
	/// ライン描画用の頂点構造体
	/// </summary>
	struct LineVertex
	{
		Math::Vector3 Position;
		Color color;
	};

	/// <summary>
	/// シンプルな2Dスプライト描画用の頂点構造体
	/// </summary>
	struct SimpleSpriteVertex
	{
		Math::Vector3 Position;
		Color color;
	};

	/// <summary>
	/// 2Dスプライト描画用の頂点構造体
	/// </summary>
	struct SpriteVertex
	{
		Math::Vector3 Position;
		Math::Vector2 UV;
	};

	/// <summary>
	/// シンプルな3Dメッシュ描画用頂点構造体
	/// </summary>
	struct SimpleMeshVertex
	{
		Math::Vector3 Position;
		Math::Vector3 Normal;
	};

	/// <summary>
	/// ボーンのない3Dメッシュ描画用頂点構造体
	/// </summary>
	struct StaticMeshVertex
	{
		Math::Vector3 Position;
		Math::Vector2 UV;
		Math::Vector3 Normal;
	};

	/// <summary>
	/// ボーン情報の3Dメッシュ描画用の頂点構造体
	/// </summary>
	struct SkeletalMeshVertex
	{
		Math::Vector3 Position;
		Math::Vector2 UV;
		Math::Vector3 Normal;
		std::array<int, 4> Bone;
		std::array<float, 4> Weight;
	};

	/// <summary>
	/// スカイボックス描画用の頂点構造体
	/// </summary>
	struct SkyBoxVertex
	{
		Math::Vector3 Position;
	};

}