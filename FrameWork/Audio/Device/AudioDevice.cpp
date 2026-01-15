#include "pch.h"
#define MINIAUDIO_IMPLEMENTATION
#include<Plugin/miniaudio/miniaudio.h>

#include "AudioDevice.hpp"
#include"Audio/Manager/AudioManager.hpp"

#include"Debug/spdlog/Logger.hpp"
#include"Utility/PtrUtils.hpp"

/// <summary>
/// リソース解放
/// </summary>
void Engine::Audio::AudioDevice::OnDestory()
{
	ma_device_uninit(&mDevice);
}

/// <summary>
/// デバイスの生成
/// </summary>
/// <returns>true:成功</returns>
bool Engine::Audio::AudioDevice::Initialize(AudioManager* Manager, uint32_t SampleRate, uint16_t Channels)
{

	if (IsNullptr(Manager))
	{
		LOG_ERROR("AudioManager nullptr.");
		return false;
	}
	mAudioManager = Manager;

	ma_device_config Config = ma_device_config_init(ma_device_type_playback);
	Config.playback.format = ma_format_s16;
	Config.playback.channels = Channels;
	Config.sampleRate = SampleRate;
	Config.dataCallback = AudioManager::DataCallback;
	Config.pUserData = Manager;

	ma_result result = ma_device_init(NULL, &Config, &mDevice);
	if (result != MA_SUCCESS)
	{
		LOG_ERROR("Failed to init audio device.");
		return false;
	}

	result = ma_device_start(&mDevice);
	if (result != MA_SUCCESS)
	{
		LOG_ERROR("Failed to start audio device.");
		ma_device_uninit(&mDevice);
		return false;
	}


	return true;
}
