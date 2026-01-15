#include "pch.h"
#include "AudioResource.hpp"
#include "AudioHeader.hpp"

bool Engine::Audio::AudioResource::LoadFromAud(const fs::path& Path)
{
    std::ifstream ifs(Path, std::ios::binary);
    if (!ifs)
    {
        return false;
    }

    AudioHeader header = {};
    ifs.read(reinterpret_cast<char*>(&header), sizeof(header));

    SampleRate = header.SampleRate;
    Channels = header.Channels;
    PcmData.resize(header.FrameCount * Channels);
    ifs.read(reinterpret_cast<char*>(PcmData.data()), PcmData.size() * sizeof(int16_t));

    return true;
}
