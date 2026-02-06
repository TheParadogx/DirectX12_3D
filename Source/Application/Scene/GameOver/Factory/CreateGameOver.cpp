#include "pch.h"
#include "CreateGameOver.hpp"
#include"System/Entity/Manager/EntityManager.hpp"
#include"System/Conponent/Transform/TransformConponent.hpp"
#include"Graphics/Texture/Manager/TextureManager.hpp"
#include"System/Conponent/Sprite/SpriteComponent.hpp"
#include"System/Window/Window.hpp"
#include"System/Conponent/Sprite/Glow/GlowComponents.hpp"
#include"System/Conponent/Sprite/Scale/ScaleComponent.hpp"

namespace Engine::Scene
{
	void GameOverFactory::CreateBG()
	{
		using namespace System;
		auto manager = EntityManager::GetInstance();
		auto& registry = manager->GetRegistry();

		auto entity = manager->CreateEntity();

		//	ç¿ïW
		auto& trans = registry.emplace<Transform2D>(entity);
		trans.Position = { 0,0 };
		trans.Scale = { 1.0f,1.0f };

		//	sprite
		auto res = Graphics::TextureManager::GetInstance()->Load("Assets/GameOver/Texture/BG.png");
		auto& sprite = registry.emplace<SpriteComponent>(entity, res);
		float color = 2.0f;
		sprite.Sprite.SetColor({ color,color,color,1 });
		auto window = Window::GetInstance();
		sprite.Sprite.SetSize({ (float)window->GetWidth(),(float)window->GetHeight() });

		auto& glow = registry.emplace<GlowComponent>(entity);
		glow.BaseIntensity = 1.0f;

		auto& liner = registry.emplace<LinearShiftTag>(entity);
		liner.StartIntensity = 10.0f;
		liner.EndIntensity = 1.0f;
		liner.Duration = 3.0f;
		liner.AutoDestroy = true;

		auto& pulse = registry.emplace<PulseTag>(entity);
		pulse.Duration = 5.0f;
		pulse.Range = 2.0f;


	}
	entt::entity GameOverFactory::CreateTitle()
	{
		using namespace System;
		auto manager = EntityManager::GetInstance();
		auto& registry = manager->GetRegistry();

		auto entity = manager->CreateEntity();
		auto window = System::Window::GetInstance();

		//	ç¿ïW
		auto& trans = registry.emplace<Transform2D>(entity);
		Math::Vector2 Pos = { (float)window->GetWidth() / 3,(float)window->GetHeight() / 5 * 4 };
		trans.Position = { Pos };
		float scale = 0.63f;
		trans.Scale = { scale ,scale };


		//	sprite
		auto res = Graphics::TextureManager::GetInstance()->Load("Assets/GameOver/Texture/TitleButton.png");
		auto& sprite = registry.emplace<SpriteComponent>(entity, res);
		float color = 2.0f;
		sprite.Sprite.SetPivot({ 0.5,0.5 });
		sprite.Sprite.SetColor({ color,color,color,1 });


		auto& glow = registry.emplace<GlowComponent>(entity);
		glow.BaseIntensity = 1.0f;

		auto& liner = registry.emplace<LinearShiftTag>(entity);
		liner.StartIntensity = 10.0f;
		liner.EndIntensity = 1.0f;
		liner.Duration = 3.0f;
		liner.AutoDestroy = true;

		auto& pulse = registry.emplace<PulseTag>(entity);
		pulse.Duration = 7.0f;
		pulse.Range = 1.0f;


		return entity;

	}
	entt::entity GameOverFactory::CreateRetry()
	{
		using namespace System;
		auto manager = EntityManager::GetInstance();
		auto& registry = manager->GetRegistry();

		auto entity = manager->CreateEntity();
		auto window = System::Window::GetInstance();

		//	ç¿ïW
		auto& trans = registry.emplace<Transform2D>(entity);
		Math::Vector2 Pos = { (float)window->GetWidth() / 3 * 2,(float)window->GetHeight() /5 * 4};
		trans.Position = { Pos };
		float scale = 0.7f;
		trans.Scale = { scale ,scale };


		//	sprite
		auto res = Graphics::TextureManager::GetInstance()->Load("Assets/GameOver/Texture/RetryButton.png");
		auto& sprite = registry.emplace<SpriteComponent>(entity, res);
		float color = 2.0f;
		sprite.Sprite.SetPivot({ 0.5,0.5 });
		sprite.Sprite.SetColor({ color,color,color,1 });


		auto& glow = registry.emplace<GlowComponent>(entity);
		glow.BaseIntensity = 1.0f;

		auto& liner = registry.emplace<LinearShiftTag>(entity);
		liner.StartIntensity = 10.0f;
		liner.EndIntensity = 1.0f;
		liner.Duration = 3.0f;
		liner.AutoDestroy = true;

		auto& pulse = registry.emplace<PulseTag>(entity);
		pulse.Duration = 7.0f;
		pulse.Range = 1.0f;

		return entity;

	}
}