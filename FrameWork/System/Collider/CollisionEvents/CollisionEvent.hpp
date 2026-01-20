#pragma once
#include<vector>
#include<Plugin/entt/entt.hpp>

namespace Engine::System
{
	struct CollisionEvent
	{
		/// <summary>
		/// “–‚½‚Á‚Ä‚«‚½‘Šè‚ÌƒRƒŒƒNƒVƒ‡ƒ“
		/// </summary>
		std::vector<entt::entity> HitEntitys;
	};
}