#pragma once
#include"System/Scene/IScene.hpp"
#include"Graphics/SkyBox/SkyBox.hpp"

#include<memory>

namespace Engine::Scene
{

	class StageSelect : public System::IScene
	{
		/// <summary>
		/// ウィンドウを開く
		/// </summary>
		void OpenConfirmWindow();

		/// <summary>
		/// ウィンドウを閉じる
		/// </summary>
		void CloseConfirmWindow();
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <returns>true:成功</returns>
		virtual bool Initialize()override;

		virtual void Update(double DeltaTime) override;

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

		enum class eState {
			Normal,     // 通常時
			Confirming  // タイトルに戻るか確認中
		};

		/// <summary>
		/// スカイボックス
		/// </summary>
		std::unique_ptr<Graphics::SkyBox> mSkyBox;

		/// <summary>
		/// 今の状態
		/// </summary>
		eState mState = eState::Normal;
		entt::entity mConfirmWindowEntity = entt::null;

	};
}

