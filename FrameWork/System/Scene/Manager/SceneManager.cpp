#include "pch.h"
#include "SceneManager.hpp"

#include"Graphics/DirectX/DirectX12.hpp"
#include"System/Entity/System/Manager/SystemManager.hpp"

#include"System/Window/Window.hpp"
#include"System/Conponent/Sprite/SpriteComponent.hpp"
#include"System/Fade/FadeComponent.hpp"
#include"Graphics/Texture/Manager/TextureManager.hpp"

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

	if (!mRegistry.valid(mFadeEntity))
	{
		return;
	}

	FadeComponent& fade = mRegistry.get<FadeComponent>(mFadeEntity);
	SpriteComponent& sprite = mRegistry.get<SpriteComponent>(mFadeEntity);
	//	時間を進めてアルファを計算するLerp
	fade.CurrentTime += (float)FixedDeltaTime;

	// 0.0 ～ 1.0 にクランプ
	float t = std::clamp(fade.CurrentTime / fade.TotalTime, 0.0f, 1.0f);

	// std::lerp(開始値, 終了値, 進行度)
	float alpha = std::lerp(fade.StartAlpha, fade.TargetAlpha, t);

	sprite.Sprite.SetColor({ 0.0f, 0.0f, 0.0f, alpha });

	if (fade.State == eFadeState::FadeIn)
	{
		if (t >= 1.0f)
		{
			mRegistry.destroy(mFadeEntity);
			mFadeEntity = entt::null;
		}
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

void Engine::System::SceneManager::FadeRender()
{
	auto view = mRegistry.view<SpriteComponent>();
	view.each([&](SpriteComponent& sprite)
		{
			sprite.Sprite.Render();
		});
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

	auto view = mRegistry.view<FadeComponent>();
	if (view.empty() == false)
	{
		auto& fade = mRegistry.get<FadeComponent>(mFadeEntity);

		//	フェードアウトでなおかつ
		if (fade.State == eFadeState::FadeOut)
		{
			//	フェードが終わっていないとまだ切り替えない
			float t = fade.CurrentTime / fade.TotalTime;
			if (t < 1.0f) return;

			Graphics::DirectX::GetInstance()->WaitForGPU();
			SystemManager::GetInstance()->AllClearSystem();


			//	終わっていたら切り替える
			if (mScene != nullptr)
			{
				mScene->Release();
				mScene.reset();
			}

			mScene = std::move(mNextScene);
			mNextScene = nullptr;

			mScene->Initialize();

			//	
			fade.State = eFadeState::FadeIn;
			fade.StartAlpha = 1.0f;
			fade.TargetAlpha = 0.0f;
			fade.CurrentTime = 0.0f;

		}

	}
	else
	{
		Graphics::DirectX::GetInstance()->WaitForGPU();
		SystemManager::GetInstance()->AllClearSystem();


		//	終わっていたら切り替える
		if (mScene != nullptr)
		{
			mScene->Release();
			mScene.reset();
		}


		mScene = std::move(mNextScene);
		mNextScene = nullptr;

		mScene->Initialize();

	}


}

/// <summary>
/// フェード用のエンティティ作成
/// </summary>
void Engine::System::SceneManager::CreateFadeEntity()
{
	if (mFadeEntity != entt::null) return;

	mFadeEntity = mRegistry.create();
	auto res = Graphics::TextureManager::GetInstance()->Load("Assets/Fade/Fade.png");
	auto& sprite = mRegistry.emplace<SpriteComponent>(mFadeEntity,res);
	auto window = Window::GetInstance();
	sprite.Sprite.SetSize({ (float)window->GetWidth(), (float)window->GetHeight() });
	sprite.Sprite.SetColor({ 0,0,0,0 });
	auto& fade = mRegistry.emplace<FadeComponent>(mFadeEntity);
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
