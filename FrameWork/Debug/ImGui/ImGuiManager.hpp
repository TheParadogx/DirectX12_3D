#pragma once
#include"System/Singleton/Singleton.hpp"

#include<Windows.h>

#include<Plugin/ImGui/imgui.h>

namespace Engine::Graphics
{
	class DirectX;
	class GraphicsDescriptorHeapManager;
}


namespace Engine::Debug
{
	class ImGuiManager : public Singleton<ImGuiManager>
	{
		GENERATE_SINGLETON_BODY(ImGuiManager);
	public:
		/// <summary>
		/// ImGuiの初期化
		/// </summary>
		/// <param name="WindowHandle">ウィンドウのハンドル</param>
		/// <param name="DirectX">DirectXクラスのポインタ</param>
		/// <param name="GDHManager">ヒープ管理のポインタ</param>
		/// <returns>true:成功</returns>
		bool Initialize(const HWND WindowHandle, Graphics::DirectX* DirectX, Graphics::GraphicsDescriptorHeapManager* GDHManager);

		/// <summary>
		/// インスタンス解放
		/// </summary>
		void OnDestory()override;

		/// <summary>
		/// 新規フレームの開始
		/// </summary>
		void NewFrame();

		/// <summary>
		/// フレームの終了
		/// </summary>
		void EndFrame(Graphics::DirectX* DirectX, Graphics::GraphicsDescriptorHeapManager* GDHManager);

	private:
		ImGuiContext* mContext;
	};
}
