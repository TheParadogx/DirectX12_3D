#include "pch.h"
#include "FbxMesh.hpp"

#include"Graphics/Graphics.hpp"
#include"Graphics/Texture/Texture.hpp"
#include"Graphics/Renderer/Renderer.hpp"
#include"Graphics/FbxMesh/Resource/FbxResource.hpp"

#include"System/Camera/Camera.hpp"

#include"Math/Math.h"

/// <summary>
/// ボーン名から配列のインデックスを検索するヘルパー
/// </summary>
/// <param name="BoneName">ボーン名</param>
/// <returns></returns>
int Engine::Graphics::FbxMesh::FindBoneIndex(const std::string& BoneName) const
{
	if (mResource == nullptr)
	{
		return -1;
	}

	const auto& bones = mResource->GetBones();
	for (int i = 0; i < static_cast<int>(bones.size()); i++)
	{
		if (bones[i].Name == BoneName)
		{
			return i;
		}
	}

	return -1;
}

Engine::Graphics::FbxMesh::FbxMesh()
	:mResource(nullptr)
	, mTransformBuffer(nullptr)
	, mSkinningMatrices()
	, mFrame(0)
	, mCurrentAnimationName()
	, mPosition(0,0,0)
	, mRotation()
	, mScale(1,1,1)
	, mMeshColor(Color::White())
	, mIsAnimationFinish(false)
{
}

Engine::Graphics::FbxMesh::~FbxMesh()
{
	this->Release();
}

/// <summary>
/// 作成
/// </summary>
/// <param name="Resource">読み込み済みのリソースのポインタ</param>
/// <returns>true:成功</returns>
bool Engine::Graphics::FbxMesh::Create(FbxResource* Resource)
{
	bool ret = false;

	mTransformBuffer = std::make_unique<ConstantBuffer>();
	ret = mTransformBuffer->Create(MeshConstantBufferSize);
	if (ret == false)
	{
		LOG_ERROR("Failed Create ConstantBuffer");
		return false;
	}

	// リソースの参照取得
	mResource = Resource;
	if (mResource == nullptr)
	{
		return false;
	}

	mCBInfo.World = Math::Matrix::identity;
	mCBInfo.View = Math::Matrix::identity;
	mCBInfo.Projection = Math::Matrix::identity;
	mCBInfo.MeshColor = Color::Red();

	//	ボーン行列を初期姿勢にセット（単位行列）
	//	Animate呼ばれるまでモデルが多分つぶれる
	for(int i = 0; i < BONE_COUNT_MAX;i++)
	{
		mSkinningMatrices[i] = Math::Matrix::identity;
		mCurrentGlobalMatrices[i] = Math::Matrix::identity;
		mCBInfo.Bones[i] = Math::Matrix::identity;
	}
	mTransformBuffer->Update(&mCBInfo);

	//	リソースからデフォルト (最初に登録されたもの) のアニメーション名を取得しておく
	auto& anims = mResource->GetAnimations();
	if (anims.empty() == false)
	{
		mCurrentAnimationName = anims.begin()->first;
	}


	return true;
}

void Engine::Graphics::FbxMesh::Release()
{
	mResource = nullptr;
	mTransformBuffer.reset();
	mCurrentAnimationName.clear();
}

/// <summary>
/// アニメーション用更新処理（フレームを進める）
/// </summary>
/// <param name="AnimationName">Load時に登録したアニメーション名</param>
/// <param name="DeltaTime"></param>
/// <param name="IsLoop">true:アニメーションをループさせる</param>
void Engine::Graphics::FbxMesh::Animate(const std::string& AnimationName, const float DeltaTime, const float IsLoop)
{
	if (mResource == nullptr)
	{
		return;
	}

	//	アニメーションの存在チェック
	const auto& AnimationList = mResource->GetAnimations();
	if (AnimationList.count(AnimationName) == 0)
	{
		//	読み込まれていないアニメーション
		return;
	}

	// true:今のアニメーションと引数のアニメーションは別物
	if (mCurrentAnimationName != AnimationName)
	{
		//	アニメーション変更
		mFrame = 0;
		mCurrentAnimationName = AnimationName;
	}

	const auto& anim = AnimationList.at(mCurrentAnimationName);
	//	アニメーションが60fps想定の数値なので余裕ができたらリソース取得の時にアニメーションの想定fpsを取得して1フレームでの時間を自由に設定できるようにしたい
	const float duration = static_cast<float>(anim.NumFrame) * 0.016666666f;

	//	フレーム更新
	mFrame += DeltaTime;
	if (mFrame >= duration)
	{
		if (IsLoop == true)
		{
			mFrame = fmod(mFrame, duration);
			mIsAnimationFinish = false;
		}
		else
		{
			mFrame = duration;
			mIsAnimationFinish = true;
		}
	}
	else
	{
		mIsAnimationFinish = false;
	}

	//	行列計算
	const float currentFramePos = mFrame * 60.0f;
	int f0 = Math::Clamp((int)currentFramePos, 0, anim.NumFrame - 1);
	int f1 = Math::Clamp(f0 + 1, 0, anim.NumFrame - 1);
	float t = currentFramePos - (float)f0;

	const auto& bones = mResource->GetBones();
	const int numBones = (int)bones.size();

	for (int i = 0; i < numBones; ++i) {
		// キーフレーム補間
		Math::Matrix local = Math::Matrix::Lerp(anim.KeyFrame[i][f0], anim.KeyFrame[i][f1], t);

		// 親子階層結合
		if (bones[i].ParentIndex >= 0) {
			mCurrentGlobalMatrices[i] = local * mCurrentGlobalMatrices[bones[i].ParentIndex];
		}
		else {
			mCurrentGlobalMatrices[i] = local;
		}

		// GPU送信用：最終行列 = BindMatrix * GlobalMatrix (転置して格納)
		mSkinningMatrices[i] = Math::Matrix::Transpose(bones[i].BindMatrix * mCurrentGlobalMatrices[i]);
	}
}

