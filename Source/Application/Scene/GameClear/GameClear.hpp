#pragma once
#include"System/Scene/IScene.hpp"

namespace Engine::Scene
{
	class GameClear : public System::IScene
	{
	public:

		/// <summary>
		/// ‰Šú‰»
		/// </summary>
		/// <returns>true:¬Œ÷</returns>
		bool Initialize()override;

		/// <summary>
		/// –ŒãXV
		/// </summary>
		void PostUpdate(double FixedDeltaTime)override;

		/// <summary>
		/// I—¹ˆ—
		/// </summary>
		void Release()override;

		std::string GetSceneName()override
		{
			return "GameClear";
		}
	private:

	};
}

