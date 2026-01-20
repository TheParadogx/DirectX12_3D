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

/// <summary>
/// 補間にかける時間の設定
/// </summary>
/// <param name="BlendTime"></param>
void Engine::Graphics::FbxMesh::SetBlendTime(float BlendTime)
{
	if (BlendTime < 0.0f)
	{
		BlendTime = 0.0f;
	}
	mBlendTime = BlendTime;
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

	// アニメーションの存在チェック
	const auto& AnimationList = mResource->GetAnimations();
	if (AnimationList.count(AnimationName) == 0)
	{
		return;
	}

	// --- 1. アニメーションの切り替え判定 ---
	if (mCurrentAnimationName != AnimationName)
	{
		// 現在のアニメーションを「前の状態」として保存
		mPreviousAnimationName = mCurrentAnimationName;
		mPreviousFrame = mFrame;

		// 補間パラメータの設定
		mBlendTimer = 0.0f;
		// 初回再生時(空文字)は補間しない。以前のアニメーションが存在する場合のみ blending を有効にする
		mIsBlending = (!mPreviousAnimationName.empty());

		// 新しいアニメーションをセット
		mCurrentAnimationName = AnimationName;
		mFrame = 0.0f;
	}

	// 補間タイマーと現在のフレームの更新
	if (mIsBlending)
	{
		mBlendTimer += DeltaTime;
		if (mBlendTimer >= mBlendTime)
		{
			mIsBlending = false;
			mBlendTimer = mBlendTime; // Clamp
		}
	}

	const auto& anim = AnimationList.at(mCurrentAnimationName);
	const float duration = static_cast<float>(anim.NumFrame) * 0.016666666f;

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

	// 前のアニメーションのフレームも止まっていると不自然な場合があるので、
	// 必要なら mPreviousFrame も DeltaTime 分進める処理をここに入れてもOK

	// 行列計算とブレンディング
	const float currentFramePos = mFrame * 60.0f;
	int f0 = Math::Clamp((int)currentFramePos, 0, anim.NumFrame - 1);
	int f1 = Math::Clamp(f0 + 1, 0, anim.NumFrame - 1);
	float t = currentFramePos - (float)f0;

	const auto& bones = mResource->GetBones();
	const int numBones = (int)bones.size();

	// 補間の強さを算出 (0.0:前のアニメが100% ～ 1.0:今のアニメが100%)
	float blendAlpha = mIsBlending ? (mBlendTimer / mBlendTime) : 1.0f;

	for (int i = 0; i < numBones; ++i)
	{
		// 今のアニメーションの姿勢
		Math::Matrix localNow = Math::Matrix::Lerp(anim.KeyFrame[i][f0], anim.KeyFrame[i][f1], t);
		Math::Matrix finalLocal = localNow;

		// 補間中なら前のアニメーションの姿勢を計算して混ぜる
		if (mIsBlending && AnimationList.count(mPreviousAnimationName))
		{
			const auto& prevAnim = AnimationList.at(mPreviousAnimationName);

			float pFramePos = mPreviousFrame * 60.0f;
			int pf0 = Math::Clamp((int)pFramePos, 0, prevAnim.NumFrame - 1);
			int pf1 = Math::Clamp(pf0 + 1, 0, prevAnim.NumFrame - 1);
			float pt = pFramePos - (float)pf0;

			Math::Matrix localPrev = Math::Matrix::Lerp(prevAnim.KeyFrame[i][pf0], prevAnim.KeyFrame[i][pf1], pt);

			// 前の姿勢と今の姿勢を線形補間
			finalLocal = Math::Matrix::Lerp(localPrev, localNow, blendAlpha);
		}

		// 親子階層結合
		if (bones[i].ParentIndex >= 0)
		{
			mCurrentGlobalMatrices[i] = finalLocal * mCurrentGlobalMatrices[bones[i].ParentIndex];
		}
		else
		{
			mCurrentGlobalMatrices[i] = finalLocal;
		}

		// GPU送信用
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


	// モデルの姿勢行列を作成
	Math::Matrix world;
	// mPosition, mRotation, mScale を使って行列を合成
	world.Update(mPosition, mRotation, mScale);

	// 定数バッファへ転置してセット
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
	int index = this->FindBoneIndex(BoneName);
	if (index == -1 || index >= BONE_COUNT_MAX) return mRotation;

	// スケールを 1.0 に固定した「向き専用」の行列を作る
	Math::Matrix worldNoScale;
	worldNoScale.Update(mPosition, mRotation, Math::Vector3(1.0f, 1.0f, 1.0f));

	// ボーンの累積行列（これもスケールを含まないのが理想）
	Math::Matrix m = mCurrentGlobalMatrices[index] * worldNoScale;

	// 行列から回転を抽出
	Math::Quaternion q = Math::Quaternion::FromMatrix(m);

	// 念のため正規化して返す
	float lenSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
	if (lenSq > 0) {
		float len = sqrtf(lenSq);
		q.x /= len; q.y /= len; q.z /= len; q.w /= len;
	}
	return q;
}

Math::Matrix Engine::Graphics::FbxMesh::GetBoneFinalMatrix(const std::string& BoneName) const
{
	int index = this->FindBoneIndex(BoneName);

	// モデル自体のワールド行列を構築
	Math::Matrix world;
	world.Update(mPosition, mRotation, mScale);

	if (index == -1 || index >= BONE_COUNT_MAX)
	{
		return world; // ボーンが見つからなければ本体の行列を返す
	}

	// ボーンのモデル空間行列(アニメーション適用済) * モデルのワールド行列
	// これで「今、画面上のどこに手があるか」の行列が完成する
	return mCurrentGlobalMatrices[index] * world;
}
