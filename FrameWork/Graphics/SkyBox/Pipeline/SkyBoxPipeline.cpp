#include"pch.h"
#include "SkyBoxPipeline.hpp"
#include <d3dcompiler.h>

#include"Debug/spdlog/Logger.hpp"
#include"Graphics/DirectX/DirectX12.hpp"

namespace
{
	auto CompileShader = [](const char* FileName, const char* FunctionName, const char* ShaderModel)
		{
			size_t wLen = 0;
			wchar_t wideFileName[256] = {};
			//	ファイル名変換
			mbstowcs_s(&wLen, wideFileName, ARRAYSIZE(wideFileName), FileName, _TRUNCATE);

			//  シェーダー作成
			mw::BlobPtr shader = nullptr;
			mw::BlobPtr error = nullptr;

			const HRESULT hr = D3DCompileFromFile(wideFileName,
				nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
				FunctionName, ShaderModel,
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
				0, shader.GetAddressOf(), error.GetAddressOf());

			if (FAILED(hr))
			{
				LOG_CRITICAL("シェーダー作成失敗");
				error.Reset();
			}

			return shader;

		};

}

namespace Engine::Graphics
{

	/// <summary>
	/// 深度ステンシルの初期設定の取得
	/// </summary>
	/// <returns></returns>
	D3D12_DEPTH_STENCIL_DESC SkyBoxPipeline::GetDepthStencilDesc()
	{
		D3D12_DEPTH_STENCIL_DESC Desc = {};
		//	深度そのものはON
		Desc.DepthEnable = TRUE;
		//	上書きしないようにZERO
		Desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		//	1.0と重なっても描画できるように
		Desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		//	ステンシルは基本使わない
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
	D3D12_BLEND_DESC SkyBoxPipeline::GetBlendStateDesc()
	{
		D3D12_BLEND_DESC Desc = {};
		Desc.AlphaToCoverageEnable = FALSE;
		Desc.IndependentBlendEnable = FALSE;

		D3D12_RENDER_TARGET_BLEND_DESC RenderTargetBlendDesc = {};

		// スカイボックスは基本「上書き」なので、ブレンドはOFFでOK
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
	D3D12_RASTERIZER_DESC SkyBoxPipeline::GetRasterizerDesc()
	{
		D3D12_RASTERIZER_DESC Desc = {};
		Desc.FillMode = D3D12_FILL_MODE_SOLID;

		// 箱の内側を描画するため、カリング（面の間引き）をしない設定にする
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
	/// ルートシグネチャの作成
	/// </summary>
	/// <returns>true:成功</returns>
	bool SkyBoxPipeline::CreateRootSignature()
	{
		// 定数バッファ用の設定 (行列送信用: b0)
		D3D12_ROOT_PARAMETER RootParams[2] = {};

		// あなたの ConstantBuffer::Set の実装に合わせて TYPE_CBV に戻します
		RootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		RootParams[0].Descriptor.ShaderRegister = 0;
		RootParams[0].Descriptor.RegisterSpace = 0;
		RootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

		// テクスチャ(CubeMap)用の設定 (SRV: t0)
		D3D12_DESCRIPTOR_RANGE Range = {};
		Range.BaseShaderRegister = 0;
		Range.NumDescriptors = 1;
		Range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		Range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		RootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		RootParams[1].DescriptorTable.pDescriptorRanges = &Range;
		RootParams[1].DescriptorTable.NumDescriptorRanges = 1;
		RootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // テクスチャはPSで使う

		// サンプラー設定 (CubeMap用)
		D3D12_STATIC_SAMPLER_DESC SamplerDesc = {};
		SamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;

		// キューブマップの境界線でノイズが出ないよう、CLAMPに設定するのがスカイボックスの定石です
		SamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		SamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		SamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;

		SamplerDesc.MipLODBias = 0;
		SamplerDesc.MaxAnisotropy = 1;
		SamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		SamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
		SamplerDesc.MinLOD = 0;
		SamplerDesc.MaxLOD = D3D12_FLOAT32_MAX; // MipMapが効くように最大値へ
		SamplerDesc.ShaderRegister = 0;
		SamplerDesc.RegisterSpace = 0;
		SamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		// シグネチャの構築
		D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc = {};
		RootSignatureDesc.NumParameters = _countof(RootParams); // パラメータ数を2に変更
		RootSignatureDesc.pParameters = RootParams;
		RootSignatureDesc.NumStaticSamplers = 1;
		RootSignatureDesc.pStaticSamplers = &SamplerDesc;
		RootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		mw::BlobPtr RootSignatureBlob = nullptr;
		mw::BlobPtr ErrorBlob = nullptr;

		HRESULT hr = D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, RootSignatureBlob.GetAddressOf(), ErrorBlob.GetAddressOf());
		if (FAILED(hr)) {
			if (ErrorBlob) {
				LOG_ERROR("RootSignature Serialize Error");
			}
			return false;
		}

		ID3D12Device* Device = Graphics::DirectX::GetInstance()->GetDevice();
		hr = Device->CreateRootSignature(0, RootSignatureBlob->GetBufferPointer(), RootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&mRootSignature));

		if (FAILED(hr)) return false;

		return true;

	}


	/// <summary>
	/// 描画パイプラインの作成
	/// </summary>
	/// <returns></returns>
	bool SkyBoxPipeline::CreatePipeline()
	{
		// 1. スカイボックス専用シェーダーのコンパイル
		auto VS = CompileShader("Assets/Shader/VS_Skybox.hlsl", "main", "vs_5_0");
		auto PS = CompileShader("Assets/Shader/PS_Skybox.hlsl", "main", "ps_5_0");

		if (VS == nullptr || PS == nullptr)
		{
			return false;
		}

		// 2. インプットレイアウト設定
		// スカイボックスは頂点の「座標」をサンプリングベクトルとして使うので、UV(TEXCOORD)は不要です
		D3D12_INPUT_ELEMENT_DESC inputLayout[1] = {};
		inputLayout[0].SemanticName = "POSITION";
		inputLayout[0].SemanticIndex = 0;
		inputLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT; // float3 (x, y, z)
		inputLayout[0].InputSlot = 0;
		inputLayout[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		inputLayout[0].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		inputLayout[0].InstanceDataStepRate = 0;

		// 3. パイプラインステートの設定
		D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
		desc.pRootSignature = mRootSignature.Get();

		// 頂点シェーダー
		desc.VS.pShaderBytecode = VS->GetBufferPointer();
		desc.VS.BytecodeLength = VS->GetBufferSize();
		desc.InputLayout.NumElements = ARRAYSIZE(inputLayout);
		desc.InputLayout.pInputElementDescs = inputLayout;

		// ピクセルシェーダー
		desc.PS.pShaderBytecode = PS->GetBufferPointer();
		desc.PS.BytecodeLength = PS->GetBufferSize();

		// 各ステート設定（先ほど作成したメンバ関数を呼び出す）
		desc.BlendState = GetBlendStateDesc();
		desc.RasterizerState = GetRasterizerDesc();
		desc.DepthStencilState = GetDepthStencilDesc();

		desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;

		// トポロジータイプは三角形
		desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		// レンダーターゲット設定
		desc.NumRenderTargets = 1;
		desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;

		// 4. パイプラインステートの生成
		ID3D12Device* device = Graphics::DirectX::GetInstance()->GetDevice();
		HRESULT hr = device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&mPipelineState));

		if (FAILED(hr))
		{
			LOG_CRITICAL("SkyBox Pipeline State Object の作成に失敗しました");
			return false;
		}

		return true;
		return true;
	}

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	bool SkyBoxPipeline::Create()
	{
		bool ret = false;
		ret = this->CreateRootSignature();
		if (ret == false)
		{
			LOG_ERROR("Failed CreateRootSignature.");
			return false;
		}

		ret = this->CreatePipeline();
		if (ret == false)
		{
			LOG_ERROR("Failed CreatePipeline.");
			return false;
		}

		return true;

	}

	/// <summary>
	/// ルートシグネチャの取得
	/// </summary>
	/// <returns></returns>
	ID3D12RootSignature* SkyBoxPipeline::GetRootSignature()const
	{
		return mRootSignature.Get();
	}

	/// <summary>
	/// パイプラインステートの取得
	/// </summary>
	/// <returns></returns>
	ID3D12PipelineState* SkyBoxPipeline::GetPipelineState()const
	{
		return mPipelineState.Get();
	}

	/// <summary>
	/// 描画方法
	/// </summary>
	/// <returns>三角形</returns>
	D3D_PRIMITIVE_TOPOLOGY SkyBoxPipeline::GetTopology()const
	{
		return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}



}