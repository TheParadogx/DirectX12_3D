#pragma once
#include"System/Singleton/Singleton.hpp"
#include"Graphics/SkyBox/Resource/SkyBoxResource.hpp"

#include<unordered_map>
#include<string>
#include<memory>
#include<filesystem>

namespace Engine::Graphics
{
	/// <summary>
	/// スカイボックスのリソース管理
	/// </summary>
	class SkyBoxResourceManager : public Singleton<SkyBoxResourceManager>
	{
		GENERATE_SINGLETON_BODY(SkyBoxResourceManager);

	public:

		/// <summary>
		/// リソースの読み込み
		/// </summary>
		/// <param name="FilePath">SkyBox用のddsのファイルパス</param>
		/// <returns>参照用のポインタ</returns>
		SkyBoxResource* Load(const std::filesystem::path& FilePath);

	private:
		/// <summary>
		/// 管理しているリソース
		/// </summary>
		std::unordered_map<std::string, std::unique_ptr<SkyBoxResource>> mResources;

	};
}

