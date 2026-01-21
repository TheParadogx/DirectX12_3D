#include "pch.h"
#include "AppDebug.hpp"

#include"System/Scene/Manager/SceneManager.hpp"
#include"Application/Scene/Title/TitleScene.hpp"
#include"Application/Scene/InGame/InGameScene.hpp"

/// <summary>
/// Scene関係のデバック
/// </summary>
void Engine::Debug::AppDebugUI::SceneDebug()
{
	System::SceneManager* scene = System::SceneManager::GetInstance();
	if (scene == nullptr) return;


	// Sceneの作成・切り替え
	ImGui::Begin("Scene");

	//	今
	ImGui::Text(scene->CurrentSceneName().c_str());
	ImGui::Separator();

	if (ImGui::Button("Title"))
	{
		scene->ChangeScene<Scene::Title>();
	}

	ImGui::SameLine();

	if (ImGui::Button("InGame"))
	{
		scene->ChangeScene<Scene::InGame>();
	}

	ImGui::Separator();

	if (ImGui::Button("Reload"))
	{
		scene->ReloadCurrentScene();
	}

	ImGui::End();
}

/// <summary>
/// 全てのDebug操作の状態更新
/// </summary>
void Engine::Debug::AppDebugUI::UpdateImGui()
{
	SceneDebug();
}
