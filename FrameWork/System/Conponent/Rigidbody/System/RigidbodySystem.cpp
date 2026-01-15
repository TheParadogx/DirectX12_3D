#include"pch.h"
#include"RigidbodySystem.hpp"

#include"System/Conponent/Rigidbody/RigidbodyComponent.hpp"
#include"System/Conponent/Transform/TransformConponent.hpp"

namespace Engine::System
{
	/// <summary>
	/// 重力と座標更新
	/// </summary>
	/// <param name="Reg"></param>
	/// <param name="FixedDeltaTime"></param>
	void RigidBodySystem::MainUpdate(entt::registry& Reg, double FixedDeltaTime)
	{
		auto view = Reg.view<Transform3D, Rigidbody3D>();
		for (auto [entity, trans, rigid] : view.each())
		{
			trans.Position += rigid.Velocity * FixedDeltaTime;
		}
	}
}