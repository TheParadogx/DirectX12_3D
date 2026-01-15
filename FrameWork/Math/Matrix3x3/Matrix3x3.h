//
#pragma once

namespace Math
{
	struct Matrix3x3
	{
		inline Matrix3x3() 
			: _11(0.0f)
			, _12(0.0f)
			, _13(0.0f)
			, _21(0.0f)
			, _22(0.0f)
			, _23(0.0f)
			, _31(0.0f)
			, _32(0.0f)
			, _33(0.0f)
		{}

		inline Matrix3x3(float _11, float _12, float _13,
			float _21, float _22, float _23,
			float _31, float _32, float _33)
			: _11(_11)
			, _12(_12)
			, _13(_13)
			, _21(_21)
			, _22(_22)
			, _23(_23)
			, _31(_31)
			, _32(_32)
			, _33(_33)
		{}

		union
		{
			struct
			{
				float _11, _12, _13;
				float _21, _22, _23;
				float _31, _32, _33;
			};
			float Mat[3][3];
		};

		//	単位行列定数
		static const Matrix3x3 identity;

		//	zero
		static const Matrix3x3 zero;

	public:
		//	単位行列か?
		static bool isIdentity(const Matrix3x3& in);
	};
}
