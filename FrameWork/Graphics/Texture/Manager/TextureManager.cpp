#include "pch.h"
#include "TextureManager.hpp"

/// <summary>
/// ƒŠƒ\[ƒX‚Ì“Ç‚İ‚İ
/// </summary>
/// <param name="FilePath"></param>
/// <returns></returns>
Engine::Graphics::Texture* Engine::Graphics::TextureManager::Load(const std::filesystem::path& FilePath)
{
	auto it = mTextures.find(FilePath.string());
	if (it != mTextures.end())
	{
		return it->second.get();
	}

	auto res = std::make_unique<Texture>();
	const bool ret = res->Create(FilePath);
	if (ret == false)
	{
		LOG_ERROR("Failed CreateTexture");
		return nullptr;
	}

	auto ptr = res.get();
	mTextures[FilePath.string()] = std::move(res);
	return ptr;
}
