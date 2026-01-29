//
#include <math.h>
#include "Quaternion.h"
#include "../Matrix/Matrix.h"
#include "../Matrix3x3/Matrix3x3.h"
#include "../Vector3/Vector3.h"

namespace Math
{
	const Quaternion Quaternion::Identity = { 0.0f, 0.0f, 0.0f, 1.0f };

	Matrix Quaternion::ToMatrix()const
	{
		const Matrix3x3 mat = ToMatrix3x3();
		Matrix m = {};
		m._11 = mat._11;
		m._12 = mat._12;
		m._13 = mat._13;

		m._21 = mat._21;
		m._22 = mat._22;
		m._23 = mat._23;

		m._31 = mat._31;
		m._32 = mat._32;
		m._33 = mat._33;

		m._44 = 1.0f;
		return m;
	}

	Matrix3x3 Quaternion::ToMatrix3x3()const
	{
		const float Qx = x * x;
		const float Qy = y * y;
		const float Qz = z * z;
		const float Qw = w * w;

		Matrix3x3 m = {};

		m._11 = Qx - Qy - Qz + Qw;
		m._12 = 2 * (x * y + z * w);
		m._13 = 2 * (x * z - y * w);

		m._21 = 2 * (x * y - z * w);
		m._22 = -Qx + Qy - Qz + Qw;
		m._23 = 2 * (y * z + x * w);

		m._31 = 2 * (x * z + y * w);
		m._32 = 2 * (y * z - x * w);
		m._33 = -Qx - Qy + Qz + Qw;

		return m;
	}

	Quaternion Quaternion::FromMatrix(const Matrix& m)
	{
		float tr = m._11 + m._22 + m._33;
		Quaternion q;

		if (tr > 0.0f) {
			float s = sqrtf(tr + 1.0f) * 2.0f; // s = 4 * qw
			q.w = 0.25f * s;
			q.x = (m._23 - m._32) / s;
			q.y = (m._31 - m._13) / s;
			q.z = (m._12 - m._21) / s;
		}
		else if ((m._11 > m._22) && (m._11 > m._33)) {
			float s = sqrtf(1.0f + m._11 - m._22 - m._33) * 2.0f; // s = 4 * qx
			q.w = (m._23 - m._32) / s;
			q.x = 0.25f * s;
			q.y = (m._12 + m._21) / s;
			q.z = (m._31 + m._13) / s;
		}
		else if (m._22 > m._33) {
			float s = sqrtf(1.0f + m._22 - m._11 - m._33) * 2.0f; // s = 4 * qy
			q.w = (m._31 - m._13) / s;
			q.x = (m._12 + m._21) / s;
			q.y = 0.25f * s;
			q.z = (m._23 + m._32) / s;
		}
		else {
			float s = sqrtf(1.0f + m._33 - m._11 - m._22) * 2.0f; // s = 4 * qz
			q.w = (m._12 - m._21) / s;
			q.x = (m._31 + m._13) / s;
			q.y = (m._23 + m._32) / s;
			q.z = 0.25f * s;
		}
		return q;
	}

	Quaternion Quaternion::Multiply(const Quaternion& p, const Quaternion& q)
	{
		Quaternion qua = {};

		qua.x = (q.w * p.x) - (q.z * p.y) + (q.y * p.z) + (q.x * p.w);
		qua.y = (q.z * p.x) + (q.w * p.y) - (q.x * p.z) + (q.y * p.w);
		qua.z = -(q.y * p.x) + (q.x * p.y) + (q.w * p.z) + (q.z * p.w);
		qua.w = -(q.x * p.x) - (q.y * p.y) - (q.z * p.z) + (q.w * p.w);

		return qua;
	}

	Quaternion Quaternion::AngleAxis(float angle, const Vector3& axis)
	{
		const float a = angle * 0.5f;
		const float sin = sinf(a);

		Vector3 v = axis;
		v.Normalize();

		const Quaternion q =
		{
			v.x * sin,
			v.y * sin,
			v.z * sin,
			cosf(a)
		};
		return q;
	}

	Quaternion Quaternion::Lerp(const Quaternion& a, const Quaternion& b, float t)
	{
		const float u = 1.0f - t;
		Quaternion q = Quaternion::Identity;
		q.x = a.x * u + b.x * t;
		q.y = a.y * u + b.y * t;
		q.z = a.z * u + b.z * t;
		q.w = a.w * u + b.w * t;

		// ここで長さを測って、1じゃなければ1にする
		float magSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
		if (magSq > 0.000001f) {
			float invMag = 1.0f / sqrtf(magSq);
			q.x *= invMag; q.y *= invMag; q.z *= invMag; q.w *= invMag;
		}

		return q;
	}

	Quaternion Quaternion::Slerp(const Quaternion& a, const Quaternion& b, float t)
	{
		Quaternion p = a;
		Quaternion q = b;

		//	内積計算
		float dot = Dot(p, q);
		if (dot < 0.0f)
		{
			q.x = -q.x;
			q.y = -q.y;
			q.z = -q.z;
			q.w = -q.w;
			dot = -dot;
		}
		float u = 0.0f, v=0.0f;
		//	密接具合チェック
		if (dot > 0.9f)
		{
			return Lerp(p, q, t);
		}
		else
		{
			float sin = sqrtf(1.0f - dot * dot);
			float angle = atan2f(sin, dot);

			float value = 1.0f / sin;
			u = sinf((1.0f - t)* angle) * value;
			v = sinf(t* angle) * value;
		}
		const Quaternion ret =
		{
			q.x * u + p.x * v,
			q.y * u + p.y * v,
			q.z * u + p.z * v,
			q.w * u + p.w * v
		};
		return ret;
	}

	Quaternion Quaternion::LookRotation(const Vector3& Direction, const Vector3& UpAxis)
	{
		//	方向ベクトルがほぼない状態
		if (Direction.SqrLength() < 0.0001f) return Quaternion::Identity;

		// Y軸（高さ）を無視して水平方向の角度を出す場合
		// ※モデルの正面が Vector3::Forward (0,0,1) であることを前提とする
		float angle = atan2f(Direction.x, Direction.z);

		// Y軸（Vector3::Up）を回転軸としてクォータニオンを生成
		return Quaternion::AngleAxis(angle, UpAxis);
	}

}
