#pragma once
#include"System/Scene/IScene.hpp"
#include"Graphics/SkyBox/SkyBox.hpp"

#include<memory>

namespace Engine::Scene
{
	class StageSelect : public System::IScene
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <returns>true:成功</returns>
		virtual bool Initialize()override;

		/// <summary>
		/// 描画
		/// </summary>
		virtual void Render()override;

		/// <summary>
		/// 終了処理
		/// </summary>
		virtual void Release()override;

		virtual std::string GetSceneName() override 
		{
			return "StageSelect";
		};

	private:
		/// <summary>
		/// スカイボックス
		/// </summary>
		std::unique_ptr<Graphics::SkyBox> mSkyBox;
	};
}

