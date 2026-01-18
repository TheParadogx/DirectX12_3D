#pragma once
#include"Math/Vector3/Vector3.h"
#include"Math/Quaternion/Quaternion.h"

namespace Engine::System
{
	class OBBCollider
	{
	public:
		/// <summary>
		/// 当たり判定用の四角形描画の作成
		/// </summary>
		void DebugRender();

		/// <summary>
		/// 中心の座標の取得
		/// </summary>
		/// <returns>中心の座標</returns>
		const Math::Vector3& GetCenter()const;

		/// <summary>
		/// 中心座標の指定
		/// </summary>
		/// <param name="center">中心座標</param>
		void SetCenter(const Math::Vector3& center);

		/// <summary>
		/// 各軸の大きさの取得
		/// </summary>
		/// <returns>各軸の大きさ</returns>
		const Math::Vector3& GetVolume()const;

		/// <summary>
		/// 各軸の大きさの指定
		/// </summary>
		/// <param name="volume">各軸の大きさ</param>
		void SetVolume(const Math::Vector3& volume);

		/// <summary>
		/// オブジェクトの回転の取得
		/// </summary>
		/// <returns>オブジェクトの回転</returns>
		const Math::Quaternion& GetRotation()const;

		/// <summary>
		/// オブジェクトの回転の指定
		/// </summary>
		/// <param name="rotation">オブジェクトの回転</param>
		void SetRotation(const Math::Quaternion& rotation);
	private:
		/// <summary>
		/// 中心座標
		/// </summary>
		Math::Vector3 mCenter = { 0.0f,0.0f,0.0f };

		/// <summary>
		/// サイズ
		/// </summary>
		Math::Vector3 mVolume = {1.0f,1.0f,1.0f};

		/// <summary>
		/// 回転
		/// </summary>
		Math::Quaternion mRotation = Math::Quaternion::Identity;
	};
}

