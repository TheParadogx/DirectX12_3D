#pragma once
#include"System/Singleton/Singleton.hpp"
#include"Effekseer/src/Effekseer/Effekseer.h"
#include"Effekseer/src/EffekseerRendererDX12/EffekseerRendererDX12.h"

#include"Math/Matrix/Matrix.h"

#include<unordered_map>
#include<string>
#include<filesystem>

namespace Engine::Graphics
{
	class EffectManager : public Singleton<EffectManager>
	{
		GENERATE_SINGLETON_BODY(EffectManager);
	public:

		/// <summary>
		/// 初期化
		/// </summary>
		/// <returns>true:成功</returns>
		bool Initialize();

		/// <summary>
		/// エフェクトの取得
		/// </summary>
		/// <param name="FilePath">フォルダパス</param>
		/// <returns></returns>
		Effekseer::EffectRef GetEffect(const std::filesystem::path& FilePath);

		/// <summary>
		/// 更新
		/// </summary>
		void Update(float DeltaTime);

		/// <summary>
		/// 描画
		/// </summary>
		void Draw();

		/// <summary>
		/// 変換
		/// </summary>
		/// <param name="CustomMat"></param>
		/// <returns></returns>
		Effekseer::Matrix44 ToEfkMatrix(const Math::Matrix& CustomMat);

		/// <summary>
		/// マネージャーの取得
		/// </summary>
		/// <returns></returns>
		Effekseer::ManagerRef GetManager();

	private:
		/// <summary>
		/// 全体管理
		/// </summary>
		Effekseer::ManagerRef mEfkManager;

		/// <summary>
		/// レンダラー
		/// </summary>
		EffekseerRenderer::RendererRef mEfkRenderer;

		/// <summary>
		/// コマンドリスト
		/// </summary>
		Effekseer::RefPtr<EffekseerRenderer::CommandList> mEfkCmdList;

		/// <summary>
		/// メモリ領域
		/// </summary>
		Effekseer::RefPtr<EffekseerRenderer::SingleFrameMemoryPool> mEfkMemoryPool;

		/// <summary>
		/// 読み込んだエフェクトのキャッシュ
		/// </summary>
		std::unordered_map<std::u16string, Effekseer::EffectRef> mEffectCache;

	};
}