#pragma once

#include "System/Singleton/Singleton.hpp"


namespace Engine::Graphics
{
	class DirectX;
	class GraphicsDescriptorHeapManager;
	class Renderer;
}

namespace Engine::Debug
{
	class ImGuiManager;
}

namespace Engine::System
{
	class Window;
}


namespace Engine::Core
{
	class Engine : public Singleton<Engine>
	{
		GENERATE_SINGLETON_BODY(Engine);

	public:
		/// <summary>
		/// フレームワークの初期化
		/// </summary>
		/// <returns>true:成功</returns>
		bool Initialize();

		/// <summary>
		/// フレームワークの解放
		/// </summary>
		void OnDestory()override;

		/// <summary>
		/// 新規フレームの開始
		/// </summary>
		void NewFrame();

		/// <summary>
		/// レンダリングの開始
		/// </summary>
		void BegineRendering();

		/// <summary>
		/// フレームの終了
		/// </summary>
		void EndFrame();

	private:
		Graphics::DirectX* mDirectX;
		System::Window* mWindow;
		Graphics::GraphicsDescriptorHeapManager* mGDHManager;
		Debug::ImGuiManager* mImgui;
		Graphics::Renderer* mRenderer;
	};
}