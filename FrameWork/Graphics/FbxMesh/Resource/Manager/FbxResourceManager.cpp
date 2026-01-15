#include "pch.h"
#include "FbxResourceManager.hpp"

/// <summary>
/// リソースの読み込み
/// </summary>
/// <param name="Path">フォルダパス</param>
/// <param name="CustomName">キーを設定可能。未設定の場合はフォルダパス</param>
/// <returns></returns>
Engine::Graphics::FbxResource* Engine::Graphics::FbxResourceManager::Load(const std::filesystem::path& Path, const std::string& CustomName)
{
	const std::string& path = Path.string();

	auto it = mResources.find(path);
	if (it != mResources.end())
	{
		return it->second.get();
	}

	//	ロードして登録
	auto res = std::make_unique<FbxResource>();
	const bool ret = res->Create(path);
	if (ret == false)
	{
		LOG_ERROR("Failed CreateResource.");
		return nullptr;
	}

	auto ptr = res.get();
	mResources[path] = std::move(res);
	return ptr;
}
