#include "pch.h"
#include "ImGuiManager.hpp"

#include"Graphics/DirectX/DirectX12.hpp"
#include"Graphics/GraphicsDescriptorHeap/GraphicsDescriptorHeapManager.hpp"

#include"Debug/spdlog/Logger.hpp"

#include<Plugin/ImGui/imgui_impl_dx12.h>
#include<Plugin/ImGui/imgui_impl_win32.h>

/// <summary>
/// ImGuiの初期化
/// </summary>
/// <param name="WindowHandle">ウィンドウのハンドル</param>
/// <param name="DirectX">DirectXクラスのポインタ</param>
/// <param name="GDHManager">ヒープ管理のポインタ</param>
/// <returns>true:成功</returns>
bool Engine::Debug::ImGuiManager::Initialize(const HWND WindowHandle,Graphics::DirectX * DirectX, Graphics::GraphicsDescriptorHeapManager* GDHManager)
{
    //  コンテキストの作成
    mContext = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    //  初期化
    ImGui::SetCurrentContext(mContext);
    ImGui_ImplWin32_Init(WindowHandle);

    ImGui_ImplDX12_InitInfo init_info = {};
    init_info.Device = DirectX->GetDevice();
    init_info.CommandQueue = DirectX->GetCommandQueue();
    init_info.NumFramesInFlight = DirectX->NUM_BACK_BAFFER;// APP_NUM_FRAMES_IN_FLIGHT;
    init_info.RTVFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    init_info.DSVFormat = DXGI_FORMAT_UNKNOWN;

    const Graphics::DescriptorHeapInfo info = GDHManager->Issuance(1);
    init_info.SrvDescriptorHeap = GDHManager->GetGraphicsHeap();
    init_info.LegacySingleSrvCpuDescriptor = GDHManager->GetCpuHandle(info);
    init_info.LegacySingleSrvGpuDescriptor = GDHManager->GetGpuHandle(info);
    ImGui_ImplDX12_Init(&init_info);

    return true;
}

/// <summary>
/// インスタンス解放
/// </summary>
void Engine::Debug::ImGuiManager::OnDestory()
{
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext(mContext);
    mContext = nullptr;
}

/// <summary>
/// 新規フレームの開始
/// </summary>
void Engine::Debug::ImGuiManager::NewFrame()
{
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

/// <summary>
/// フレームの終了
/// </summary>
void Engine::Debug::ImGuiManager::EndFrame(Graphics::DirectX* DirectX, Graphics::GraphicsDescriptorHeapManager* GDHManager)
{
    ImGui::Render();
    ID3D12GraphicsCommandList* cmdlist = DirectX->GetCommandList();
    ID3D12DescriptorHeap* heap = GDHManager->GetGraphicsHeap();
    cmdlist->SetDescriptorHeaps(1, &heap);
    ImGui_ImplDX12_RenderDrawData(
        ImGui::GetDrawData(),
        cmdlist);

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}
