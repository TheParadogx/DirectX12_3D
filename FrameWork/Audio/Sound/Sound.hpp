#pragma once

#include "Audio/Resource/AudioResource.hpp"

namespace Engine::Audio
{
	/// <summary>
/// SoundManagerが管理するサウンドクラス
/// このクラス単体でサウンドを流すことができないようになっているので構造体で。
/// </summary>
	struct Sound
	{
		Sound(AudioResource* Resorce);
		virtual ~Sound() = default;

		/// <summary>
		/// 再生
		/// </summary>
		void Play();

		/// <summary>
		/// 停止
		/// </summary>
		void Stop();

		/// <summary>
		///	中断
		/// </summary>
		void Pause();

		/// <summary>
		/// 再生判定
		/// </summary>
		/// <returns>true:再生中</returns>
		bool IsPlaying();

		/// <summary>
		/// Outputに直接 PCMを加算
		/// </summary>
		void ApplyAndMix(int16_t* Output, size_t FramesRequested, uint16_t OutputChannels);

		/// <summary>
		/// 再生用リソースの参照用ポインタ
		/// </summary>
		AudioResource* Resource;

		/// <summary>
		/// 今の再生フレーム
		/// </summary>
		uint64_t CurrentFrame = 0;

		/// <summary>
		/// 音量
		/// </summary>
		float Volume = 1.0f;

		/// <summary>
		/// true:ループ処理
		/// </summary>
		bool Loop = false;

		/// <summary>
		/// true:再生中
		/// </summary>
		bool Playing = false;

	};

}