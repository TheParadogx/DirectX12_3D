#include "pch.h"
#include "AABBCollider.hpp"
#include "Graphics/Renderer/Renderer.hpp"
#include"Graphics/Color/Color.hpp"

Engine::System::AABBCollider::AABBCollider()
	:mCenter()
	, mVolume()
{

}

void Engine::System::AABBCollider::DebugRender()
{
	Graphics::LineVertex vertices[8];
	for (auto& v : vertices)
	{
		v.color = Graphics::Color::Yellow();
	}

	const Math::Vector3 Right = Math::Vector3(mVolume.x * 0.5f, 0.0f, 0.0f);
	const Math::Vector3 Up = Math::Vector3(0.0f, mVolume.y * 0.5f, 0.0f);
	const Math::Vector3 Forward = Math::Vector3(0.0f, 0.0f, mVolume.z * 0.5f);
	//	上面
	//	左手前
	vertices[0].Position = -Right + Up + Forward + mCenter;
	//	左奥
	vertices[1].Position = -Right + Up + -Forward + mCenter;
	//	右手前
	vertices[2].Position = Right + Up + Forward + mCenter;
	//	右奥
	vertices[3].Position = Right + Up + -Forward + mCenter;
	//	底面
	//	左手前
	vertices[4].Position = -Right + -Up + Forward +mCenter;
	//	左奥
	vertices[5].Position = -Right + -Up + -Forward + mCenter;
	//	右手前
	vertices[6].Position = Right + -Up + Forward + mCenter;
	//	右奥
	vertices[7].Position = Right + -Up + -Forward + mCenter;

	const uint16_t indices[24] =
	{
		//上面
		0,1,
		0,2,
		1,3,
		2,3,
		//縦線
		0,4,
		1,5,
		3,7,
		2,6,
		//底面
		4,5,
		4,6,
		5,7,
		6,7
	};

	Graphics::Renderer* renderer = Graphics::Renderer::GetInstance();
	renderer->DrawLines(vertices, 8, indices, 24);
}

/// <summary>
/// 中心座標の指定
/// </summary>
/// <param name="Center">中心座標</param>
void Engine::System::AABBCollider::SetCenter(const Math::Vector3& Center)
{
	mCenter = Center;
}

/// <summary>
/// 中心座標の取得
/// </summary>
/// <returns></returns>
const Math::Vector3& Engine::System::AABBCollider::GetCenter() const
{
	return 	mCenter;
}

/// <summary>
/// 各軸の大きさの指定
/// </summary>
/// <param name="Volume"></param>
void Engine::System::AABBCollider::SetVolume(const Math::Vector3& Volume)
{
	mVolume = Volume;
}

/// <summary>
/// 各軸の大きさの取得
/// </summary>
/// <returns></returns>
const Math::Vector3& Engine::System::AABBCollider::GetVolume() const
{
	return mVolume;
}

/// <summary>
/// X軸の範囲の取得
/// </summary>
/// <returns></returns>
Engine::System::AABBCollider::Range Engine::System::AABBCollider::GetAxisXRange() const
{
	const float HalfVolume = mVolume.x / 2;
	Range range = {};
	range.Min = mCenter.x - HalfVolume;
	range.Max = mCenter.x + HalfVolume;
	return range;
}

/// <summary>
/// Y軸の範囲の取得
/// </summary>
/// <returns></returns>
Engine::System::AABBCollider::Range Engine::System::AABBCollider::GetAxisYRange() const
{
	const float HalfVolume = mVolume.y / 2;
	Range range = {};
	range.Min = mCenter.y - HalfVolume;
	range.Max = mCenter.y + HalfVolume;
	return range;
}

/// <summary>
/// Z軸の範囲の取得
/// </summary>
/// <returns></returns>
Engine::System::AABBCollider::Range Engine::System::AABBCollider::GetAxisZRange() const
{
	const float HalfVolume = mVolume.z / 2;
	Range range = {};
	range.Min = mCenter.z - HalfVolume;
	range.Max = mCenter.z + HalfVolume;
	return range;
}
