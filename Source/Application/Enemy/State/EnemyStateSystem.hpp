#pragma once
#include"System/Entity/System/Base/ISystem.hpp"

namespace Engine::System
{
	class EnemyStateSystem : public ISystem
	{
	public:

		/// <summary>
		/// ó‘Ô‚ÌI—¹‚Ì”»’è
		/// </summary>
		/// <param name="Reg"></param>
		/// <param name="DeltaTime"></param>
		void PreUpdate(entt::registry& Reg, double DeltaTime)override;

		/// <summary>
		/// ó‘Ô‚Ì‘JˆÚ
		/// </summary>
		/// <param name="Reg"></param>
		/// <param name="DeltaTime"></param>
		void MainUpdate(entt::registry& Reg, double DeltaTime)override;


	};

}

