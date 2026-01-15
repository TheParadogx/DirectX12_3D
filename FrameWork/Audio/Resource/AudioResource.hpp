#pragma once
#include<vector>
#include<filesystem>
#include<cstdint>

namespace fs = std::filesystem;

namespace Engine::Audio
{
    /// <summary>
    /// audのデータからリソースの取得
    /// </summary>
    struct AudioResource {
        uint32_t SampleRate;
        uint16_t Channels;
        std::vector<int16_t> PcmData;

        bool LoadFromAud(const fs::path& Path);
    };
}