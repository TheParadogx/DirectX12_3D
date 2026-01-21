#include "pch.h"
#include "Camera.hpp"
#include"Math/Math.h"

Engine::System::Camera* Engine::System::Camera::Main = nullptr;

Engine::System::Camera::Camera()
	:mView()
	, mProjection()
	, mPosition()
	, mRight()
	, mUp()
	, mForward()
	, mFov()
	, mAspect()
	, mZNear()
	, mZFar()
{
}

bool Engine::System::Camera::Create()
{
	SetFieldOfView(90.0f);
	SetAspect(16.0f, 9.0f);
	SetZ(0.1f, 1000.0f);
	//	自分以外の前のカメラが残っている時
	if (Main != nullptr && Main != this)
	{
		delete Main;
		Main = nullptr;
	}
	Main = this;
	return true;
}

void Engine::System::Camera::Release()
{
	if (Main != nullptr)
	{
		delete Main;
		Main = nullptr;
	}
}

void Engine::System::Camera::Update(
	const Math::Vector3& Position,
	const Math::Vector3& Target,
	const Math::Vector3& Up
)
{
	mPosition = Position;
	//	カメラ行列
	mView = Math::Matrix::LookAt(Position, Target, Up);
	//	カメラの右ベクトル更新
	mRight.x = mView._11;
	mRight.y = mView._21;
	mRight.z = mView._31;

	//	カメラの上ベクトル更新
	mUp.x = mView._12;
	mUp.y = mView._22;
	mUp.z = mView._32;

	//	カメラの前ベクトル更新
	mForward.x = mView._13;
	mForward.y = mView._23;
	mForward.z = mView._33;

	//	プロジェクション行列設定
	//Projection = Math::Matrix::PerspectiveFovRH(Fov, Aspect, ZNear, ZFar);
	mProjection = Math::Matrix::PerspectiveFovLH(mFov, mAspect, mZNear, mZFar);

}

void Engine::System::Camera::SetFieldOfView(float Angle)
{
	mFov = Angle * Math::RAD;
}

void Engine::System::Camera::SetAspect(float Aspect)
{
	this->mAspect = Aspect;
}
void Engine::System::Camera::SetAspect(float Width, float Height)
{
	mAspect = Width / Height;
}

void Engine::System::Camera::SetZ(float Near, float Far)
{
	mZNear = Near;
	mZFar = Far;
}

const Math::Matrix& Engine::System::Camera::GetView()const
{
	return mView;
}
const Math::Matrix& Engine::System::Camera::GetProjection()const
{
	return mProjection;
}

const Math::Vector3& Engine::System::Camera::GetPosition()const
{
	return mPosition;
}
const Math::Vector3& Engine::System::Camera::GetRight()
{
	return mRight;
}
const Math::Vector3& Engine::System::Camera::GetUp()const
{
	return mUp;
}

const Math::Vector3& Engine::System::Camera::GetForward()const
{
	return mForward;
}
