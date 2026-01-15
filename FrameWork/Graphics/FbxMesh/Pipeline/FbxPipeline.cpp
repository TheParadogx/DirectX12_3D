#include "pch.h"
#include "FbxPipeline.hpp"
#include"Graphics/DirectX/DirectX12.hpp"

#include <d3dcompiler.h>

namespace 
{
	//	シェーダーコンパイル用処理
	auto CompileShader = [](const char* FileName, const char* FunctionName, const char* ShaderModel)
		{
			size_t wLen = 0;
			wchar_t wideFileName[256] = {};
			//	ファイル名変換
			mbstowcs_s(&wLen, wideFileName, ARRAYSIZE(wideFileName), FileName, _TRUNCATE);

			//	シェーダー作成
			mw::BlobPtr ShaderBlob = nullptr;
			mw::BlobPtr ErrorBlob = nullptr;

			const HRESULT hr = D3DCompileFromFile(wideFileName,
				nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
				FunctionName, ShaderModel,
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
				0, ShaderBlob.GetAddressOf(),ErrorBlob.GetAddressOf());

			if (FAILED(hr))
			{
				OutputDebugString((char*)ErrorBlob->GetBufferPointer());
				ErrorBlob.Reset();
			}

			return ShaderBlob;
		};

}


namespace Engine::Graphics
{		
	/// <summary>
	/// ルートシグネチャの作成
	/// </summary>
	/// <returns>true:成功</returns>
	bool FbxPipeline::CreateRootSignature()
	{
		// パラメーターは2つ（0: 行列用CBV, 1: テクスチャ用Table）
		std::array<D3D12_ROOT_PARAMETER, 2> RootParam = {};

		// --- 0番目: 定数バッファ (CBV) ---
		// DescriptorTable ではなく、直接 CBV として定義する
		RootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		RootParam[0].Descriptor.ShaderRegister = 0; // register(b0) と対応
		RootParam[0].Descriptor.RegisterSpace = 0;
		RootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		// --- 1番目: テクスチャ (SRV) ---
		// テクスチャは今まで通り DescriptorTable を使う
		std::array<D3D12_DESCRIPTOR_RANGE, 1> Range = {};
		Range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		Range[0].NumDescriptors = 1;
		Range[0].BaseShaderRegister = 0; // register(t0)
		Range[0].RegisterSpace = 0;
		Range[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		RootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		RootParam[1].DescriptorTable.pDescriptorRanges = &Range[0];
		RootParam[1].DescriptorTable.NumDescriptorRanges = 1;
		RootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;		//	サンプラー設定
		
		
		D3D12_STATIC_SAMPLER_DESC SamplerDesc = {};
		SamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		SamplerDesc.MinLOD = 0.0f;
		SamplerDesc.MaxLOD = 0.0f;
		SamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		SamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		SamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		SamplerDesc.MipLODBias = 0.0f;
		SamplerDesc.MaxAnisotropy = 1; // 16だと重い場合があるので一旦1で
		SamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // ★必ず NEVER に！
		SamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
		SamplerDesc.MinLOD = 0.0f;
		SamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		SamplerDesc.ShaderRegister = 0; // register(s0)
		SamplerDesc.RegisterSpace = 0;
		SamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;


		D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc = {};
		RootSignatureDesc.pParameters = RootParam.data();
		RootSignatureDesc.NumParameters = static_cast<UINT>(RootParam.size());
		RootSignatureDesc.pStaticSamplers = &SamplerDesc;
		RootSignatureDesc.NumStaticSamplers = 1;
		RootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		mw::BlobPtr RootSignatureBlob = nullptr;
		mw::BlobPtr ErrorBlob = nullptr;
		HRESULT hr = D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, RootSignatureBlob.GetAddressOf(), ErrorBlob.GetAddressOf());
		if (FAILED(hr))
		{
			OutputDebugString((char*)ErrorBlob->GetBufferPointer());
			ErrorBlob.Reset();
			return false;
		}

		ID3D12Device* Device = Graphics::DirectX::GetInstance()->GetDevice();
		hr = Device->CreateRootSignature(0, RootSignatureBlob->GetBufferPointer(), RootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&mRootSignature));
		if (FAILED(hr))
		{
			LOG_ERROR("Failed CreateRootSignature");
			return false;
		}
		return true;
	}

	/// <summary>
	/// 描画パイプラインの作成
	/// </summary>
	/// <returns></returns>
	bool FbxPipeline::CreatePipeline()
	{
		auto VS = CompileShader("Assets/Shader/VS_3D.hlsl", "main", "vs_5_0");
		auto PS = CompileShader("Assets/Shader/PS_3D.hlsl", "main", "ps_5_0");
		if (VS == nullptr || PS == nullptr)
		{
			LOG_ERROR("Failed CompileShader");
			return false;
		}
		
		/*
		* 頂点シェーダー用インプットレイアウト
		*/
		std::vector<D3D12_INPUT_ELEMENT_DESC> InputLayouts;
		D3D12_INPUT_ELEMENT_DESC element = {};
		element.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		element.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		element.InstanceDataStepRate = 0;
		element.InputSlot = 0;

		element.SemanticName = "POSITION";
		element.SemanticIndex = 0;
		element.Format = DXGI_FORMAT_R32G32B32_FLOAT;//float3
		InputLayouts.push_back(element);

		element.SemanticName = "TEXCOORD";
		element.SemanticIndex = 0;
		element.Format = DXGI_FORMAT_R32G32_FLOAT;//float2
		InputLayouts.push_back(element);

		element.SemanticName = "NORMAL";
		element.SemanticIndex = 0;
		element.Format = DXGI_FORMAT_R32G32B32_FLOAT;//float3
		InputLayouts.push_back(element);

		element.SemanticName = "BONE_INDEX";
		element.SemanticIndex = 0;
		element.Format = DXGI_FORMAT_R32G32B32A32_SINT;//int[4]
		InputLayouts.push_back(element);

		element.SemanticName = "WEIGHT";
		element.SemanticIndex = 0;
		element.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;//float[4]
		InputLayouts.push_back(element);

		/*
		* 描画パイプライン設定用
		*/
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDesc = {};
		PipelineStateDesc.pRootSignature = mRootSignature.Get();

		//コンパイルした頂点シェーダーの情報を格納
		PipelineStateDesc.VS.pShaderBytecode = VS->GetBufferPointer();
		PipelineStateDesc.VS.BytecodeLength = VS->GetBufferSize();
		PipelineStateDesc.InputLayout.pInputElementDescs = InputLayouts.data();
		PipelineStateDesc.InputLayout.NumElements = static_cast<UINT>(InputLayouts.size());

		//コンパイルしたピクセルシェーダーの情報を格納
		PipelineStateDesc.PS.pShaderBytecode = PS->GetBufferPointer();
		PipelineStateDesc.PS.BytecodeLength = PS->GetBufferSize();

		//	ブレンドステート設定
		PipelineStateDesc.BlendState = GetBlendStateDesc();
		//	ラスタライザー設定
		PipelineStateDesc.RasterizerState = GetRasterizerDesc();
		//	深度ステンシル設定
		PipelineStateDesc.DepthStencilState = GetDepthStencilDesc();

		PipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		PipelineStateDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		PipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		//	描画先のレンダーターゲットの設定
		for (auto& rtv : PipelineStateDesc.RTVFormats)
		{
			rtv = DXGI_FORMAT_UNKNOWN;
		}
		PipelineStateDesc.NumRenderTargets = 1;
		for (UINT i = 0; i < PipelineStateDesc.NumRenderTargets; i++)
		{
			PipelineStateDesc.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM;
		}

		PipelineStateDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		PipelineStateDesc.SampleDesc.Count = 1;
		PipelineStateDesc.SampleDesc.Quality = 0;

		ID3D12Device* Device = Graphics::DirectX::GetInstance()->GetDevice();
		const HRESULT hr = Device->CreateGraphicsPipelineState(&PipelineStateDesc, IID_PPV_ARGS(&mPipelineState));
		if (FAILED(hr))
		{
			LOG_ERROR("Failed CreateGraphicsPipelineState.");
			return false;
		}
		return true;
	}

	/// <summary>
	/// 深度ステンシルの初期設定の取得
	/// </summary>
	/// <returns></returns>
	D3D12_DEPTH_STENCIL_DESC FbxPipeline::GetDepthStencilDesc()
	{
		D3D12_DEPTH_STENCIL_DESC Desc = {};
		Desc.DepthEnable = TRUE;
		Desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		Desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		Desc.StencilEnable = FALSE;
		Desc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
		Desc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;

		const D3D12_DEPTH_STENCILOP_DESC defaultStencilOp =
		{
			D3D12_STENCIL_OP_KEEP,
			D3D12_STENCIL_OP_KEEP,
			D3D12_STENCIL_OP_KEEP,
			D3D12_COMPARISON_FUNC_NEVER
		};
		Desc.BackFace = defaultStencilOp;
		Desc.FrontFace = defaultStencilOp;

		return Desc;
	}

	/// <summary>
	/// ブレンドステートの初期設定の取得
	/// </summary>
	/// <returns></returns>
	D3D12_BLEND_DESC FbxPipeline::GetBlendStateDesc()
	{
		D3D12_BLEND_DESC Desc = {};
		Desc.AlphaToCoverageEnable = FALSE;
		Desc.IndependentBlendEnable = FALSE;
		D3D12_RENDER_TARGET_BLEND_DESC RenderTargetBlendDesc = {};
		RenderTargetBlendDesc.BlendEnable = FALSE;
		RenderTargetBlendDesc.LogicOpEnable = FALSE;
		RenderTargetBlendDesc.SrcBlend = D3D12_BLEND_ONE;
		RenderTargetBlendDesc.DestBlend = D3D12_BLEND_ZERO;
		RenderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		RenderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		RenderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		RenderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		RenderTargetBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
		RenderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		for (auto& rt : Desc.RenderTarget)
		{
			rt = RenderTargetBlendDesc;
		}
		return Desc;
	}

	/// <summary>
	/// ラスタライザーの初期設定の取得
	/// </summary>
	/// <returns></returns>
	D3D12_RASTERIZER_DESC FbxPipeline::GetRasterizerDesc()
	{
		D3D12_RASTERIZER_DESC Desc = {};
		Desc.FillMode = D3D12_FILL_MODE_SOLID;
		Desc.CullMode = D3D12_CULL_MODE_NONE;
		Desc.FrontCounterClockwise = FALSE;
		Desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		Desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		Desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		Desc.DepthClipEnable = TRUE;
		Desc.MultisampleEnable = FALSE;
		Desc.AntialiasedLineEnable = FALSE;
		Desc.ForcedSampleCount = 0;
		Desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

		return Desc;
	}

	/// <summary>
	/// 生成
	/// </summary>
	/// <returns>true:成功</returns>
	bool FbxPipeline::Create()
	{
		bool ret = false;
		
		//	ルートシグネチャの作成
		ret = this->CreateRootSignature();
		if (ret == false)
		{
			return false;
		}
		//	パイプラインの作成
		ret = this->CreatePipeline();
		if (ret == false)
		{
			return false;
		}

		return true;
	}

	/// <summary>
	/// ルートシグネチャの取得
	/// </summary>
	/// <returns></returns>
	ID3D12RootSignature* FbxPipeline::GetRootSignature()const
	{
		return mRootSignature.Get();
	}

	/// <summary>
	/// パイプラインステートの取得
	/// </summary>
	/// <returns></returns>
	ID3D12PipelineState* FbxPipeline::GetPipelineState()const
	{
		return mPipelineState.Get();
	}

	/// <summary>
	/// 描画方法
	/// </summary>
	/// <returns>三角形</returns>
	D3D_PRIMITIVE_TOPOLOGY FbxPipeline::GetTopology()const
	{
		return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}



}