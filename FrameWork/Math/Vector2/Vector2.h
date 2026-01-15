//
#pragma once
#include <math.h>

namespace Math
{
	//	2次元ベクトル
	struct Vector2
	{
		inline Vector2()
			: x(0.0f)
			, y(0.0f)
		{}

		inline Vector2(float x,float y)
			: x(x)
			, y(y)
		{}

		inline Vector2(const Vector2& v)
			: x(v.x)
			, y(v.y)
		{}

		union
		{
			struct
			{
				float x;
				float y;
			};
			float value[2];
		};

		//	downベクトル
		static const Vector2 Down;

		//	upベクトル
		static const Vector2 Up;

		//	rightベクトル
		static const Vector2 Right;

		//	leftベクトル
		static const Vector2 Left;

		//	zeroベクトル
		static const Vector2 Zero;

		//	oneベクトル
		static const Vector2 One;

		//	長さを求める
		inline float Length()const
		{
			return sqrtf(x * x + y * y);
		}

		//	2乗の長さを求める
		inline float SqrLength()const
		{
			return x * x + y * y;
		}

		//	正規化
		inline void Normalize()
		{
			const float l = Length();
			if (l != 0.0f)
			{
				const float d = 1.0f / l;
				x *= d;
				y *= d;
			}
		}

	public:
		
		Vector2 operator+(const Vector2& other) const {
			return { x + other.x, y + other.y };
		}

		inline float& operator[](unsigned int num)
		{
			return value[num];
		}

		inline Vector2& operator+=(const Vector2& a)
		{
			x += a.x;
			y += a.y;
			return *this;
		}

		inline Vector2& operator-=(const Vector2& a)
		{
			x -= a.x;
			y -= a.y;
			return *this;
		}

		inline Vector2 operator-(const Vector2& a)const
		{
			return Vector2(x - a.x, y - a.y);
		}

		inline Vector2 operator+()const
		{
			return Vector2(+x, +y);
		}

		inline Vector2 operator-()const
		{
			return Vector2(-x, -y);
		}

	};
}