#pragma once
#include"Graphics/FbxMesh/FbxMesh.hpp"
#include"Math/Math.h"

#include<memory>
#include<string>

namespace Engine::System
{
	struct FbxComponent
	{
		//	3Dモデル描画補間用
		Math::Quaternion OffsetRotation = Math::Quaternion::Identity;

		//	FBX本体
		std::unique_ptr<Graphics::FbxMesh> Mesh;

		//	今のアニメーション名
		std::string CurrAnimation = "";
		//	アニメーションの経過倍率
		float AnimationScale = 1.0f;
		//	ループ再生するかどうか
		bool IsLoop = true;
		//	再生中かどうか
		bool IsPlay = true;


		FbxComponent(Graphics::FbxResource* Resource, bool isUEOrigin = true)
		{
			Mesh = std::make_unique<Graphics::FbxMesh>();
			Mesh->Create(Resource);
			if (isUEOrigin == true)
			{
				OffsetRotation = Math::Quaternion::AngleAxis(-90.0f * Math::RAD, Math::Vector3::Right);
			}
		}

		// unique_ptr を持っているならコピーは禁止すべき
		FbxComponent(const FbxComponent&) = delete;
		FbxComponent& operator=(const FbxComponent&) = delete;

		// ムーブは許可する
		FbxComponent(FbxComponent&&) noexcept = default;
		FbxComponent& operator=(FbxComponent&&) noexcept = default;
	};
}