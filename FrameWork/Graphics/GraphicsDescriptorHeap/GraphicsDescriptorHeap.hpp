#pragma once
#include<d3d12.h>
#include"GraphicsDescriptorHeapManager.hpp"

namespace Engine::Graphics 
{
	class GraphicsDescriptorHeap
	{
	public:
		GraphicsDescriptorHeap();
		~GraphicsDescriptorHeap();

		/// <summary>
		/// ヒープの作成
		/// </summary>
		/// <returns>true:成功</returns>
		bool Create();

		/// <summary>
		/// ヒープの解放
		/// </summary>
		void Release();

		/// <summary>
		/// パイプラインへヒープのセット
		/// </summary>
		/// <param name="RootParameterIndex">ルートシグネチャへセットしたパラメーターインデックス</param>
		void Set(int RootParameterIndex);

		D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle()const;
		D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle()const;

	private:
		DescriptorHeapInfo mHeapInfo;
		bool mIsValid;
	};

}