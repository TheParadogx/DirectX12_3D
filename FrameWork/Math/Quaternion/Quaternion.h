//
#pragma once
#include"Math/Vector3/Vector3.h"

namespace Math
{
	struct Matrix;
	struct Matrix3x3;

	struct Quaternion
	{
		inline Quaternion()
			: x(0.0f)
			, y(0.0f)
			, z(0.0f)
			, w(0.0f)
		{}

		inline Quaternion(float x,float y,float z,float w)
			: x(x)
			, y(y)
			, z(z)
			, w(w)
		{}

		//	単位回転
		static const Quaternion Identity;

		union
		{
			struct
			{
				float x, y, z, w;
			};
			float value[4];
		};

		//	行列に変換
		Matrix ToMatrix()const;

		//	行列に変換
		Matrix3x3 ToMatrix3x3()const;

	public:
		//	行列から回転の抽出
		static Quaternion FromMatrix(const Matrix& m);

		//	クォータニオンの掛け算
		static Quaternion Multiply(const Quaternion& p, const Quaternion& q);


		//	軸回転を作成
		static Quaternion AngleAxis(float angle, const Vector3& axis);

		//	内積
		static inline float Dot(const Quaternion& a, const Quaternion& b)
		{
			return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
		}

		//	線形補間
		static Quaternion Lerp(const Quaternion& a, const Quaternion& b,float t);

		//	球面線形補間
		static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t);

		/// <summary>
		/// 指定した方向を向くクォータニオンを生成する
		/// </summary>
		/// <param name="Direction">向きたい方向ベクトル</param>
		/// <returns>回転クォータニオン</returns>
		static Quaternion LookRotation(const Vector3& Direction, const Vector3& UpAxis = Vector3::Forward);


		//	オペレーター
	public:
		inline float& operator[](unsigned int num)
		{
			return value[num];
		}

		inline Quaternion& operator*=(const Quaternion& a)
		{
			*this = Multiply(*this, a);
			return	*this;
		}

		inline Quaternion operator*(const Quaternion& a)const
		{
			const Quaternion q = Multiply(*this, a);
			return q;
		}


	};
}
