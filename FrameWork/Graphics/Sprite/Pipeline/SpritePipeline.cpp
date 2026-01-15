#include "pch.h"
#include "SpritePipeline.hpp"
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

/// <summary>
/// 深度ステンシルの初期設定の取得
/// </summary>
/// <returns></returns>
D3D12_DEPTH_STENCIL_DESC Engine::Graphics::SpritePipeline::GetDepthStencilDesc()
{
    D3D12_DEPTH_STENCIL_DESC Desc = {};
    Desc.DepthEnable = FALSE;
    Desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
    Desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
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
D3D12_BLEND_DESC Engine::Graphics::SpritePipeline::GetBlendStateDesc()
{
    D3D12_BLEND_DESC Desc = {};
    Desc.AlphaToCoverageEnable = FALSE;
    Desc.IndependentBlendEnable = FALSE;
    D3D12_RENDER_TARGET_BLEND_DESC RenderTargetBlendDesc = {};
    RenderTargetBlendDesc.BlendEnable = TRUE;
    RenderTargetBlendDesc.LogicOpEnable = FALSE;
    RenderTargetBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
    RenderTargetBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
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
D3D12_RASTERIZER_DESC Engine::Graphics::SpritePipeline::GetRasterizerDesc()
{
    D3D12_RASTERIZER_DESC Desc = {};
    Desc.FillMode = D3D12_FILL_MODE_SOLID;
    Desc.CullMode = D3D12_CULL_MODE_NONE;
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

/// <summary>
/// ルートシグネチャの作成
/// </summary>
/// <returns>true:成功</returns>
bool Engine::Graphics::SpritePipeline::CreateRootSignature()
{
    //	テクスチャパラメーター設定
    D3D12_DESCRIPTOR_RANGE Range = {};
    Range.BaseShaderRegister = 0;
    Range.NumDescriptors = 1;
    Range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    Range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    //	ルートパラメーター設定
    D3D12_ROOT_PARAMETER RootParam = {};
    RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    RootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    RootParam.DescriptorTable.pDescriptorRanges = &Range;
    RootParam.DescriptorTable.NumDescriptorRanges = 1;

    //	サンプラー設定
    D3D12_STATIC_SAMPLER_DESC SamplerDesc = {};
    SamplerDesc.Filter = D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;
    SamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    SamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    SamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    SamplerDesc.MipLODBias = 0;
    SamplerDesc.MaxAnisotropy = 16;
    SamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    SamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
    SamplerDesc.MinLOD = 0;
    SamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
    SamplerDesc.ShaderRegister = 0;
    SamplerDesc.RegisterSpace = 0;
    SamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    SamplerDesc.MinLOD = 0.0f;
    SamplerDesc.MaxLOD = 0.0f;

    D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc = {};
    RootSignatureDesc.NumParameters = 1;
    RootSignatureDesc.pParameters = &RootParam;
    RootSignatureDesc.NumStaticSamplers = 1;
    RootSignatureDesc.pStaticSamplers = &SamplerDesc;
    RootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    mw::BlobPtr RootSignatureBlob = nullptr;
    mw::BlobPtr ErrorBlob = nullptr;
    HRESULT hr = D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, RootSignatureBlob.GetAddressOf(), ErrorBlob.GetAddressOf());
    if (FAILED(hr))
    {
        return false;
    }

    ID3D12Device* Device = Graphics::DirectX::GetInstance()->GetDevice();
    hr = Device->CreateRootSignature(0, RootSignatureBlob->GetBufferPointer(), RootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&mRootSignature));
    RootSignatureBlob.Reset();
    if (FAILED(hr))
    {
        return false;
    }


    return true;
}

/// <summary>
/// ルートシグネチャの作成
/// </summary>
/// <returns>true:成功</returns>
bool Engine::Graphics::SpritePipeline::CreatePipeline()
{
    auto VS = CompileShader("Assets/Shader/VS_Texture.hlsl", "main", "vs_5_0");
    auto PS = CompileShader("Assets/Shader/PS_Texture.hlsl", "main", "ps_5_0");

    if (VS == nullptr || PS == nullptr)
    {
        return false;
    }

    //  0:座標 1:UV
    D3D12_INPUT_ELEMENT_DESC inputLayout[2] = {};
    for (auto& IL : inputLayout)
    {
        IL.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
        IL.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        IL.InstanceDataStepRate = 0;
    }
    inputLayout[0].SemanticName = "POSITION";
    inputLayout[0].SemanticIndex = 0;
    inputLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;//float3
    inputLayout[0].InputSlot = 0;

    inputLayout[1].SemanticName = "TEXCOORD";
    inputLayout[1].SemanticIndex = 0;
    inputLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;//float2
    inputLayout[1].InputSlot = 0;

    D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
    desc.pRootSignature = mRootSignature.Get();

    //コンパイルした頂点シェーダーの情報を格納
    desc.VS.pShaderBytecode = VS->GetBufferPointer();
    desc.VS.BytecodeLength = VS->GetBufferSize();
    desc.InputLayout.NumElements = ARRAYSIZE(inputLayout);
    desc.InputLayout.pInputElementDescs = inputLayout;

    //コンパイルしたピクセルシェーダーの情報を格納
    desc.PS.pShaderBytecode = PS->GetBufferPointer();
    desc.PS.BytecodeLength = PS->GetBufferSize();

    //	ブレンドステート設定
    desc.BlendState = GetBlendStateDesc();
    //	ラスタライザー設定
    desc.RasterizerState = GetRasterizerDesc();
    //	深度ステンシル設定
    desc.DepthStencilState = GetDepthStencilDesc();

    desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
    desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    //	描画先のレンダーターゲットの設定
    desc.NumRenderTargets = 1;
    desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;

    ID3D12Device* device = Graphics::DirectX::GetInstance()->GetDevice();
    HRESULT hr = device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&mPipelineState));
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}

bool Engine::Graphics::SpritePipeline::Create()
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
ID3D12RootSignature* Engine::Graphics::SpritePipeline::GetRootSignature() const
{
    return mRootSignature.Get();
}

/// <summary>
/// パイプラインステートの取得
/// </summary>
/// <returns></returns>
ID3D12PipelineState* Engine::Graphics::SpritePipeline::GetPipelineState() const
{
    return mPipelineState.Get();
}

/// <summary>
/// 描画方法
/// </summary>
/// <returns>三角形</returns>

D3D_PRIMITIVE_TOPOLOGY Engine::Graphics::SpritePipeline::GetTopology() const
{
    return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
}
