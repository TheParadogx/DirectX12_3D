#pragma once

#include"Math/Matrix/Matrix.h"
#include"math/Vector3/Vector3.h"

namespace Engine::System
{
	class Camera
	{
	public:
		Camera();
		virtual ~Camera() = default;

		static Camera* Main;

		bool Create();

		void Update(
			const Math::Vector3& Position,
			const Math::Vector3& Target,
			const Math::Vector3& Up
		);

		void SetFieldOfView(float Angle);
		void SetAspect(float Aspect);
		void SetAspect(float Width, float Height);
		void SetZ(float Near, float Far);

		const Math::Matrix& GetView()const;
		const Math::Matrix& GetProjection()const;

		const Math::Vector3& GetPosition()const;
		const Math::Vector3& GetRight();
		const Math::Vector3& GetUp()const;
		const Math::Vector3& GetForward()const;

	private:
		Math::Matrix mView;
		Math::Matrix mProjection;

		Math::Vector3 mPosition;
		Math::Vector3 mRight;
		Math::Vector3 mUp;
		Math::Vector3 mForward;

		float mFov;
		float mAspect;
		float mZNear;
		float mZFar;
	};
}

