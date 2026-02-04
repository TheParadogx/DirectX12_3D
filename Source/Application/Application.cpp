#include "pch.h"
#include "Application.hpp"
#include "Engine/Engine.hpp"

#include "Debug/spdlog/Logger.hpp"
#include "System/Window/Window.hpp"

#include "System/Clock/Clock.hpp"
#include "System/Input/Manager/InputManager.hpp"
#include"System/Scene/Manager/SceneManager.hpp"
#include "System/Entity/System/Manager/SystemManager.hpp"
#include"System/Entity/Manager/EntityManager.hpp"
#include"System/Scene/Default/DefaultScene.hpp"

#include "Audio/Resource/Manager/AudioResourceManager.hpp"
#include "Audio/Manager/AudioManager.hpp"

#include"Application/Debug/AppDebug/AppDebug.hpp"
#include"Application/Macro/ProjMacros.hpp"

#include"Scene/Title/TitleScene.hpp"
#include"Scene/StageSelect/StageSelectScene.hpp"
#include"Scene/InGame/InGameScene.hpp"

#include"Graphics/Effect/Manager/EffectManager.hpp"

void Engine::App::Application::CreateStartScene()
{

#if START_SCENE == 0
    mScene->ChangeScene<Scene::Title>();

#elif START_SCENE == 1
    mScene->ChangeScene<Scene::StageSelect>();


#elif START_SCENE == 2
    mScene->ChangeScene<Scene::InGame>();

#endif // START_SCENE
}

/// <summary>
/// メインループ
/// </summary>
void Engine::App::Application::Run()
{
    System::Window* window = System::Window::GetInstance();

	while (window->IsQuitMessage() == false)
	{
        window->IsUpdateMessage();
        mEngine->NewFrame();
        mEngine->BegineRendering();

        this->Tick();
        mEngine->EndFrame();
        mScene->PostPresentUpdate();
    }
}

/// <summary>
/// 初期化
/// </summary>
/// <returns>true:成功</returns>
bool Engine::App::Application::Initialize()
{
    bool ret = false;

    //  Engine
    Core::Engine::Create();
    mEngine = Core::Engine::GetInstance();
    ret = mEngine->Initialize();
    if (ret == false)
    {
        LOG_CRITICAL("エンジンの初期化に失敗");
        return false;
    }

    mEffect = Graphics::EffectManager::GetInstance();

    //  システム管理
    System::SystemManager::Create();
    mSystems = System::SystemManager::GetInstance();

    //  Entity管理
    mEntitys = System::EntityManager::GetInstance();

    //  Scene管理
    System::SceneManager::Create();
    mScene = System::SceneManager::GetInstance();
    CreateStartScene();
    //mScene->ChangeScene<System::DefaultScene>();
    ret = mScene->Initialize();
    if (ret == false)
    {
        LOG_CRITICAL("Failed Initilize SceneManager");
        return false;
    }

    return true;
}

/// <summary>
/// 終了処理
/// </summary>
void Engine::App::Application::Shutdown()
{
    mEngine->Release();
}

/// <summary>
/// １フレームの更新処理
/// </summary>
void Engine::App::Application::Tick()
{
    auto Scene = System::SceneManager::GetInstance();

    if (mIsRunning == true)
    {
        //  時間更新
        System::Time dt = mClock.Restart();
        auto seconds = dt.AsSeconds();

        //  完全にフレームが飛んだ時の保険
        if (seconds > 0.1)
        {
            seconds = 0.1;
        }

        //  事前更新
        this->PreUpdate(seconds);
        this->ImGuiUpdate();

        //  メイン更新(物理)
        this->MainUpdate(seconds);

        //  事後更新
        this->PostUpdate(seconds);

        //  描画
        this->Render();

    }

}



/// <summary>
/// 可変更新
/// </summary>
void Engine::App::Application::PreUpdate(double dt)
{
    mSystems->PreUpdate(mEntitys->GetRegistry(), dt);
}

/// <summary>
/// 固定更新
/// </summary>
void Engine::App::Application::MainUpdate(double dt)
{
    mSystems->MainUpdate(mEntitys->GetRegistry(), dt);
    mScene->Update(dt);
    mEffect->Update(dt);
}

/// <summary>
/// 事後更新
/// </summary>
void Engine::App::Application::PostUpdate(double dt)
{
    mSystems->PostUpdate(mEntitys->GetRegistry(), dt);
    mScene->PostUpdate(dt);
    Debug::AppDebugUI::UpdateImGui();
}

/// <summary>
/// 描画
/// </summary>
void Engine::App::Application::Render()
{
    mScene->Render();
    mSystems->Render(mEntitys->GetRegistry());

    mEffect->Draw();

    GET_INPUT_MANAGER->Update();
    mScene->FadeRender();
}

/// <summary>
/// ImGUiの更新、表示
/// </summary>
void Engine::App::Application::ImGuiUpdate()
{
    GET_INPUT_MANAGER->GetPadManager()->ImGuiUpdate();
    mScene->ImGuiUpdate();
}
