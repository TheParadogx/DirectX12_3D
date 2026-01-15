#include "Vector3.h"
#include "../Matrix/Matrix.h"
#include "../Matrix3x3/Matrix3x3.h"

namespace Math
{
	//	upベクトル(0,1,0)
	const Vector3 Vector3::Up = { 0.0f,1.0f,0.0f };

	//	downベクトル(0,-1,0)
	const Vector3 Vector3::Down = { 0.0f,-1.0f,0.0f };

	//	forwardベクトル(0,0,1)
	const Vector3 Vector3::Forward = { 0.0f,0.0f,1.0f };

	//	backベクトル(0,0,-1)
	const Vector3 Vector3::Back = { 0.0f,0.0f,-1.0f };

	//	rightベクトル(1,0,0)
	const Vector3 Vector3::Right = { 1.0f,0.0f,0.0f };

	//	leftベクトル(-1,0,0
	const Vector3 Vector3::Left = { -1.0f,0.0f,0.0f };

	//	zeroベクトル(0,0,0)
	const Vector3 Vector3::Zero = { 0.0f,0.0f,0.0f };

	//	oneベクトル(1,1,1)
	const Vector3 Vector3::One = { 1.0f,1.0f,1.0f };

	Vector3 Vector3::Cross(const Vector3& a, const Vector3& b)noexcept
	{
		Vector3 v = {};
		v.x = a.y * b.z - a.z * b.y;
		v.y = a.z * b.x - a.x * b.z;
		v.z = a.x * b.y - a.y * b.x;
		return v;
	}

	Vector3  Vector3::Lerp(const Vector3& a, const Vector3& b, float t)
	{
		const float u = 1.0f - t;

		Vector3 v = {};
		v.x = a.x * u + b.x * t;
		v.y = a.y * u + b.y * t;
		v.z = a.z * u + b.z * t;
		return v;
	}

	/// <summary>
	/// 反射ベクトルの計算
	/// </summary>
	/// <param name="Direction"></param>
	/// <param name="NormalizedNormal">正規化された法線</param>
	/// <returns>反射ベクトル</returns>
	Vector3 Vector3::Reflect(const Vector3& Direction, const Vector3& NormalizedNormal)
	{
		const Vector3& d = Direction;
		const Vector3& n = NormalizedNormal;
		const float dot = Vector3::Dot(-d, n);

		Vector3 v = d + n * dot;
		v.x *= 2.0f;
		v.y *= 2.0f;
		v.z *= 2.0f;

		return v - d;
	}


	Vector3 Vector3::TransformCoord(const Vector3& a, const Matrix& m)
	{
		Vector3 v = {};
		v.x = a.x * m._11 + a.y * m._21 + a.z * m._31 + m._41;
		v.y = a.x * m._12 + a.y * m._22 + a.z * m._32 + m._42;
		v.z = a.x * m._13 + a.y * m._23 + a.z * m._33 + m._43;
		const float w = a.x * m._14 + a.y * m._24 + a.z * m._34 + m._44;
		const float t = 1.0f / w;
		v.x *= t;
		v.y *= t;
		v.z *= t;
		return v;
	}

	Vector3 Vector3::TransformNormal(const Vector3& a, const Matrix& m)
	{
		Vector3 v = {};
		v.x = a.x * m._11 + a.y * m._21 + a.z * m._31;
		v.y = a.x * m._12 + a.y * m._22 + a.z * m._32;
		v.z = a.x * m._13 + a.y * m._23 + a.z * m._33;

		return v;
	}

	Vector3 Vector3::TransformNormal(const Vector3& a, const Matrix3x3& m)
	{
		Vector3 v = {};
		v.x = a.x * m._11 + a.y * m._21 + a.z * m._31;
		v.y = a.x * m._12 + a.y * m._22 + a.z * m._32;
		v.z = a.x * m._13 + a.y * m._23 + a.z * m._33;

		return v;
	}

}
