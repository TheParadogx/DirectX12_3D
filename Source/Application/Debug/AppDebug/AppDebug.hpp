#pragma once


namespace Engine::Debug
{
	class AppDebugUI
	{
	public:
		/// <summary>
		/// Scene関係のデバック
		/// </summary>
		static void SceneDebug();

		/// <summary>
		/// 全てのDebug操作の状態更新
		/// </summary>
		static void UpdateImGui();
	private:

	};
}

