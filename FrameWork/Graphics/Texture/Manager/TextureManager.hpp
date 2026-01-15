#pragma once
#include"System/Singleton/Singleton.hpp"
#include"Graphics/Texture/Texture.hpp"

#include<filesystem>
#include<string>
#include<memory>
#include<unordered_map>

namespace Engine::Graphics
{
	class TextureManager : public Singleton<TextureManager >
	{
		GENERATE_SINGLETON_BODY(TextureManager);
	public:
		/// <summary>
		/// リソースの読み込み
		/// </summary>
		/// <param name="FilePath"></param>
		/// <returns></returns>
		Texture* Load(const std::filesystem::path& FilePath);
	private:
		/// <summary>
		/// テクスチャのコレクション
		/// </summary>
		std::unordered_map<std::string, std::unique_ptr<Texture>> mTextures;
	};
}

