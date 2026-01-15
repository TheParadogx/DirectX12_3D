#pragma once
#include"System/Singleton/Singleton.hpp"
#include"Graphics/FbxMesh/Resource/FbxResource.hpp"

#include<filesystem>
#include<string>
#include<memory>
#include<unordered_map>

namespace Engine::Graphics
{
	class FbxResourceManager : public Singleton<FbxResourceManager>
	{
		GENERATE_SINGLETON_BODY(FbxResourceManager);

	public:
		/// <summary>
		/// 最初にリソースを読み込む
		/// </summary>
		void Initialize();

		/// <summary>
		/// リソースの読み込み
		/// </summary>
		/// <param name="Path">フォルダパス</param>
		/// <param name="CustomName">キーを設定可能。未設定の場合はフォルダパス</param>
		/// <returns></returns>
		FbxResource* Load(const std::filesystem::path& Path,const std::string& CustomName = "");
	private:
		/// <summary>
		/// リソースのディクショナリー
		/// </summary>
		std::unordered_map<std::string, std::unique_ptr<FbxResource>> mResources;
	};
}


