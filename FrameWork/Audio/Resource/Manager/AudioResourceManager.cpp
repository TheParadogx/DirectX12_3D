#include "pch.h"
#include "AudioResourceManager.hpp"
#include "Debug/spdlog/Logger.hpp"



/// <summary>
/// ファイルパスを全て相対パスに変換する。
/// </summary>
/// <param name="FilePath"></param>
/// <returns></returns>
std::string Engine::Audio::AudioResourceManager::NormalizePath(const std::filesystem::path& FilePath)
{
	return std::filesystem::relative(FilePath, std::filesystem::current_path()).generic_string();
}

/// <summary>
/// リソースの取得
/// まだ読み込んでいないリソースなら読み込む。
/// </summary>
/// <param name="FileName">ファイルパス</param>
/// <returns></returns>
Engine::Audio::AudioResource* Engine::Audio::AudioResourceManager::GetResource(const std::string& FileName)
{
	std::string path = this->NormalizePath(FileName);

	//	すでに読み込まれていたら返す
	auto it = mResources.find(path);
	if (it != mResources.end())
	{
		return it->second.get();
	}

	//	なければ読み込み処理
	std::unique_ptr<AudioResource> resource = std::make_unique<AudioResource>();
	const bool ret = resource->LoadFromAud(path);
	if (ret == false)
	{
		LOG_ERROR("読み込み失敗: " + path);
		return nullptr;
	}

	AudioResource* result = resource.get();
	mResources[path] = std::move(resource);
	LOG_INFO("Loaded audio resource: ",path);
	return result;
}

/// <summary>
/// 指定したディレクトリ配下の.audを全て読み込む。
/// </summary>
/// <param name="Root"></param>
void Engine::Audio::AudioResourceManager::LoadAllFromDirectory(const fs::path& Root)
{
	//	ファイルとディレクトリ捜査
	if (fs::exists(Root) == false || fs::is_directory(Root) == false)
	{
		LOG_WARN("not find directory: ",Root.string());
		return;
	}

	//	サブフォルダまで捜査
	for (auto& entry : fs::recursive_directory_iterator(Root))
	{
		if (entry.is_regular_file() == false)
		{
			continue;
		}

		fs::path filepath = entry.path();
		std::string ext = filepath.extension().string();

		//	拡張子を小文字に変換する
		std::transform(ext.begin(), ext.end(),ext.begin(),
			[](unsigned char c) { return std::tolower(c); });

		//	拡張子判定
		if (ext != ".aud")
		{
			continue;
		}

		//	読み込み処理をする
		this->GetResource(filepath.string());
	}

}

/// <summary>
/// 指定したディレクトリ配下のリソースをすべて解放する。
/// </summary>
/// <param name="Root"></param>
void Engine::Audio::AudioResourceManager::UnloadAllFromDirectory(const fs::path& Root)
{
	std::string path = this->NormalizePath(Root);

	size_t before = mResources.size();

	std::erase_if(mResources,
		[&](const auto& pair)
		{
			const std::string& path = pair.first;
			return path.starts_with(path);
		}
	);

	size_t after = mResources.size();

	LOG_INFO("%s 以下のテクスチャを % zu 個削除しました", path.c_str(), before - after);

}
