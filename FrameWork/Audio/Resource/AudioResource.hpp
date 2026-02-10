#pragma once
#include<vector>
#include<filesystem>
#include<cstdint>

namespace fs = std::filesystem;

namespace Engine::Audio
{
    /// <summary>
    /// audのデータからリソースの取得するための構造体
    /// </summary>
    struct AudioResource {
        uint32_t SampleRate;
        uint16_t Channels;
        std::vector<int16_t> PcmData;

        /// <summary>
        /// Aud拡張子の内容を読み込む
        /// </summary>
        /// <param name="Path"></param>
        /// <returns></returns>
        bool LoadFromAud(const fs::path& Path);
    };
}