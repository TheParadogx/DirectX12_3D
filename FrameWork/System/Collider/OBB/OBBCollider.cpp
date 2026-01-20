#include "pch.h"
#include "OBBCollider.hpp"
#include"Graphics/Graphics.hpp"
#include"Math/Matrix/Matrix.h"
#include"Graphics/Renderer/Renderer.hpp"

namespace Engine::System
{
	/// <summary>
/// 当たり判定用の四角形描画の作成
/// </summary>

	void OBBCollider::DebugRender()
	{
		Graphics::LineVertex vertices[8];
		for (auto& v : vertices) { v.color = Graphics::Color::Magenta(); }

		// 1. 行列から回転軸（向き）を取得して、サイズを掛ける
		const Math::Matrix rot = GetRotation().ToMatrix();
		const Math::Vector3 half = GetVolume() * 0.5f;

		// 各軸の正規化ベクトルに半分のサイズを掛けて、中心からのオフセット量にする
		const Math::Vector3 Right = Math::Vector3::Normalize(rot.GetRightVector()) * half.x;
		const Math::Vector3 Up = Math::Vector3::Normalize(rot.GetUpVector()) * half.y;
		const Math::Vector3 Forward = Math::Vector3::Normalize(rot.GetForwardVector()) * half.z;

		// 2. 頂点計算 (中心 + 各軸の組み合わせ)
		const Math::Vector3& C = mCenter;
		vertices[0].Position = C - Right + Up + Forward; // 左手上
		vertices[1].Position = C - Right + Up - Forward; // 左手奥
		vertices[2].Position = C + Right + Up + Forward; // 右手上
		vertices[3].Position = C + Right + Up - Forward; // 右手奥
		vertices[4].Position = C - Right - Up + Forward; // 左手下
		vertices[5].Position = C - Right - Up - Forward; // 左手奥下
		vertices[6].Position = C + Right - Up + Forward; // 右手下
		vertices[7].Position = C + Right - Up - Forward; // 右手奥下

		// 3. インデックス（線をつなぐ順序）
		const uint16_t indices[24] = {
			0,1, 0,2, 1,3, 2,3, // 上面
			4,5, 4,6, 5,7, 6,7, // 下面
			0,4, 1,5, 2,6, 3,7  // 縦の柱
		};

		Graphics::Renderer::GetInstance()->DrawLines(vertices, 8, indices, 24);
	}

	/// <summary>
	/// 中心の座標の取得
	/// </summary>
	/// <returns>中心の座標</returns>
	const Math::Vector3& OBBCollider::GetCenter()const
	{
		return mCenter;
	}

	/// <summary>
	/// 中心座標の指定
	/// </summary>
	/// <param name="center">中心座標</param>
	void OBBCollider::SetCenter(const Math::Vector3& center)
	{
		mCenter = center;
	}

	/// <summary>
	/// 各軸の大きさの取得
	/// </summary>
	/// <returns>各軸の大きさ</returns>
	const Math::Vector3& OBBCollider::GetVolume()const
	{
		return mVolume;
	}

	/// <summary>
	/// 各軸の大きさの指定
	/// </summary>
	/// <param name="volume">各軸の大きさ</param>
	void OBBCollider::SetVolume(const Math::Vector3& volume)
	{
		mVolume = volume;
	}

	/// <summary>
	/// オブジェクトの回転の取得
	/// </summary>
	/// <returns>オブジェクトの回転</returns>
	const Math::Quaternion& OBBCollider::GetRotation()const
	{
		return mRotation;
	}

	/// <summary>
	/// オブジェクトの回転の指定
	/// </summary>
	void OBBCollider::SetRotation(const Math::Quaternion& rotation)
	{
		mRotation = rotation;
	}
}