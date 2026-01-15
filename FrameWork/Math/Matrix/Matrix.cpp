#include "Matrix.h"
#include "../Vector3/Vector3.h"
#include "../Quaternion/Quaternion.h"

namespace Math
{

	const Matrix Matrix::identity =
	{ 
		1.0f,0.0f,0.0f,0.0f,
		0.0f, 1.0f,0.0f,0.0f,
		0.0f,0.0f, 1.0f,0.0f,
		0.0f,0.0f,0.0f, 1.0f 
	};

	const Matrix Matrix::zero =
	{ 
		0.0f,0.0f,0.0f,0.0f,
		0.0f, 0.0f,0.0f,0.0f,
		0.0f,0.0f, 0.0f,0.0f,
		0.0f,0.0f,0.0f, 0.0f 
	};


	void Matrix::Translate(const Vector3& position)
	{
		_41 = position.x;
		_42 = position.y;
		_43 = position.z;
	}

	void Matrix::Update(const Vector3& position, const Vector3& rotation, const Vector3& scale)
	{
		const Quaternion qx = Quaternion::AngleAxis(rotation.x, Vector3::Right);
		const Quaternion qy = Quaternion::AngleAxis(rotation.y, Vector3::Up);
		const Quaternion qz = Quaternion::AngleAxis(rotation.z, Vector3::Forward);

		const Quaternion qrot = qz * qx * qy;

		Matrix m = qrot.ToMatrix();

		_11 = m._11 * scale.x;
		_12 = m._12 * scale.x;
		_13 = m._13 * scale.x;

		_21 = m._21 * scale.y;
		_22 = m._22 * scale.y;
		_23 = m._23 * scale.y;

		_31 = m._31 * scale.z;
		_32 = m._32 * scale.z;
		_33 = m._33 * scale.z;

		_41 = position.x;
		_42 = position.y;
		_43 = position.z;
	}

