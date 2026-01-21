#include "pch.h"
#include "SceneManager.hpp"

#include"Graphics/DirectX/DirectX12.hpp"
#include"System/Entity/System/Manager/SystemManager.hpp"

/// <summary>
///	初期化
/// </summary>
/// <returns></returns>
bool Engine::System::SceneManager::Initialize()
{
	if (mScene != nullptr)
	{
		return mScene->Initialize();
	}
	return true;
}

/// <summary>
/// 可変更新
/// </summary>
/// <param name="DeltaTime"></param>
void Engine::System::SceneManager::Update(double DeltaTime)
{
	if (mScene != nullptr)
	{
		mScene->Update(DeltaTime);
	}
}

/// <summary>
/// 固定更新
/// </summary>
void Engine::System::SceneManager::PostUpdate(double  FixedDeltaTime)
{
	if (mScene != nullptr)
	{
		mScene->PostUpdate(FixedDeltaTime);
	}
}

/// <summary>
/// ImGuiの更新表示
/// </summary>
void Engine::System::SceneManager::ImGuiUpdate()
{
	if (mScene != nullptr)
	{
		mScene->ImGuiUpdate();
	}
}

/// <summary>
/// 描画
/// </summary>
void Engine::System::SceneManager::Render()
{
	if (mScene != nullptr)
	{
		mScene->Render();
	}
}

/// <summary>
/// 次のSceneがある場合に切り替える
/// </summary>
void Engine::System::SceneManager::PostPresentUpdate()
{
	if (mNextScene == nullptr)
	{
		return;
	}

	Graphics::DirectX::GetInstance()->WaitForGPU();
	SystemManager::GetInstance()->AllClearSystem();

	if (mScene != nullptr)
	{
		mScene->Release();
		mScene.reset();
	}

	mScene = std::move(mNextScene);
	mNextScene = nullptr;

	mScene->Initialize();
}

/// <summary>
/// 今のSceneを読み込み直す
/// </summary>
void Engine::System::SceneManager::ReloadCurrentScene()
{
	mNextScene = mReloader();
}

std::string Engine::System::SceneManager::CurrentSceneName()
{
	if (mScene == nullptr)
	{
		return "None";
	}

	return mScene->GetSceneName();
}
