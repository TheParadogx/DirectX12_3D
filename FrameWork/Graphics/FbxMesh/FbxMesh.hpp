#pragma once
#include"Math/Vector3/Vector3.h"
#include"Math/Matrix/Matrix.h"
#include"Math/Quaternion/Quaternion.h"
#include"Graphics/Color/Color.hpp"
#include"Graphics/ConstantBuffer/ConstantBuffer.hpp"

#include<string>
#include<vector>

namespace Engine::Graphics
{

	struct SkeletalMeshVertex;
	class Texture;

	class FbxResource;

	class FbxMesh
	{


	public:
		FbxMesh();
		virtual ~FbxMesh();

		/// <summary>
		/// 作成
		/// </summary>
		/// <param name="Resource">読み込み済みのリソースのポインタ</param>
		/// <returns>true:成功</returns>
		bool Create(FbxResource* Resource);

		/// <summary>
		/// 解放
		/// </summary>
		void Release();

		/// <summary>
		/// アニメーション用更新処理（フレームを進める）
		/// </summary>
		/// <param name="AnimationName">Load時に登録したアニメーション名</param>
		/// <param name="DeltaTime"></param>
		/// <param name="IsLoop">true:アニメーションをループさせる</param>
		void Animate(
			const std::string& AnimationName,
			const float DeltaTime,
			const float IsLoop);

		/// <summary>
		/// 描画
		/// </summary>
		void Render();
		
		const Math::Vector3& GetPosition()const;
		void SetPosition(const Math::Vector3& position);

		const Math::Quaternion& GetRotation()const;
		void SetRotation(const Math::Quaternion& rotaion);

		const Math::Vector3& GetScale()const;
		void SetScale(const Math::Vector3& scale);

		void SetColor(const Color& color);

		/// <summary>
		/// ボーン名からワールド座標を取得する
		/// </summary>
		/// <param name="BoneName">ボーン名</param>
		/// <returns>座標</returns>
		Math::Vector3 GetBoneWorldPosition(const std::string& BoneName)const;

		/// <summary>
		/// ボーン名からワールド回転を取得する
		/// </summary>
		/// <param name="BoneName"></param>
		/// <returns></returns>
		Math::Quaternion GetBoneWorldRotation(const std::string& BoneName)const;

		/// <summary>
		/// 回転と座標を合わせた最終的なワールド行列を直接返す
		/// </summary>
		/// <param name="BoneName"></param>
		/// <returns></returns>
		Math::Matrix GetBoneFinalMatrix(const std::string& BoneName) const;

		/// <summary>
		/// ボーン名から配列のインデックスを検索するヘルパー
		/// </summary>
		/// <param name="BoneName">ボーン名</param>
		/// <returns></returns>
		int FindBoneIndex(const std::string& BoneName)const;

		const Math::Matrix& GetGlobalMatrix(int index) const {
			return mCurrentGlobalMatrices[index];
		}

		/// <summary>
		/// アニメーションが終了しているかどうか
		/// </summary>
		/// <returns>true:終了</returns>
		bool GetAnimationFinish()const
		{
			return mIsAnimationFinish;
		}

		/// <summary>
		/// 補間にかける時間の設定
		/// </summary>
		/// <param name="BlendTime"></param>
		void SetBlendTime(float BlendTime);

	protected:
		/// <summary>
		/// モデル用定数バッファ構造体
		/// </summary>
		struct MeshConstantBufferInfo {
			Math::Matrix World;
			Math::Matrix View;
			Math::Matrix Projection;
			Color MeshColor;
			Math::Matrix Bones[256]; // GPUスキニング用
			int IsSkinned;	// 0:スキニングをしない 1:スキニングをする
			std::array<float, 3> Padding;
		};


	protected:

		/// <summary>
		/// バッファサイズ
		/// </summary>
		static constexpr auto MeshConstantBufferSize = (sizeof(MeshConstantBufferInfo) + 255) & ~255;

		/// <summary>
		/// ボーン数（256は適当）
		/// よほどのモデルじゃなければ256で足りる。
		/// 軽量なモデルだけなら128でも問題ない。
		///	512は重たいし、そこまで必要なモデルは少ない。骨多すぎやろ？
		/// </summary>
		static constexpr int BONE_COUNT_MAX = 256;

		/// <summary>
		/// モデルデータのリソース
		/// </summary>
		FbxResource* mResource;

		/// <summary>
		/// 行列用の定数バッファ
		/// </summary>
		std::unique_ptr<ConstantBuffer> mTransformBuffer;

		/// <summary>
		/// GPUの送るボーン行列（最終変形行列）
		/// </summary>
		std::array<Math::Matrix, BONE_COUNT_MAX> mSkinningMatrices;

		/// <summary>
		/// CPU側で保持する現在のボーン行列
		/// ボーン名から座標取得の効率を上げる用
		/// </summary>
		std::array<Math::Matrix, BONE_COUNT_MAX> mCurrentGlobalMatrices;

		/// <summary>
		/// 今のアニメーション名
		/// </summary>
		std::string mCurrentAnimationName;

		/// <summary>
		/// 前のアニメーション名
		/// </summary>
		std::string mPreviousAnimationName;

		/// <summary>
		/// 座標
		/// </summary>
		Math::Vector3 mPosition;

		/// <summary>
		/// 角度・回転
		/// </summary>
		Math::Quaternion mRotation;

		/// <summary>
		/// 拡縮
		/// </summary>
		Math::Vector3 mScale;

		/// <summary>
		/// メッシュの色
		/// </summary>
		Color mMeshColor;

		/// <summary>
		///	メッシュの定数バッファ用の情報
		/// </summary>
		MeshConstantBufferInfo mCBInfo;

		/// <summary>
		/// 今のフレーム
		/// </summary>
		float mFrame;

		/// <summary>
		/// 前のフレーム進捗
		/// </summary>
		float mPreviousFrame = 0.0f;

		/// <summary>
		///	補間にかける全時間
		/// </summary>
		float mBlendTime = 0.2f;

		/// <summary>
		/// 補間開始からの経過時間
		/// </summary>
		float mBlendTimer = 0.0f;

		/// <summary>
		/// 今補間中かどうか
		/// </summary>
		bool  mIsBlending = false;

		/// <summary>
		/// アニメーションの終了判定
		/// </summary>
		bool mIsAnimationFinish;
	};
}

