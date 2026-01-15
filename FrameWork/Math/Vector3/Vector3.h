//
#pragma once
#include <math.h>
namespace Math
{
	struct Matrix;
	struct Matrix3x3;

	struct Vector3
	{
		inline Vector3()noexcept :x(0.0f), y(0.0f), z(0.0f) {}

		inline Vector3(float x, float y, float z)noexcept : x(x), y(y), z(z) {}

		inline Vector3(const Vector3& v)noexcept : x(v.x), y(v.y), z(v.z) {}

		union
		{
			struct
			{
				float x, y, z;
			};
			float value[3];
		};

		//	upベクトル(0,1,0)
		static const Vector3 Up;

		//	downベクトル(0,-1,0)
		static const Vector3 Down;

		//	forwardベクトル(0,0,1)
		static const Vector3 Forward;

		//	backベクトル(0,0,-1)
		static const Vector3 Back;

		//	rightベクトル(1,0,0)
		static const Vector3 Right;

		//	leftベクトル(-1,0,0)
		static const Vector3 Left;

		//	zeroベクトル(0,0,0)
		static const Vector3 Zero;

		//	oneベクトル(1,1,1)
		static const Vector3 One;

		/// <summary>
		/// ベクトルの長さを求める
		/// </summary>
		/// <param name="v">ベクトル</param>
		/// <returns>ベクトルの長さ</returns>
		static inline float Length(const Vector3& v) { return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z); }

		/// <summary>
		/// ベクトルの長さを求める
		/// </summary>
		/// <returns>ベクトルの長さ</returns>
		inline float Length()const { return Length(*this); }

		/// <summary>
		/// 2乗の長さを求める
		/// </summary>
		/// <param name="v">ベクトル</param>
		/// <returns>平方根を取らない長さ</returns>
		static inline float SqrLength(const Vector3& v)noexcept { return v.x * v.x + v.y * v.y + v.z * v.z; }

		/// <summary>
		/// 2乗の長さを求める
		/// </summary>
		/// <returns>平方根を取らない長さ</returns>
		inline float SqrLength()const noexcept { return SqrLength(*this); }
		
		/// <summary>
		/// ベクトルの正規化
		/// </summary>
		/// <param name="v">ベクトル</param>
		/// <returns>ゼロベクトルか正規化されたベクトル</returns>
		static inline Vector3 Normalize(const Vector3& v)
		{
			const float l = SqrLength(v);
			if (l != 0.0f && l != 1.0f)
			{
				const float d = 1.0f / sqrtf(l);
				const Vector3 ret = { v.x * d,v.y * d, v.z * d };
				return ret;
			}
			//	ゼロベクトルか正規化済みベクトル
			return v;
		}

		/// <summary>
		/// ベクトルの正規化
		/// </summary>
		inline void Normalize()
		{
			*this = Normalize(*this);
		}

		/// <summary>
		/// 内積
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <returns>内積</returns>
		static inline float Dot(const Vector3& a, const Vector3& b)noexcept
		{
			return a.x * b.x + a.y * b.y + a.z * b.z;
		}

		/// <summary>
		/// 外積
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <returns>外積</returns>
		static Vector3 Cross(const Vector3& a, const Vector3& b)noexcept;

		/// <summary>
		/// 2つのベクトルの線形補間
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <param name="t">補完割合(0 ~ 1.0)</param>
		/// <returns>補完されたベクトル</returns>
		static Vector3 Lerp(const Vector3& a, const Vector3& b, float t);

		/// <summary>
		/// 2つのベクトルの距離を返す
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <returns>2つのベクトルの距離</returns>
		static inline float Distance(const Vector3& a, const Vector3& b)
		{
			return Vector3::Length(b - a);
		}

		/// <summary>
		/// 反射ベクトルの計算
		/// </summary>
		/// <param name="Direction"></param>
		/// <param name="NormalizedNormal">正規化された法線</param>
		/// <returns>反射ベクトル</returns>
		static Vector3 Reflect(const Vector3& Direction, const Vector3& NormalizedNormal);
		
		//	ベクトルに行列を掛け合わせる
		static Vector3 TransformCoord(const Vector3& a, const Matrix& m);

		//	ベクトルに行列を掛け合わせる
		static Vector3 TransformNormal(const Vector3& a, const Matrix& m);

		//	ベクトルに行列を掛け合わせる
		static Vector3 TransformNormal(const Vector3& a, const Matrix3x3& m);
	public:

		inline float& operator[](const int index)
		{
			return value[index];
		}
		inline float operator[](const int index)const
		{
			return value[index];
		}

		inline Vector3& operator=(const Vector3& a)
		{
			x = a.x;
			y = a.y;
			z = a.z;
			return *this;
		}
		inline Vector3& operator+=(const Vector3& a)
		{
			x += a.x;
			y += a.y;
			z += a.z;
			return *this;
		}
		inline Vector3& operator-=(const Vector3& a)
		{
			x -= a.x;
			y -= a.y;
			z -= a.z;
			return *this;
		}

		inline Vector3 operator+(const Vector3& a)const
		{
			return Vector3(x + a.x, y + a.y, z + a.z);
		}
		inline Vector3 operator-(const Vector3& a)const
		{
			return Vector3(x - a.x, y - a.y, z - a.z);
		}

		inline Vector3 operator+()const
		{
			return Vector3(+x, +y, +z);
		}
		inline Vector3 operator-()const
		{
			return Vector3(-x, -y, -z);
		}

		inline Vector3& operator*=(const float a)
		{
			x *= a;
			y *= a;
			z *= a;
			return *this;
		}
		inline Vector3& operator/=(const float a)
		{
			const float v = 1.0f / a;
			x *= v;
			y *= v;
			z *= v;
			return *this;
		}

		inline Vector3 operator*(const float a)const
		{
			return Vector3(x * a, y * a, z * a);
		}
		inline Vector3 operator/(const float a)const
		{
			const float f = 1.0f / a;
			return Vector3(x * f, y * f, z * f);
		}
	};
}
