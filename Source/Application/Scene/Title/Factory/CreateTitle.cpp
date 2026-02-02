#include "pch.h"
#include "CreateTitle.hpp"

#include"System/Entity/Manager/EntityManager.hpp"
#include"System/Conponent/Transform/TransformConponent.hpp"
#include"Graphics/Texture/Manager/TextureManager.hpp"
#include"System/Conponent/Sprite/SpriteComponent.hpp"
#include"System/Window/Window.hpp"

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
	auto res = Graphics::TextureManager::GetInstance()->Load("Assets/Title/Texture/TitleBG.png");
	auto& sprite = registry.emplace<SpriteComponent>(entity, res);
	auto window = Window::GetInstance();
	sprite.Sprite.SetSize({ (float)window->GetWidth(),(float)window->GetHeight()});
}

/// <summary>
/// タイトルのロゴの作成
/// </summary>
void Engine::Scene::CreateTitleObject::CreateLogo()
{
	using namespace System;
	auto manager = EntityManager::GetInstance();
	auto& registry = manager->GetRegistry();
	auto window = Window::GetInstance();

	auto entity = manager->CreateEntity();

	//	座標
	auto& trans = registry.emplace<Transform2D>(entity);
	trans.Position = { window->GetWidth() / 2.0f ,(float)window->GetHeight()  / 3};	//	とりま100
	trans.Scale = { 1.0f,1.0f };

	////	sprite
	auto res = Graphics::TextureManager::GetInstance()->Load("Assets/Title/Texture/TitleLogo.png");
	auto& sprite = registry.emplace<SpriteComponent>(entity, res);
	sprite.Sprite.SetColor({ 1,1,1,1 });
	sprite.Sprite.SetPivot({ 0.5,0.5 });

}
