#include "pch.h"
#include "AudioManager.hpp"
#include "Utility/PtrUtils.hpp"

#include "Debug/spdlog/Logger.hpp"

/// <summary>
/// 初期化
/// </summary>
/// <param name="AudioResManager">リソース管理のポインタ</param>
/// <returns>true:成功</returns>
bool Engine::Audio::AudioManager::Initialize(AudioResourceManager* AudioResManager)
{
	if (AudioResManager == nullptr)
	{
		LOG_ERROR("サウンドリソース管理の取得に失敗");
		return false;
	}

	mResources = AudioResManager;

	return true;
}

/// <summary>
/// SEの再生
/// </summary>
/// <param name="FilePath">ファイル名</param>
/// <param name="Loop">true:ループ再生</param>
/// <param name="Volume">音量:0.0f - 1.0f</param>
void Engine::Audio::AudioManager::PlaySE(const std::string& FilePath, bool Loop, float Volume)
{
	auto resource = mResources->GetResource(FilePath);
	if (IsNullptr(resource))
	{
		return;
	}

	Sound sound(resource);
	sound.Loop = Loop;
	sound.Volume = Volume;
	sound.Play();
	std::lock_guard<std::recursive_mutex> lock(mMtx);
	mSounds.push_back(std::move(sound));
}

void Engine::Audio::AudioManager::MixSounds(int16_t* Output, size_t FramesRequested, uint16_t Channels)
{
	std::lock_guard<std::recursive_mutex> lock(mMtx);
	std::fill(Output, Output + FramesRequested * Channels, 0);

	for (auto it = mSounds.begin(); it != mSounds.end();) {
		if (it->IsPlaying() == false) 
		{
			it = mSounds.erase(it);
			continue;
		}

		it->ApplyAndMix(Output, FramesRequested, Channels);
		++it;
	}
}

/// <summary>
/// miniaudioで呼び出されるコールバック
/// データを生成して送信する
/// </summary>
void Engine::Audio::AudioManager::DataCallback(ma_device* pDevice, void* pOutput, const void*, ma_uint32 frameCount)
{
	AudioManager* mgr = reinterpret_cast<AudioManager*>(pDevice->pUserData);
	mgr->MixSounds(reinterpret_cast<int16_t*>(pOutput), frameCount, 2); // 2ch固定
}
