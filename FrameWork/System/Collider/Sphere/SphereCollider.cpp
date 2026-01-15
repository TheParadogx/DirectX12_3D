#include "pch.h"
#include "SphereCollider.hpp"


Engine::System::SphereCollider::SphereCollider()
	:mCenter()
	, mRadius(10)
{
}

/// <summary>
/// 中心座標の取得
/// </summary>
/// <returns>中心座標</returns>
const Math::Vector3& Engine::System::SphereCollider::GetCenter()const
{
	return mCenter;
}

/// <summary>
/// 中心座標の指定
/// </summary>
/// <param name="center">中心座標</param>
void Engine::System::SphereCollider::SetCenter(const Math::Vector3& Center)
{
	mCenter = Center;
}

/// <summary>
/// 半径の取得
/// </summary>
/// <returns>半径</returns>
float Engine::System::SphereCollider::GetRadius()const
{
	return mRadius;
}

/// <summary>
/// 半径の指定
/// </summary>
/// <param name="radius">半径</param>
void Engine::System::SphereCollider::SetRadius(float Radius)
{
	mRadius = Radius;
}