/// <summary>
/// 描画
/// </summary>
void Engine::Graphics::FbxMesh::Render()
{
	if (mResource == nullptr)
	{
		return;
	}

	//	パイプライン設定をRenderer経由で行う
	Graphics::Renderer* renderer = Graphics::Renderer::GetInstance();
	renderer->SetFbxPipeline();


	// 1. モデルの姿勢行列を作成
	Math::Matrix world;
	// mPosition, mRotation, mScale を使って行列を合成
	world.Update(mPosition, mRotation, mScale);

	// 2. 定数バッファへ転置してセット
	mCBInfo.World = Math::Matrix::Transpose(world);
	mCBInfo.View = Math::Matrix::Transpose(System::Camera::Main->GetView());
	mCBInfo.Projection = Math::Matrix::Transpose(System::Camera::Main->GetProjection());
	mCBInfo.MeshColor = mMeshColor;

	bool hasBones = (mResource->GetBones().size() > 0);
	mCBInfo.IsSkinned = hasBones ? 1 : 0;
	if (hasBones)
	{
		// ボーンがある場合のみ行列をコピー
		memcpy(mCBInfo.Bones, mSkinningMatrices.data(), sizeof(Math::Matrix) * BONE_COUNT_MAX);
	}
	else
	{
		// ボーンがない場合は、念のため Bones[0] を単位行列にしておくと安全（任意）
		mCBInfo.Bones[0] = Math::Matrix::identity;
	}

	mTransformBuffer->Update(&mCBInfo);

	mTransformBuffer->Set(0);

	//	マテリアル毎に描画をする
	auto materials = mResource->GetMaterials();
	auto vbv = mResource->GetVertexBufferView();
	auto ibv = mResource->GetIndexBufferView();

	for (const auto& mat : materials)
	{
		//	テクスチャ(SRV)をRootParam[1]にセット
		if (mat.Texture != nullptr)
		{
			mat.Texture->Set(1);
		}

		renderer->DrawMesh(vbv, ibv, mat.IndexCount, mat.StartIndex);
	}
}


const Math::Vector3& Engine::Graphics::FbxMesh::GetPosition()const
{
	return mPosition;
}

void Engine::Graphics::FbxMesh::SetPosition(const Math::Vector3& position)
{
	mPosition = position;
}

const Math::Quaternion& Engine::Graphics::FbxMesh::GetRotation() const
{
	return mRotation;
}

void Engine::Graphics::FbxMesh::SetRotation(const Math::Quaternion& rotaion)
{
	mRotation = rotaion;
}

const Math::Vector3& Engine::Graphics::FbxMesh::GetScale() const
{
	return mScale;
}

void Engine::Graphics::FbxMesh::SetScale(const Math::Vector3& scale)
{
	mScale = scale;
}

void Engine::Graphics::FbxMesh::SetColor(const Color& color)
{
	mMeshColor = color;
}

/// <summary>
/// ボーン名からワールド座標を取得するメソッド
/// </summary>
/// <param name="BoneName">ボーン名</param>
/// <returns>座標</returns>
Math::Vector3 Engine::Graphics::FbxMesh::GetBoneWorldPosition(const std::string& BoneName) const
{
	//	index
	int index = this->FindBoneIndex(BoneName);
	if (index == -1 || index >= BONE_COUNT_MAX)
	{
		return mPosition;
	}

	//	モデル自体のワールド行列を計算
	Math::Matrix world;
	world.Update(mPosition, mRotation, mScale);

	const auto& b = mCurrentGlobalMatrices[index];
	Math::Vector3 boneLocalPos(b._41, b._42, b._43);

	// モデルのワールド行列で座標変換する
	return Math::Vector3::TransformCoord(boneLocalPos, world);
}

Math::Quaternion Engine::Graphics::FbxMesh::GetBoneWorldRotation(const std::string& BoneName) const
{
	//	index
	int index = this->FindBoneIndex(BoneName);
	if (index == -1 || index >= BONE_COUNT_MAX)
	{
		return mRotation;
	}

	//	モデル自体のワールド行列を計算
	Math::Matrix world;
	world.Update(mPosition, mRotation, mScale);

	//	モデル空間のボーン行列にモデルのワールド行列を掛ける
	Math::Matrix m = mCurrentGlobalMatrices[index] * world;
	// ベクトルの作成
	Math::Vector3 x(m._11, m._12, m._13);
	Math::Vector3 y(m._21, m._22, m._23);
	Math::Vector3 z(m._31, m._32, m._33);

	// 正規化
	x.Normalize();
	y.Normalize();
	z.Normalize();

	// 正規化された軸を使って回転行列を組み立てる
	Math::Matrix rot = Math::Matrix::identity;
	rot._11 = x.x; rot._12 = x.y; rot._13 = x.z;
	rot._21 = y.x; rot._22 = y.y; rot._23 = y.z;
	rot._31 = z.x; rot._32 = z.y; rot._33 = z.z;

	// クォータニオンに変換
	return Math::Quaternion::FromMatrix(rot);
}
