//
#pragma once
#include <math.h>
#include "../Vector3/Vector3.h"

namespace Math
{
	struct Quaternion;

	struct Matrix
	{
		inline Matrix() 
			: _11(0.0f)
			, _12(0.0f)
			, _13(0.0f)
			, _14(0.0f)
			, _21(0.0f)
			, _22(0.0f)
			, _23(0.0f)
			, _24(0.0f)
			, _31(0.0f)
			, _32(0.0f)
			, _33(0.0f)
			, _34(0.0f)
			, _41(0.0f)
			, _42(0.0f)
			, _43(0.0f)
			, _44(0.0f)
		{}

		inline Matrix(float _11, float _12, float _13, float _14,
			float _21, float _22, float _23, float _24,
			float _31, float _32, float _33, float _34,
			float _41, float _42, float _43, float _44)
			: _11(_11)
			, _12(_12)
			, _13(_13)
			, _14(_14)
			, _21(_21)
			, _22(_22)
			, _23(_23)
			, _24(_24)
			, _31(_31)
			, _32(_32)
			, _33(_33)
			, _34(_34)
			, _41(_41)
			, _42(_42)
			, _43(_43)
			, _44(_44)
		{}


		union
		{
			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float Mat[4][4];
		};

		//	単位行列定数
		static const Matrix identity;

		//	zero
		static const Matrix zero;

		void Translate(const Vector3& position);

		void Update(const Vector3& position, const Vector3& rotation, const Vector3& scale);
		void Update(const Vector3& position, const Quaternion& rotation, const Vector3& scale);

		Vector3 GetRightVector()const;
		Vector3 GetUpVector()const;
		Vector3 GetForwardVector()const;
	public:

		//	単位行列か?
		static inline bool isIdentity(const Matrix& in)
		{
			return in._11 == 1 && in._12 == 0 && in._13 == 0 && in._14 == 0 &&
				in._21 == 0 && in._22 == 1 && in._23 == 0 && in._24 == 0 &&
				in._31 == 0 && in._32 == 0 && in._33 == 1 && in._34 == 0 &&
				in._41 == 0 && in._42 == 0 && in._43 == 0 && in._44 == 1;
		}

		//	行と列の変換
		static Matrix Transpose(const Matrix& matrix);

		//	逆行列
		static Matrix Inverse(const Matrix& mat);

		//	拡縮
		static Matrix Scaling(float x, float y, float z);

		//	乗算
		static Matrix Multiply(const Matrix& in1, const Matrix& in2);

		//	ビュー行列
		static Matrix LookAt(const Vector3& position, const Vector3& target, const Vector3& up);
		static Matrix LookAtRH(const Vector3& position, const Vector3& target, const Vector3& up);
		static Matrix LookAtLH(const Vector3& position, const Vector3& target, const Vector3& up);

		//	右手系透視変換
		static Matrix PerspectiveFovRH(float Fov, float Aspect, float Znear, float Zfar);
		//	左手系透視変換
		static Matrix PerspectiveFovLH(float Fov, float Aspect, float Znear, float Zfar);

		//	右手系平行投影変換
		static Matrix OrthoRH(float width, float height, float Znear, float Zfar);
		//	左手系平行投影変換
		static Matrix OrthoLH(float width, float height, float Znear, float Zfar);

		//	Y(Yaw),X(Picth),Z(Roll)回転行列
		static Matrix YawPitchRoll(float x, float y, float z);

		//	x軸回転行列
		static Matrix RotationX(float Angle);

		//	y軸回転行列
		static Matrix RotationY(float Angle);

		//	z軸回転行列
		static Matrix RotationZ(float Angle);

		//	任意軸回転行列
		static Matrix AxsisRotation(const Vector3& axsis, float Angle);

		//	線形補間
		static Matrix Lerp(const Matrix& in1, const Matrix& in2, float t);

		//	オペレーター
	public:

		inline Matrix& operator=(const Matrix& m)
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					Mat[i][j] = m.Mat[i][j];
				}
			}
			return *this;
		}
		
		Matrix& operator*=(const Matrix&);
		Matrix operator*(const Matrix&)const;
	};
}
