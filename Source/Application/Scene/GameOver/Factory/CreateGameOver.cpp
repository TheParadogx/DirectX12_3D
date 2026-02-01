#include "pch.h"
#include "CreateGameOver.hpp"
#include"System/Entity/Manager/EntityManager.hpp"
#include"System/Conponent/Transform/TransformConponent.hpp"
#include"Graphics/Texture/Manager/TextureManager.hpp"
#include"System/Conponent/Sprite/SpriteComponent.hpp"
#include"System/Window/Window.hpp"


namespace Engine::Scene
{
	void GameOverFactory::CreateBG()
	{
		using namespace System;
		auto manager = EntityManager::GetInstance();
		auto& registry = manager->GetRegistry();

		auto entity = manager->CreateEntity();

		//	À•W
		auto& trans = registry.emplace<Transform2D>(entity);
		trans.Position = { 0,0 };
		trans.Scale = { 1.0f,1.0f };

		//	sprite
		auto res = Graphics::TextureManager::GetInstance()->Load("Assets/GameOver/Texture/GameOverBG.png");
		auto& sprite = registry.emplace<SpriteComponent>(entity, res);
		float color = 2.0f;
		sprite.Sprite.SetColor({ color,color,color,1 });
		auto window = Window::GetInstance();
		sprite.Sprite.SetSize({ (float)window->GetWidth(),(float)window->GetHeight() });

	}
}