#include "pch.h"
#include "LinePipeline.hpp"
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
				0, &ShaderBlob, &ErrorBlob);

			if (FAILED(hr))
			{
				ErrorBlob.Reset();
			}
			return ShaderBlob;
		};

}


namespace Engine::Graphics
{

	/// <summary>
	/// 作成
	/// </summary>
	/// <returns>true:成功</returns>
	bool LinePipeline::Create()
	{
		bool ret = false;
		ret = this->CreateRootSignature();
		if (ret == false)
		{
			return false;
		}

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
	ID3D12RootSignature* LinePipeline::GetRootSignature()const
	{
		return mRootSignature.Get();
	}

	/// <summary>
	/// パイプラインステートの取得
	/// </summary>
	/// <returns></returns>
	ID3D12PipelineState* LinePipeline::GetPipelineState()const 
	{
		return mPipelineState.Get();
	}

	/// <summary>
	/// 描画方法
	/// </summary>
	/// <returns>三角形</returns>
	D3D_PRIMITIVE_TOPOLOGY LinePipeline::GetTopology()const
	{
		return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
	}

	/// <summary>
	/// 深度ステンシルの初期設定の取得
	/// </summary>
	/// <returns></returns>
	D3D12_DEPTH_STENCIL_DESC LinePipeline::GetDepthStencilDesc()
	{
		D3D12_DEPTH_STENCIL_DESC Desc = {};
		Desc.DepthEnable = FALSE;
		Desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		Desc.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		Desc.StencilEnable = FALSE;
		Desc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
		Desc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;

		Desc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		Desc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		Desc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		Desc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		Desc.BackFace = Desc.FrontFace;

		return Desc;

	}


	/// <summary>
	/// ブレンドステートの初期設定の取得
	/// </summary>
	/// <returns></returns>
	D3D12_BLEND_DESC  LinePipeline::GetBlendStateDesc()
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
	D3D12_RASTERIZER_DESC  LinePipeline::GetRasterizerDesc()
	{
		D3D12_RASTERIZER_DESC Desc = {};
		Desc.FillMode = D3D12_FILL_MODE_SOLID;
		Desc.CullMode = D3D12_CULL_MODE_BACK;
		Desc.FrontCounterClockwise = FALSE;
		Desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		Desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		Desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		Desc.DepthClipEnable = FALSE;
		Desc.MultisampleEnable = FALSE;
		Desc.AntialiasedLineEnable = FALSE;
		Desc.ForcedSampleCount = 0;
		Desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

		return Desc;
	}

	bool LinePipeline::CreateRootSignature()
	{

		D3D12_ROOT_PARAMETER RootParam = {};
		RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // ここを CBV に変更！
		RootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
		RootParam.Descriptor.ShaderRegister = 0; // b0 レジスタ
		RootParam.Descriptor.RegisterSpace = 0;

		D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc = {};
		RootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		RootSignatureDesc.pParameters = &RootParam;
		RootSignatureDesc.NumParameters = 1;

		//	ルートシグネチャ設定
		mw::BlobPtr RootSignatureBlob = nullptr;
		mw::BlobPtr ErrorBlob = nullptr;
		HRESULT hr = D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, RootSignatureBlob.GetAddressOf(), ErrorBlob.GetAddressOf());
		if (FAILED(hr))
		{
			LOG_CRITICAL("Failed SerializeRootSignature.");
			ErrorBlob.Reset();
			return false;
		}

		ID3D12Device* Device = Graphics::DirectX::GetInstance()->GetDevice();
		hr = Device->CreateRootSignature(0, RootSignatureBlob->GetBufferPointer(), RootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&mRootSignature));
		RootSignatureBlob.Reset();
		if (FAILED(hr))
		{
			LOG_CRITICAL("Failed CreateRootSignature.");
			return false;
		}

		return true;
	}

	/// <summary>
	/// 描画パイプラインの作成
	/// </summary>
	/// <returns></returns>
	bool LinePipeline::CreatePipeline()
	{
		auto VS = CompileShader("Assets/Shader/Line.hlsl", "VertexMain", "vs_5_0");
		auto PS = CompileShader("Assets/Shader/Line.hlsl", "PixelMain", "ps_5_0");

		if (VS == nullptr || PS == nullptr)
		{
			return false;
		}

		D3D12_INPUT_ELEMENT_DESC InputLayout[2] = {};
		//	頂点シェーダーへの入力情報の設定
		for (auto& IL : InputLayout)
		{
			IL.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
			IL.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			IL.InstanceDataStepRate = 0;
			IL.SemanticIndex = 0;
			IL.InputSlot = 0;
		}

		InputLayout[0].SemanticName = "POSITION";
		InputLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;//float3

		InputLayout[1].SemanticName = "COLOR";
		InputLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;//float4

		D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDesc = {};

		PipelineStateDesc.pRootSignature = mRootSignature.Get();

		//コンパイルした頂点シェーダーの情報を格納
		PipelineStateDesc.VS.pShaderBytecode = VS->GetBufferPointer();
		PipelineStateDesc.VS.BytecodeLength = VS->GetBufferSize();
		PipelineStateDesc.InputLayout.NumElements = ARRAYSIZE(InputLayout);
		PipelineStateDesc.InputLayout.pInputElementDescs = InputLayout;

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
		PipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;

		//	描画先のレンダーターゲットの設定
		PipelineStateDesc.NumRenderTargets = 1;
		PipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		PipelineStateDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		PipelineStateDesc.SampleDesc.Count = 1;
		PipelineStateDesc.SampleDesc.Quality = 0;


		ID3D12Device* Device = Graphics::DirectX::GetInstance()->GetDevice();
		HRESULT hr = Device->CreateGraphicsPipelineState(&PipelineStateDesc, IID_PPV_ARGS(&mPipelineState));

		if (FAILED(hr))
		{
			LOG_CRITICAL("Failed CreateGraphicsPipelineState.");
			return false;
		}
		return true;

	}

}