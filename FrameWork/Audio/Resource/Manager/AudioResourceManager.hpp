#pragma once
#include "Audio/Resource/AudioResource.hpp"
#include "System/Singleton/Singleton.hpp"

#include<unordered_map>
#include<string>
#include<memory>
#include<filesystem>

namespace fs = std::filesystem;

namespace Engine::Audio
{
	/// <summary>
	/// サウンドのリソース管理
	/// </summary>
	class AudioResourceManager : public Singleton<AudioResourceManager>
	{
		GENERATE_SINGLETON_BODY(AudioResourceManager);

		/// <summary>
		/// ファイルパスを全て相対パスに変換する。
		/// </summary>
		/// <param name="FilePath"></param>
		/// <returns></returns>
		std::string NormalizePath(const std::filesystem::path& FilePath);

	public:
		/// <summary>
		/// リソースの取得
		/// まだ読み込んでいないリソースなら読み込む。
		/// </summary>
		/// <param name="FileName">ファイルパス</param>
		/// <returns></returns>
		AudioResource* GetResource(const std::string& FileName);

		/// <summary>
		/// 指定したディレクトリ配下の.audを全て読み込む。
		/// </summary>
		/// <param name="Root"></param>
		void LoadAllFromDirectory(const fs::path& Root);

		/// <summary>
		/// 指定したディレクトリ配下のリソースをすべて解放する。
		/// </summary>
		/// <param name="Root"></param>
		void UnloadAllFromDirectory(const fs::path& Root);

	private:
		/// <summary>
		/// リソースのコレクション
		/// </summary>
		std::unordered_map<std::string, std::unique_ptr<AudioResource>> mResources;
	};

}