	void Matrix::Update(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
	{
		*this = rotation.ToMatrix();

		_11 *= scale.x;
		_12 *= scale.x;
		_13 *= scale.x;

		_21 *= scale.y;
		_22 *= scale.y;
		_23 *= scale.y;

		_31 *= scale.z;
		_32 *= scale.z;
		_33 *= scale.z;

		Translate(position);
	}


	Vector3 Matrix::GetRightVector()const
	{
		Vector3 v = {};
		v.x = _11;
		v.y = _12;
		v.z = _13;
		return v;
	}

	Vector3 Matrix::GetUpVector()const
	{
		Vector3 v = {};
		v.x = _21;
		v.y = _22;
		v.z = _23;
		return v;
	}

	Vector3 Matrix::GetForwardVector()const
	{
		Vector3 v = {};
		v.x = _31;
		v.y = _32;
		v.z = _33;
		return v;
	}


	Matrix Matrix::Transpose(const Matrix& matrix)
	{
		Matrix m = {};
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				m.Mat[i][j] = matrix.Mat[j][i];
			}
		}
		return m;
	}


	Matrix Matrix::Inverse(const Matrix& mat)
	{
		Matrix m = Matrix::zero;
		//	matが単位行列なら逆行列を作らない
		if (Matrix::isIdentity(mat))
		{
			return mat;
		}
		//	1行1列目
		m._11 = mat._22 * (mat._33 * mat._44 - mat._34 * mat._43) +
			mat._23 * (mat._34 * mat._42 - mat._32 * mat._44) +
			mat._24 * (mat._32 * mat._43 - mat._33 * mat._42);

		//	1行2列目
		m._12 = mat._21 * (mat._33 * mat._44 - mat._34 * mat._43) +
			mat._23 * (mat._34 * mat._41 - mat._31 * mat._44) +
			mat._24 * (mat._31 * mat._43 - mat._33 * mat._41);

		//	1行3列目
		m._13 = mat._21 * (mat._32 * mat._44 - mat._34 * mat._42) +
			mat._22 * (mat._34 * mat._41 - mat._31 * mat._44) +
			mat._24 * (mat._31 * mat._42 - mat._32 * mat._41);

		//	1行4列目
		m._14 = mat._21 * (mat._32 * mat._43 - mat._33 * mat._42) +
			mat._22 * (mat._33 * mat._41 - mat._31 * mat._43) +
			mat._23 * (mat._31 * mat._42 - mat._32 * mat._41);

		//	2行1列目
		m._21 = mat._12 * (mat._33 * mat._44 - mat._34 * mat._43) +
			mat._13 * (mat._34 * mat._42 - mat._32 * mat._44) +
			mat._14 * (mat._32 * mat._43 - mat._33 * mat._42);

		//	2行2列目
		m._22 = mat._11 * (mat._33 * mat._44 - mat._34 * mat._43) +
			mat._13 * (mat._34 * mat._41 - mat._31 * mat._44) +
			mat._14 * (mat._31 * mat._43 - mat._33 * mat._41);

		//	2行3列目
		m._23 = mat._11 * (mat._32 * mat._44 - mat._34 * mat._42) +
			mat._12 * (mat._34 * mat._41 - mat._31 * mat._44) +
			mat._14 * (mat._31 * mat._42 - mat._32 * mat._41);

		//	2行4列目
		m._24 = mat._11 * (mat._32 * mat._43 - mat._33 * mat._42) +
			mat._12 * (mat._33 * mat._41 - mat._31 * mat._43) +
			mat._13 * (mat._31 * mat._42 - mat._32 * mat._41);

		//	3行1列目
		m._31 = mat._12 * (mat._23 * mat._44 - mat._24 * mat._43) +
			mat._13 * (mat._24 * mat._42 - mat._22 * mat._44) +
			mat._14 * (mat._22 * mat._43 - mat._23 * mat._42);

		//	3行2列目
		m._32 = mat._11 * (mat._23 * mat._44 - mat._24 * mat._43) +
			mat._13 * (mat._24 * mat._41 - mat._21 * mat._44) +
			mat._14 * (mat._21 * mat._43 - mat._23 * mat._41);

		//	3行3列目
		m._33 = mat._11 * (mat._22 * mat._44 - mat._24 * mat._42) +
			mat._12 * (mat._24 * mat._41 - mat._21 * mat._44) +
			mat._14 * (mat._21 * mat._42 - mat._22 * mat._41);

		//	3行4列目
		m._34 = mat._11 * (mat._22 * mat._43 - mat._23 * mat._42) +
			mat._12 * (mat._23 * mat._41 - mat._21 * mat._43) +
			mat._13 * (mat._21 * mat._42 - mat._22 * mat._41);

		//	4行1列目
		m._41 = mat._12 * (mat._23 * mat._34 - mat._24 * mat._33) +
			mat._13 * (mat._24 * mat._32 - mat._22 * mat._34) +
			mat._14 * (mat._22 * mat._33 - mat._23 * mat._32);

		//	4行2列目
		m._42 = mat._11 * (mat._23 * mat._34 - mat._24 * mat._33) +
			mat._13 * (mat._24 * mat._31 - mat._21 * mat._34) +
			mat._14 * (mat._21 * mat._33 - mat._23 * mat._31);

		//	4行3列目
		m._43 = mat._11 * (mat._22 * mat._34 - mat._24 * mat._32) +
			mat._12 * (mat._24 * mat._31 - mat._21 * mat._34) +
			mat._14 * (mat._21 * mat._32 - mat._22 * mat._31);

		//	4行4列目
		m._44 = mat._11 * (mat._22 * mat._33 - mat._23 * mat._32) +
			mat._12 * (mat._23 * mat._31 - mat._21 * mat._33) +
			mat._13 * (mat._21 * mat._32 - mat._22 * mat._31);

		//	1 / |mat|
		float det = 1.0f / ((mat._11 * m._11) - (mat._21 * m._21) + (mat._31 * m._31) - (mat._41 * m._41));

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				m.Mat[i][j] *= det;
			}
		}

		return m;
	}

	Matrix Matrix::Scaling(float x, float y, float z)
	{
		Matrix m = Matrix::identity;
		m._11 = x;
		m._22 = y;
		m._33 = z;
		return m;
	}

	Matrix Matrix::Multiply(const Matrix& in1, const Matrix& in2)
	{
		Matrix m = Matrix::zero;
		//=========1行目
		m._11 = in1._11 * in2._11 + in1._12 * in2._21 + in1._13 * in2._31 + in1._14 * in2._41;
		m._12 = in1._11 * in2._12 + in1._12 * in2._22 + in1._13 * in2._32 + in1._14 * in2._42;
		m._13 = in1._11 * in2._13 + in1._12 * in2._23 + in1._13 * in2._33 + in1._14 * in2._43;
		m._14 = in1._11 * in2._14 + in1._12 * in2._24 + in1._13 * in2._34 + in1._14 * in2._44;
		//=========2行目
		m._21 = in1._21 * in2._11 + in1._22 * in2._21 + in1._23 * in2._31 + in1._24 * in2._41;
		m._22 = in1._21 * in2._12 + in1._22 * in2._22 + in1._23 * in2._32 + in1._24 * in2._42;
		m._23 = in1._21 * in2._13 + in1._22 * in2._23 + in1._23 * in2._33 + in1._24 * in2._43;
		m._24 = in1._21 * in2._14 + in1._22 * in2._24 + in1._23 * in2._34 + in1._24 * in2._44;
		//=========3行目 
		m._31 = in1._31 * in2._11 + in1._32 * in2._21 + in1._33 * in2._31 + in1._34 * in2._41;
		m._32 = in1._31 * in2._12 + in1._32 * in2._22 + in1._33 * in2._32 + in1._34 * in2._42;
		m._33 = in1._31 * in2._13 + in1._32 * in2._23 + in1._33 * in2._33 + in1._34 * in2._43;
		m._34 = in1._31 * in2._14 + in1._32 * in2._24 + in1._33 * in2._34 + in1._34 * in2._44;
		//=========4行目 
		m._41 = in1._41 * in2._11 + in1._42 * in2._21 + in1._43 * in2._31 + in1._44 * in2._41;
		m._42 = in1._41 * in2._12 + in1._42 * in2._22 + in1._43 * in2._32 + in1._44 * in2._42;
		m._43 = in1._41 * in2._13 + in1._42 * in2._23 + in1._43 * in2._33 + in1._44 * in2._43;
		m._44 = in1._41 * in2._14 + in1._42 * in2._24 + in1._43 * in2._34 + in1._44 * in2._44;

		return m;
	}

	Matrix Matrix::LookAt(const Vector3& position, const Vector3& target, const Vector3& up)
	{
		Vector3 za = target - position;
		
		//	pos-Targetを正規化
		za.Normalize();

		Vector3 xa = Vector3::Cross(up, za);
		xa.Normalize();

		Vector3 ya = Vector3::Cross(za, xa);
		ya.Normalize();

		Matrix m = {};
		m._11 = xa.x;
		m._12 = ya.x;
		m._13 = za.x;

		m._21 = xa.y;
		m._22 = ya.y;
		m._23 = za.y;

		m._31 = xa.z;
		m._32 = ya.z;
		m._33 = za.z;

		m._41 = -Vector3::Dot(xa, position);
		m._42 = -Vector3::Dot(ya, position);
		m._43 = -Vector3::Dot(za, position);
		m._44 = 1.0f;

		return m;
	}

	//	右手系透視変換
	Matrix Matrix::PerspectiveFovRH(float Fov, float Aspect, float Znear, float Zfar)
	{
		const float InvTangent = 1.0f / tanf(Fov * 0.5f);

		//	描画範囲
		Matrix m = {};

		m._11 = InvTangent / Aspect;

		m._22 = InvTangent;

		m._33 = Zfar / (Znear - Zfar);
		m._34 = -1.0;

		m._43 = (Znear * Zfar) / (Znear - Zfar);
		m._44 = 0;

		return m;
	}

	Matrix Matrix::PerspectiveFovLH(float Fov, float Aspect, float Znear, float Zfar)
	{
		const float InvTangent = 1 / tanf(Fov * 0.5f);

		//	描画範囲
		Matrix m = Matrix::identity;

		m._11 = InvTangent / Aspect;

		m._22 = InvTangent;

		m._33 = Zfar / (Zfar - Znear);
		m._34 = 1;

		m._43 = (-Znear * Zfar) / (Zfar - Znear);
		m._44 = 0;
		return m;
	}

	//	右手系平行投影変換
	Matrix Matrix::OrthoRH(float width, float height, float Znear, float Zfar)
	{
		Matrix m = Matrix::identity;

		m._11 = 2.0f / width;
		m._22 = 2.0f / height;
		m._33 = 1.0f / (Znear - Zfar);
		m._34 = Znear / (Znear - Zfar);

		return m;
	}

	Matrix Matrix::OrthoLH(float width, float height, float Znear, float Zfar)
	{
		Matrix m = Matrix::identity;

		m._11 = 2.0f / width;
		m._22 = 2.0f / height;
		m._33 = 1.0f / (Zfar - Znear);
		m._34 = -Znear / (Zfar - Znear);

		return m;
	}

	Matrix Matrix::YawPitchRoll(float x, float y, float z)
	{
		Matrix m = Matrix::zero;
		float sinx, cosx, siny, cosy, sinz, cosz;
		sinx = sinf(x); cosx = cosf(x);
		siny = sinf(y); cosy = cosf(y);
		sinz = sinf(z); cosz = cosf(z);

		//Z * X * Yの順
		m._11 = cosz * cosy + sinz * sinx * siny;
		m._12 = sinz * cosx;
		m._13 = -cosz * siny + sinz * sinx * cosy;

		m._21 = -sinz * cosy + cosz * sinx * siny;
		m._22 = cosz * cosx;
		m._23 = sinz * siny + cosz * sinx * cosy;

		m._31 = cosx * siny;
		m._32 = -sinx;
		m._33 = cosx * cosy;

		return m;
	}

	Matrix Matrix::RotationX(float Angle)
	{
		Matrix m = Matrix::zero;
		m._22 = cosf(Angle);
		m._23 = sinf(Angle);
		m._32 = -sinf(Angle);
		m._33 = cosf(Angle);
		return m;
	}

	Matrix Matrix::RotationY(float Angle)
	{
		Matrix m = Matrix::zero;
		m._11 = cosf(Angle);
		m._13 = -sinf(Angle);
		m._31 = sinf(Angle);
		m._33 = cosf(Angle);
		return m;
	}

	Matrix Matrix::RotationZ(float Angle)
	{
		Matrix m = Matrix::zero;
		m._11 = cosf(Angle);
		m._12 = sinf(Angle);
		m._21 = -sinf(Angle);
		m._22 = cosf(Angle);
		return m;
	}

	Matrix Matrix::AxsisRotation(const Vector3& axsis, float Angle)
	{
		Matrix m = Matrix::zero;
		m._11 = axsis.x * axsis.x * (1.0f - cosf(Angle)) + cosf(Angle);
		m._12 = axsis.x * axsis.y * (1.0f - cosf(Angle)) + axsis.z * sinf(Angle);
		m._13 = axsis.x * axsis.z * (1.0f - cosf(Angle)) - axsis.y * sinf(Angle);

		m._21 = axsis.x * axsis.y * (1.0f - cosf(Angle)) - axsis.z * sinf(Angle);
		m._22 = axsis.y * axsis.y * (1.0f - cosf(Angle)) + cosf(Angle);
		m._23 = axsis.y * axsis.z * (1.0f - cosf(Angle)) + axsis.x * sinf(Angle);

		m._31 = axsis.x * axsis.z * (1.0f - cosf(Angle)) + axsis.y * sinf(Angle);
		m._32 = axsis.y * axsis.z * (1.0f - cosf(Angle)) - axsis.x * sinf(Angle);
		m._33 = axsis.z * axsis.z * (1.0f - cosf(Angle)) + cosf(Angle);

		return m;
	}

	Matrix Matrix::Lerp(const Matrix& in1, const Matrix& in2, float t)
	{
		Matrix m = Matrix::zero;
		float u = 1.0f - t;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				m.Mat[i][j] = in1.Mat[i][j] * u + in2.Mat[i][j] * t;
			}
		}
		return m;
	}

	Matrix& Matrix::operator*=(const Matrix& m)
	{
		*this = Matrix::Multiply(*this, m);
		return *this;
	}

	Matrix Matrix::operator*(const Matrix& m) const
	{
		Matrix Mat = Matrix::Multiply(*this, m);
		return Mat;
	}

}
