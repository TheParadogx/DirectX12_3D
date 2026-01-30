#pragma once
#include"../SpriteComponent.hpp"


namespace Engine::System
{
	class SpriteRenderSystem
	{
	public:
		/// <summary>
		/// À•W‚Ì“¯Šú
		/// </summary>
		/// <param name="Reg"></param>
		static void Update(entt::registry& Reg);

		/// <summary>
		/// •`‰æ
		/// </summary>
		/// <param name="Reg"></param>
		static void Render(entt::registry& Reg);
	};
}

