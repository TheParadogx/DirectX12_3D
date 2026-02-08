#include "pch.h"
#include "GameClearFactory.hpp"
#include"System/Entity/Manager/EntityManager.hpp"
#include"System/Conponent/Transform/TransformConponent.hpp"
#include"Graphics/Texture/Manager/TextureManager.hpp"
#include"System/Conponent/Sprite/SpriteComponent.hpp"
#include"System/Window/Window.hpp"

#include"System/Conponent/Sprite/Glow/GlowComponents.hpp"


void Engine::Scene::GameClearFactory::CreateBG()
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
	auto res = Graphics::TextureManager::GetInstance()->Load("Assets/GameClear/Texture/BG.png");
	auto& sprite = registry.emplace<SpriteComponent>(entity, res);
	float color = 1.0f;
	sprite.Sprite.SetColor({ color,color,color,1 });
	auto window = Window::GetInstance();
	sprite.Sprite.SetSize({ (float)window->GetWidth(),(float)window->GetHeight() });

	auto& glow = registry.emplace<GlowComponent>(entity);
	glow.BaseIntensity = 1.0f;

	auto& liner = registry.emplace<LinearShiftTag>(entity);
	liner.StartIntensity = 5.0f;
	liner.EndIntensity = 1.0f;
	liner.Duration = 5.0f;
	liner.AutoDestroy = true;

	auto& pulse = registry.emplace<PulseTag>(entity);
	pulse.Duration = 7.0f;
	pulse.Range = 1.5f;

}
