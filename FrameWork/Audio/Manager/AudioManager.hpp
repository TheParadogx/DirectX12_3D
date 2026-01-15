#pragma once
#include"System/Singleton/Singleton.hpp"
#include "Audio/Resource/Manager/AudioResourceManager.hpp"
#include "Audio/Sound/Sound.hpp"
#include<Plugin/miniaudio/miniaudio.h>

#include<mutex>
#include<vector>

namespace Engine::Audio
{
	/// <summary>
	/// オーディオ再生管理
	/// </summary>
	class AudioManager : public Singleton<AudioManager>
	{
		GENERATE_SINGLETON_BODY(AudioManager);

	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="AudioResManager">リソース管理のポインタ</param>
		/// <returns>true:成功</returns>
		bool Initialize(AudioResourceManager* AudioResManager);

		/// <summary>
		/// SEの再生
		/// </summary>
		/// <param name="FilePath">ファイル名</param>
		/// <param name="Loop">true:ループ再生</param>
		/// <param name="Volume">音量:0.0f - 1.0f</param>
		void PlaySE(const std::string& FilePath, bool Loop = false, float Volume = 1.0f);


		void MixSounds(int16_t* Output, size_t FramesRequested, uint16_t Channels);

		/// <summary>
		/// miniaudioで呼び出されるコールバック
		/// データを生成して送信する
		/// </summary>
		static void DataCallback(ma_device* pDevice, void* pOutput, const void* /*pInput*/, ma_uint32 frameCount);
	private:
		/// <summary>
		/// サウンドのリソース管理の参照
		/// </summary>
		AudioResourceManager* mResources;

		/// <summary>
		/// 今流れているサウンド
		/// </summary>
		std::vector<Sound> mSounds;

		/// <summary>
		/// マルチスレッド用
		/// </summary>
		std::recursive_mutex  mMtx;
	};
}
