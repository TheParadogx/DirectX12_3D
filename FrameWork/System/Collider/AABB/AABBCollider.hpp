#pragma once
#include"Math/Vector3/Vector3.h"

namespace Engine::System
{
	/// <summary>
	/// Axis-Aligned Bounding Boxes(座標軸に沿ったバウンディングボックス)
	/// 回転しないボックス用コリジョン
	/// </summary>
	class AABBCollider
	{
	public:
		AABBCollider();
		virtual ~AABBCollider() = default;


		struct Range
		{
			float Min;
			float Max;
		};

		void DebugRender();

		/// <summary>
		/// 中心座標の指定
		/// </summary>
		/// <param name="Center">中心座標</param>
		void SetCenter(const Math::Vector3& Center);
		
		/// <summary>
		/// 中心座標の取得
		/// </summary>
		/// <returns></returns>
		const Math::Vector3& GetCenter()const;

		/// <summary>
		/// 各軸の大きさの指定
		/// </summary>
		/// <param name="Volume"></param>
		void SetVolume(const Math::Vector3& Volume);

		/// <summary>
		/// 各軸の大きさの取得
		/// </summary>
		/// <returns></returns>
		const Math::Vector3& GetVolume()const;

		/// <summary>
		/// X軸の範囲の取得
		/// </summary>
		/// <returns></returns>
		Range GetAxisXRange()const;

		/// <summary>
		/// Y軸の範囲の取得
		/// </summary>
		/// <returns></returns>
		Range GetAxisYRange()const;

		/// <summary>
		/// Z軸の範囲の取得
		/// </summary>
		/// <returns></returns>
		Range GetAxisZRange()const;

	private:

		/// <summary>
		/// 中心座標
		/// </summary>
		Math::Vector3 mCenter;

		/// <summary>
		/// 各軸の大きさ
		/// </summary>
		Math::Vector3 mVolume;

	};
}

