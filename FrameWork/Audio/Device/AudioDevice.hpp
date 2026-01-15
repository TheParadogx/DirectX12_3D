#pragma once
#include"System/Singleton/Singleton.hpp"

#include<Plugin/miniaudio/miniaudio.h>

namespace Engine::Audio
{
	class AudioManager;

	class AudioDevice : public Singleton<AudioDevice>
	{
		GENERATE_SINGLETON_BODY(AudioDevice);

		/// <summary>
		/// リソース解放
		/// </summary>
		void OnDestory()override;
	public:

		/// <summary>
		/// デバイスの生成
		/// </summary>
		/// <returns>true:成功</returns>
		bool Initialize(AudioManager* Manager, uint32_t SampleRate = 44100, uint16_t Channels = 2);

	private:
		/// <summary>
		/// オーディオデバイス
		/// </summary>
		ma_device mDevice;

		/// <summary>
		/// オーディオ管理の参照用ポインタ
		/// </summary>
		AudioManager* mAudioManager;

	};
}

