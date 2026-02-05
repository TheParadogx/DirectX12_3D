#pragma once
#include"entt/entt.hpp"

namespace Engine::Scene
{
	class GameOverFactory
	{
	public:
		//	”wŒi
		static void CreateBG();

		//	Titleƒ{ƒ^ƒ“
		static entt::entity CreateTitle();

		//	Retryƒ{ƒ^ƒ“
		static entt::entity CreateRetry();

	};

}