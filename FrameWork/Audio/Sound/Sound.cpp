#include "pch.h"
#include "Sound.hpp"

Engine::Audio::Sound::Sound(AudioResource* Resorce)
	:Resource(Resorce)
	, CurrentFrame(0)
	, Volume(1.0f)
	, Loop(false)
	, Playing(false)
{
}

/// <summary>
/// 再生
/// </summary>
void Engine::Audio::Sound::Play()
{
	Playing = true;
}

/// <summary>
/// 停止
/// </summary>
void Engine::Audio::Sound::Stop()
{
	Playing = false;
	CurrentFrame = 0;
}

/// <summary>
///	中断
/// </summary>
void Engine::Audio::Sound::Pause()
{
	Playing = false;
}

/// <summary>
/// 再生判定
/// </summary>
/// <returns>true:再生中</returns>
bool Engine::Audio::Sound::IsPlaying()
{
	return Playing;
}

/// <summary>
/// Outputに直接 PCMを加算
/// </summary>
void Engine::Audio::Sound::ApplyAndMix(int16_t* Output, size_t FramesRequested, uint16_t OutputChannels)
{
	if (Playing == false || Resource == nullptr)
	{
		return;
	}

	//　リソースの全フレーム数と現在の位置から残りフレーム数を計算
	size_t framesAvailable = Resource->PcmData.size() / Resource->Channels - CurrentFrame;

	//	要求された数と提供可能な数の小さい方をコピー対象とする
	size_t framesToCopy = std::min(FramesRequested, framesAvailable);

	//	フレーム単位でループ（時間軸の移動）
	for (size_t frame = 0; frame < framesToCopy; frame++)
	{
		//	デバイスの出力チャンネル（L/R）単位でループ
		for (size_t channel = 0; channel < OutputChannels; channel++)
		{
			int16_t sample = 0;

			//	リソース側に該当チャンネルがあるときはサンプルを取得
			if (channel < Resource->Channels)
			{
				sample = static_cast<int16_t>(Resource->PcmData[(CurrentFrame + frame) * Resource->Channels + channel] * Volume);
			}
			//	ミキシング設定
			// 既存の出力バッファの値に今回のサンプルを加算。
			// 16bitの範囲を超える可能性があるため、一旦 32bit(int32_t) で計算する。
			int32_t mixed = Output[frame * OutputChannels + channel] + sample;

			// int16_tの範囲にクランプする
			if (mixed > INT16_MAX)
			{
				mixed = INT16_MAX;
			}
			if (mixed < -INT16_MAX)
			{
				mixed = -INT16_MAX;
			}

			//	最終的な値をバッファに書き戻す
			Output[frame * OutputChannels + channel] = static_cast<int16_t>(mixed);
		}
	}

	//	再生位置を更新
	CurrentFrame += framesToCopy;

	//	最後まで再生しきった場合の処理
	if (CurrentFrame >= Resource->PcmData.size() / Resource->Channels)
	{
		if (Loop == true)
		{
			//	生成位置を先頭に戻す
			CurrentFrame = 0;
		}
		else
		{
			//	再生終了フラグを立てる
			Playing = false;
		}
	}

}

