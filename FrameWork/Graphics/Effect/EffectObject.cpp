#include "pch.h"
#include "EffectObject.hpp"
#include "Manager/EffectManager.hpp"

/// <summary>
/// エフェクトの再生
/// </summary>
/// <param name="effect"></param>
void Engine::Graphics::EffectObject::Play(Effekseer::EffectRef Effect, const Math::Vector3& Position,bool AutoDelete)
{
	auto manager = EffectManager::GetInstance()->GetManager();
	if (manager == nullptr || Effect == nullptr) return;

	//	すでに再生中なら停止
	Stop();

	mIsAutoDelete = AutoDelete;

	//	再生
	mHandle = manager->Play(Effect, Position.x, Position.y, Position.z);

	//	再生速度の反映
	if (mHandle != -1)
	{
		manager->SetSpeed(mHandle, mSpeed);
	}

}

void Engine::Graphics::EffectObject::SetLocation(const Math::Vector3& Position)
{
	auto manager = EffectManager::GetInstance()->GetManager();
	if (manager == nullptr || mHandle == -1) return;

	manager->SetLocation(mHandle, Position.x, Position.y, Position.z);
}

void Engine::Graphics::EffectObject::SetRotation(const Math::Vector3& Rotation)
{
	auto manager = EffectManager::GetInstance()->GetManager();
	if (manager == nullptr || mHandle == -1) return;

	//	回転はラジアン角
	manager->SetRotation(mHandle, Rotation.x, Rotation.y, Rotation.z);
}

void Engine::Graphics::EffectObject::SetScale(const Math::Vector3& Scale)
{
	auto manager = EffectManager::GetInstance()->GetManager();
	if (manager == nullptr || mHandle == -1) return;
	manager->SetScale(mHandle, Scale.x, Scale.y, Scale.z);
}

void Engine::Graphics::EffectObject::SetSpeed(float Speed)
{
	mSpeed = Speed;
	auto manager = EffectManager::GetInstance()->GetManager();
	if (manager == nullptr || mHandle == -1) return;
	manager->SetSpeed(mHandle, mSpeed);
}

/// <summary>
/// ハンドルの解放
/// </summary>
void Engine::Graphics::EffectObject::Stop()
{
	auto manager = EffectManager::GetInstance()->GetManager();
	if (manager == nullptr || mHandle == -1) return;
	manager->StopEffect(mHandle);
	mHandle = -1;
}

/// <summary>
/// 表示の切り替え　
/// </summary>
/// <param name="IsShow">true:表示する</param>
void Engine::Graphics::EffectObject::SetShown(bool IsShow)
{
	auto manager = EffectManager::GetInstance()->GetManager();
	if (manager == nullptr || mHandle == -1) return;
	manager->SetShown(mHandle, IsShow);
}

/// <summary>
/// エフェクトの再生を止める
/// </summary>
/// <param name="Visible">true:表示する</param>
void Engine::Graphics::EffectObject::SetVisible(bool Visible)
{
	auto manager = EffectManager::GetInstance()->GetManager();
	if (manager == nullptr || mHandle == -1) return;
	manager->SetShown(mHandle, Visible);
	manager->SetPaused(mHandle, !Visible);
}

/// <summary>
/// 再生中かどうかの判定
/// </summary>
/// <returns>true:存在している</returns>
bool Engine::Graphics::EffectObject::IsPlaying() const
{
	auto manager = EffectManager::GetInstance()->GetManager();
	if (manager == nullptr || mHandle == -1) return false;
	return manager->Exists(mHandle);
}
/// <summary>
/// 自動削除の条件判定
/// </summary>
/// <returns></returns>
bool Engine::Graphics::EffectObject::ShouldDestroy() const
{
	return mIsAutoDelete && !IsPlaying();
}

/// <summary>
/// ハンドルの取得
/// </summary>
/// <returns></returns>
Effekseer::Handle Engine::Graphics::EffectObject::GetHandle() const
{
	return mHandle;
}
