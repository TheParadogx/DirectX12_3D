#pragma once
#include"System/Scene/IScene.hpp"

namespace Engine::Scene
{
	class Title : public Engine::System::IScene
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
		/// I—¹ˆ—
		/// </summary>
		void Release()override;

		std::string GetSceneName()override
		{
			return "Title";
		}
	private:

	};
}