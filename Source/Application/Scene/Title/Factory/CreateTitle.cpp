#include "pch.h"
#include "CreateTitle.hpp"

#include"System/Entity/Manager/EntityManager.hpp"
#include"System/Conponent/Transform/TransformConponent.hpp"
#include"Graphics/Texture/Manager/TextureManager.hpp"
#include"System/Conponent/Sprite/SpriteComponent.hpp"
#include"System/Window/Window.hpp"
#include"System/Conponent/Sprite/Glow/GlowComponents.hpp"

void Engine::Scene::CreateTitleObject::CreateBG()
{
	using namespace System;
	auto manager = EntityManager::GetInstance();
	auto& registry = manager->GetRegistry();

	auto entity = manager->CreateEntity();

	//	座標
	auto& trans = registry.emplace<Transform2D>(entity);
	trans.Position = { 0,0 };
	trans.Scale = { 1.0f,1.0f };

	//	sprite
	auto res = Graphics::TextureManager::GetInstance()->Load("Assets/Title/Texture/Title.png");
	auto& sprite = registry.emplace<SpriteComponent>(entity, res);
	auto window = Window::GetInstance();
	sprite.Sprite.SetSize({ (float)window->GetWidth(),(float)window->GetHeight()});
	sprite.Sprite.SetColor(Graphics::Color::White());
	//sprite.Sprite.SetIntensity(10.0f);

	//	glow
	auto& glow = registry.emplace<GlowComponent>(entity);
	glow.BaseIntensity = 1.0f;

	auto& liner = registry.emplace<LinearShiftTag>(entity);
	liner.StartIntensity = 10.0f;
	liner.EndIntensity = 1.0f;
	liner.Duration = 5.0f;
	liner.AutoDestroy = true;
	
	auto& pulse = registry.emplace<PulseTag>(entity);
	pulse.Duration = 7.0f;
	pulse.Range = 1.0f;
}

/// <summary>
/// タイトルのロゴの作成
/// </summary>
void Engine::Scene::CreateTitleObject::CreateGuide()
{
	using namespace System;
	auto manager = EntityManager::GetInstance();
	auto& registry = manager->GetRegistry();
	auto window = Window::GetInstance();

	auto entity = manager->CreateEntity();

	//	座標
	auto& trans = registry.emplace<Transform2D>(entity);
	trans.Position = { window->GetWidth() / 2.0f ,(float)window->GetHeight()  / 5 * 4};
	float scale = 0.6f;
	trans.Scale = { scale,scale };

	////	sprite
	auto res = Graphics::TextureManager::GetInstance()->Load("Assets/Title/Texture/Guide.png");
	auto& sprite = registry.emplace<SpriteComponent>(entity, res);
	sprite.Sprite.SetColor({ 1,1,1,1 });
	sprite.Sprite.SetPivot({ 0.5,0.5 });


}

