#pragma once
#include"System/Scene/IScene.hpp"
#include"Graphics/SkyBox/SkyBox.hpp"
#include"Application/Objects/EnemyRank.hpp"
#include"entt/entt.hpp"

#include<memory>

namespace Engine::Scene
{
	class InGame : public Engine::System::IScene
	{
	public:
		InGame(System::EnemyRank Rank = System::EnemyRank::Basic);

		/// <summary>
		///	初期化
		/// </summary>
		/// <returns></returns>
		bool Initialize()override;

		/// <summary>
		/// 画面の切り替え判定
		/// </summary>
		/// <param name="FixedDeltaTime"></param>
		void PostUpdate(double FixedDeltaTime)override;

		/// <summary>
		/// 描画
		/// </summary>
		void Render()override;

		/// <summary>
		/// 終了処理
		/// </summary>
		void Release()override;

		std::string GetSceneName()override
		{
			return "InGame";
		}
	private:
		/// <summary>
		/// スカイボックス
		/// </summary>
		std::unique_ptr<Graphics::SkyBox> mSkyBox;
		System::EnemyRank mSelectEnemy;
	};
}

