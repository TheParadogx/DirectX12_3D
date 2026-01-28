#pragma once
#include"System/Scene/IScene.hpp"

namespace Engine::Scene
{
	class InGame : public Engine::System::IScene
	{
	public:
		/// <summary>
		///	‰Šú‰»
		/// </summary>
		/// <returns></returns>
		bool Initialize()override;

		/// <summary>
		/// ‰æ–Ê‚ÌØ‚è‘Ö‚¦”»’è
		/// </summary>
		/// <param name="FixedDeltaTime"></param>
		void PostUpdate(double FixedDeltaTime)override;

		/// <summary>
		/// •`‰æ
		/// </summary>
		void Render()override;

		/// <summary>
		/// I—¹ˆ—
		/// </summary>
		void Release()override;

		std::string GetSceneName()override
		{
			return "InGame";
		}
	private:

	};
}

