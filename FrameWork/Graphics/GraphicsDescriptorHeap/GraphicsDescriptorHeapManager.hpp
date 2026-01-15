#pragma once
#include"System/Singleton/Singleton.hpp"
#include"Graphics/DirectX/TypeAlias/DirectX12TypeAlias.hpp"
#include"Graphics/GraphicsDescriptorHeap/GraphicsDescriptorHeapInfo.hpp"
#include<array>

namespace Engine::Graphics
{

	/// <summary>
	/// ヒープ領域の管理をする
	/// </summary>
	class GraphicsDescriptorHeapManager:public Singleton<GraphicsDescriptorHeapManager>
	{
		GENERATE_SINGLETON_BODY(GraphicsDescriptorHeapManager);

		static constexpr int MAX_DESCRIPTOR = 512;

		/// <summary>
		/// 初期値
		/// </summary>
		void OnCreate()override;

		/// <summary>
		/// リソース解放
		/// </summary>
		void OnDestory()override;

	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <returns>true:成功</returns>
		bool Initialize();

		/// <summary>
		/// ヒープから使用領域の発行
		/// </summary>
		/// <param name="size"></param>
		/// <returns></returns>
		[[nodiscard]] DescriptorHeapInfo Issuance(uint32_t Size);

		/// <summary>
		/// 使用領域の破棄
		/// </summary>
		/// <param name="Info"></param>
		void Discard(DescriptorHeapInfo& Info);

		void Set(int RootParameterIndex, const DescriptorHeapInfo& Info);

		D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(const DescriptorHeapInfo& Info)const noexcept;
		D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle(const DescriptorHeapInfo& Info)const noexcept;

		/// <summary>
		/// グラフィックス用ディスクリプタヒープ
		/// </summary>
		/// <returns></returns>
		ID3D12DescriptorHeap* GetGraphicsHeap();

	private:
		struct HandleInfo
		{
			D3D12_CPU_DESCRIPTOR_HANDLE Cpu;
			D3D12_GPU_DESCRIPTOR_HANDLE Gpu;
		};

		mw::DescriptorHeapPtr mGraphicsHeap;
		std::array<HandleInfo, MAX_DESCRIPTOR> mInfo;
		std::array<bool, MAX_DESCRIPTOR> mInUse;
		int mSearchOffset;

	};
}

