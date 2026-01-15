//
#include "Matrix3x3.h"

namespace Math
{
	//	単位行列定数
	const Matrix3x3 Matrix3x3::identity =
	{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	//	zero
	const Matrix3x3 Matrix3x3::zero =
	{
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};
	bool Matrix3x3::isIdentity(const Matrix3x3& in)
	{
		return in._11 == 1.0f && in._12 == 0.0f && in._13 == 0.0f
			&& in._21 == 0.0f && in._22 == 1.0f && in._23 == 0.0f
			&& in._31 == 0.0f && in._32 == 0.0f && in._33 == 1.0f;
	}
}
