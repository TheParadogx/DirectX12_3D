#pragma once
#include<cstdint>

namespace Engine::Audio
{
    //	ファイル先頭からのサンプリングレートの位置を固定する
#pragma pack(push,1)
    struct AudioHeader
    {
        uint32_t SampleRate;    // サンプリング周波数
        uint16_t Channels;      // チャンネル数
        uint16_t BitsPerSample; // ビット深度（16bit固定）
        uint64_t FrameCount;    // PCMフレーム総数
    };
#pragma pack(pop)
}