#include "pch.h"
#include "SkyBoxResourceManager.hpp"

namespace Engine::Graphics
{

    /// <summary>
    /// リソースの読み込み
    /// </summary>
    /// <param name="FilePath">SkyBox用のddsのファイルパス</param>
    /// <returns>参照用のポインタ</returns>
    SkyBoxResource* Engine::Graphics::SkyBoxResourceManager::Load(const std::filesystem::path& FilePath)
    {
        auto it = mResources.find(FilePath.string());
        if (it != mResources.end())
        {
            return it->second.get();
        }

        auto res = std::make_unique<SkyBoxResource>();
        const bool ret = res->Create(FilePath);
        if (ret == false)
        {
            LOG_ERROR("Failed CreateSkyBoxResource.");
            return nullptr;
        }

        auto ptr = res.get();
        mResources[FilePath.string()] = std::move(res);
        return ptr;
    }
}
