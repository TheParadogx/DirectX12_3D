#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<wrl/client.h>
#include<memory>
#include<array>

#include"TypeAlias/DirectX12TypeAlias.hpp"
#include"System/Singleton/Singleton.hpp"
#include"Graphics/Color/Color.hpp"

namespace Engine::Graphics
{
	class DirectX : public Singleton<DirectX>
	{
		GENERATE_SINGLETON_BODY(DirectX);

		/// <summary>
		/// 初期値
		/// </summary>
		void OnCreate()override;

		/// <summary>
		/// メモリの解放
		/// </summary>
		void OnDestory()override;
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="WindowHandle">ウィンドウのハンドル</param>
		/// <param name="Width">スクリーン横幅</param>
		/// <param name="Height">スクリーン縦幅</param>
		/// <returns>true:成功</returns>
		bool Initialize(HWND WindowHandle, UINT Width, UINT Height);

		/// <summary>
		/// 描画開始
		/// </summary>
		void BegineRendering();

		/// <summary>
		/// 画面のフリップ
		/// </summary>
		void Flip();

		/// <summary>
		/// GPUの処理待ち
		/// </summary>
		void WaitForGPU();

		/// <summary>
		/// ビューポートの設定
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <param name="width"></param>
		/// <param name="height"></param>
		void SetViewPort(float x, float y, float Width, float Height);

		/// <summary>
		/// Dx12デバイスの取得
		/// </summary>
		/// <returns></returns>
		ID3D12Device* GetDevice()
		{
			return mDevice.Get();
		}

		/// <summary>
		/// Dx12コマンドリストの取得
		/// </summary>
		ID3D12GraphicsCommandList* GetCommandList()
		{
			return mCommandList.Get();
		}

		/// <summary>
		/// Dx12コマンドアロケーターの取得
		/// </summary>
		/// <returns></returns>
		ID3D12CommandAllocator* GetCommandAllocator();
 
		/// <summary>
		/// Dx12コマンドキューの取得
		/// </summary>
		/// <returns></returns>
		ID3D12CommandQueue* GetCommandQueue()
		{
			return mCommandQueue.Get();
		}

		/// <summary>
		/// フレームインデックスの取得
		/// </summary>
		/// <returns></returns>
		int GetFrameIndex()const
		{
			return mFrameIndex;
		}

		/// <summary>
		/// 背景色のセット
		/// </summary>
		/// <param name="BackColor"></param>
		void SetBackColor(const Color& BackColor);

		DXGI_FORMAT GetFormat()
		{
			return mFormat;
		}

	private:
		/// <summary>
		/// デバッグレイヤーの起動
		/// </summary>
		void DebugLayerOn();

		/// <summary>
		/// デバイスの初期化
		/// </summary>
		/// <returns>true:成功</returns>
		bool InitializeDevice();

		/// <summary>
		/// ファクトリーの初期化
		/// </summary>
		/// <returns>true:成功</returns>
		bool InitializeFactory();

		/// <summary>
		/// スワップチェインの初期化
		/// </summary>
		/// <param name="WindowHandle"></param>
		/// <param name="Width"></param>
		/// <param name="Height"></param>
		/// <returns>true:成功</returns>
		bool InitializeSwapChain(HWND WindowHandle, UINT Width, UINT Height);

		/// <summary>
		/// バックバッファ用ディスクリプタヒープの初期化
		/// </summary>
		/// <returns>true:成功</returns>
		bool InitializeBackBufferHeap();

		/// <summary>
		/// 深度バッファ用ディスクリプタヒープの初期化
		/// </summary>
		/// <returns>true:成功</returns>
		bool InitializeDepthHeap();

		/// <summary>
		/// レンダーターゲットの初期化
		/// </summary>
		/// <param name="width"></param>
		/// <param name="height"></param>
		/// <returns>true:成功</returns>
		bool InitializeRenderTarget(UINT Width, UINT Height);

		/// <summary>
		/// フェンスの初期化
		/// </summary>
		/// <returns>true:成功</returns>
		bool InitializeFence();

	public:
		/// <summary>
		/// バックバッファの数
		/// </summary>
		static constexpr int NUM_BACK_BAFFER = 2;
	private:
		/// <summary>
		/// デバイス
		/// </summary>
		mw::DevicePtr mDevice;
		/// <summary>
		/// ファクトリ
		/// </summary>
		mw::Factory6Ptr mFactory;
		/// <summary>
		/// スワップチェイン
		/// </summary>
		mw::SwapChain mSwapChain;
		/// <summary>
		/// コマンドリスト
		/// </summary>
		mw::CmdListPtr mCommandList;
		/// <summary>
		/// コマンドキュー
		/// </summary>
		mw::CmdQueuePtr mCommandQueue;
		/// <summary>
		/// コマンドアロケーター
		/// </summary>
		std::array<mw::CmdAllocatorPtr, NUM_BACK_BAFFER> mCommandAllocator;
		/// <summary>
		/// バックバッファ
		/// </summary>
		std::array<mw::ResourcePtr, NUM_BACK_BAFFER> mBackBaffers;
		/// <summary>
		/// 深度バッファ
		/// </summary>
		mw::ResourcePtr mDepthBuffer;
		/// <summary>
		/// バックバッファ用ヒープ
		/// </summary>
		mw::DescriptorHeapPtr mBackBufferHeap;
		/// <summary>
		/// 深度バッファ用のヒープ
		/// </summary>
		mw::DescriptorHeapPtr mDepthHeap;
		/// <summary>
		/// フェンス
		/// </summary>
		mw::FencePtr mFence;
		/// <summary>
		/// リソース解放検知
		/// </summary>
		mw::DebugDevicePtr mDebugDevice;
		
		/// /// <summary>
		/// リソースバリア
		/// </summary>
		D3D12_RESOURCE_BARRIER mResourceBarrier;
		/// <summary>
		/// GPU待ちイベントハンドル
		/// </summary>
		HANDLE mWaitForGPUEventHandle;

		/// <summary>
		/// 各フェンスのバッファのフェンス値
		/// </summary>
		std::array<UINT64, NUM_BACK_BAFFER> mFenceValues;
		/// <summary>
		/// 次のGPUにSignalする値
		/// </summary>
		UINT64 mNextFenceValue;
		/// <summary>
		/// 今のバックバッファのインデックス
		/// </summary>
		UINT mFrameIndex;

		/// <summary>
		/// 背景色
		/// </summary>
		Color mBackColor;

		/// <summary>
		/// フォーマット
		/// </summary>
		DXGI_FORMAT mFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	};
}