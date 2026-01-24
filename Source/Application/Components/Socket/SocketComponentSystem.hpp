#pragma once

#include"System/Entity/System/Base/ISystem.hpp"

namespace Engine::System
{
	class SocketComponentSystem : public ISystem
	{
	public:
		/// <summary>
		/// 座標位置が確定した親オブジェクトの指定したボーン座標にセット
		/// </summary>
		virtual void PostUpdate(entt::registry& Reg, double DeltaTime)override;

	private:
		/// <summary>
		/// ImGuiでオフセットをいじるための
		/// </summary>
		/// <param name="Reg"></param>
		void OnGui(entt::registry& Reg);
	};
}

