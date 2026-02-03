#include "pch.h"
#include "TitleScene.hpp"

#include"System/Input/Manager/InputManager.hpp"
#include"System/Scene/Manager/SceneManager.hpp"
#include"System/Entity/Manager/EntityManager.hpp"

#include"Application/Scene/InGame/InGameScene.hpp"
#include"Factory/CreateTitle.hpp"
#include"Application/Scene/StageSelect/StageSelectScene.hpp"

/// <summary>
///	初期化
/// </summary>
/// <returns></returns>
bool Engine::Scene::Title::Initialize()
{


    ////  ロゴ
    //CreateTitleObject::CreateLogo();
    ////  エフェクト
    //CreateTitleObject::CreateEffect();
    //  背景
    CreateTitleObject::CreateBG();

    return true;
}

/// <summary>
/// 画面の切り替え判定
/// </summary>
/// <param name="FixedDeltaTime"></param>
void Engine::Scene::Title::PostUpdate(double FixedDeltaTime)
{
    if (GET_INPUT_MANAGER->IsActionPressed("Select"))
    {
        Engine::System::SceneManager::GetInstance()->ChangeSceneFade<StageSelect>();
    }
}

/// <summary>
/// 終了処理
/// </summary>
void Engine::Scene::Title::Release()
{
    //  ローカルオブジェクトの解放
    Engine::System::EntityManager::GetInstance()->ClearLocalEntities();
}
