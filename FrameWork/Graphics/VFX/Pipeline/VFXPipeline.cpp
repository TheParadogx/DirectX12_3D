#include "pch.h"
#include "VFXPipeline.hpp"
#include "Graphics/DirectX/DirectX12.hpp"
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
                0, ShaderBlob.GetAddressOf(), ErrorBlob.GetAddressOf());

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
    bool VFXPipeline::Create()
    {
        if (!CreateRootSignature()) {
            LOG_ERROR("Failed CreateRootSignature.");
            return false;
        }
        if (!CreatePipeline()) {
            LOG_ERROR("Failed CreatePipeline.");
            return false;
        }
        return true;
    }

    bool VFXPipeline::CreateRootSignature()
    {
        // パラメータ0: 定数バッファ(b0) [World, ViewProj, Color]
        // パラメータ1: テクスチャ(t0)
        std::array<D3D12_ROOT_PARAMETER, 2> RootParams = {};

        RootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
        RootParams[0].Descriptor.ShaderRegister = 0;
        RootParams[0].Descriptor.RegisterSpace = 0;
        RootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

        //  テクスチャ
        D3D12_DESCRIPTOR_RANGE range = {};
        range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        range.NumDescriptors = 1;
        range.BaseShaderRegister = 0;
        range.RegisterSpace = 0;
        range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

        RootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        RootParams[1].DescriptorTable.NumDescriptorRanges = 1;
        RootParams[1].DescriptorTable.pDescriptorRanges = &range;
        RootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

        D3D12_STATIC_SAMPLER_DESC sampler = {};
        sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
        sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        sampler.MipLODBias = 0.0f;
        sampler.MaxAnisotropy = 1; // 16だと重い場合があるので一旦1で
        sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
        sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
        sampler.MinLOD = 0.0f;
        sampler.MaxLOD = D3D12_FLOAT32_MAX;
        sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
        sampler.ShaderRegister = 0;
        sampler.RegisterSpace = 0;

        D3D12_ROOT_SIGNATURE_DESC rsDesc = {};
        rsDesc.NumParameters = static_cast<UINT>(RootParams.size());
        rsDesc.pParameters = RootParams.data();
        rsDesc.NumStaticSamplers = 1;
        rsDesc.pStaticSamplers = &sampler;
        rsDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

        mw::BlobPtr blob, error;
        HRESULT hr = D3D12SerializeRootSignature(&rsDesc, D3D_ROOT_SIGNATURE_VERSION_1, blob.GetAddressOf(), error.GetAddressOf());
        if (FAILED(hr)) return false;

        ID3D12Device* device = DirectX::GetInstance()->GetDevice();
        hr = device->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&mRootSignature));
        if (FAILED(hr))
        {
            return false;
        }

        return true;
    }

    bool VFXPipeline::CreatePipeline()
    {
        // 先ほど作成したシェーダーファイルとエントリーポイントを指定
        auto VS = CompileShader("Assets/Shader/VS_Vfx.hlsl", "main", "vs_5_0");
        auto PS = CompileShader("Assets/Shader/PS_Vfx.hlsl", "main", "ps_5_0");

        if (VS == nullptr || PS == nullptr) return false;

        std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayouts;
        D3D12_INPUT_ELEMENT_DESC element = {};
        element.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
        element.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        element.InstanceDataStepRate = 0;
        element.InputSlot = 0;

        element.SemanticName = "POSITION";
        element.SemanticIndex = 0;
        element.Format = DXGI_FORMAT_R32G32B32_FLOAT;//float3
        inputLayouts.push_back(element);

        element.SemanticName = "TEXCOORD";
        element.SemanticIndex = 0;
        element.Format = DXGI_FORMAT_R32G32_FLOAT;//float2
        inputLayouts.push_back(element);


        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.pRootSignature = mRootSignature.Get();

        psoDesc.VS = { VS->GetBufferPointer(), VS->GetBufferSize() };
        psoDesc.PS = { PS->GetBufferPointer(), PS->GetBufferSize() };
        psoDesc.InputLayout.pInputElementDescs = inputLayouts.data();
        psoDesc.InputLayout.NumElements = static_cast<UINT>(inputLayouts.size());

        //コンパイルしたピクセルシェーダーの情報を格納
        psoDesc.PS.pShaderBytecode = PS->GetBufferPointer();
        psoDesc.PS.BytecodeLength = PS->GetBufferSize();

        //	ブレンドステート設定
        psoDesc.BlendState = GetBlendStateDesc();
        //	ラスタライザー設定
        psoDesc.RasterizerState = GetRasterizerDesc();
        //	深度ステンシル設定
        psoDesc.DepthStencilState = GetDepthStencilDesc();

        psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
        psoDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

        //	描画先のレンダーターゲットの設定
        for (auto& rtv : psoDesc.RTVFormats)
        {
            rtv = DXGI_FORMAT_UNKNOWN;
        }
        psoDesc.NumRenderTargets = 1;
        for (UINT i = 0; i < psoDesc.NumRenderTargets; i++)
        {
            psoDesc.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM;
        }

        psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
        psoDesc.SampleDesc.Count = 1;
        psoDesc.SampleDesc.Quality = 0;

        ID3D12Device* device = Graphics::DirectX::GetInstance()->GetDevice();
        const HRESULT hr = device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPipelineState));
        if (FAILED(hr))
        {
            LOG_ERROR("Failed CreateGraphicsPipelineState.");
            return false;
        }
        return true;
    }

    D3D12_DEPTH_STENCIL_DESC VFXPipeline::GetDepthStencilDesc()
    {
        D3D12_DEPTH_STENCIL_DESC Desc = {};
        Desc.DepthEnable = TRUE;
        Desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
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

    D3D12_BLEND_DESC VFXPipeline::GetBlendStateDesc()
    {
        D3D12_BLEND_DESC Desc = {};
        Desc.AlphaToCoverageEnable = FALSE;
        Desc.IndependentBlendEnable = FALSE;

        D3D12_RENDER_TARGET_BLEND_DESC rtDesc = {};
        rtDesc.BlendEnable = TRUE; // エフェクトなのでTRUE
        rtDesc.LogicOpEnable = FALSE;

        // --- ここがVFXの肝（加算合成） ---
        rtDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;  // 自分の色にアルファを掛ける
        rtDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;        // 背景の色はそのまま100%使う（足し算）
        rtDesc.BlendOp = D3D12_BLEND_OP_ADD;
        // ------------------------------

        rtDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
        rtDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
        rtDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
        rtDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
        rtDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

        // FBXと同じように全ターゲットに適用（これで構造が一致する）
        for (auto& rt : Desc.RenderTarget)
        {
            rt = rtDesc;
        }
        return Desc;
    }

    D3D12_RASTERIZER_DESC VFXPipeline::GetRasterizerDesc()
    {
        D3D12_RASTERIZER_DESC Desc = {};
        Desc.FillMode = D3D12_FILL_MODE_SOLID;
        Desc.CullMode = D3D12_CULL_MODE_NONE; // 両面表示
        Desc.DepthClipEnable = TRUE;
        return Desc;
    }

    ID3D12RootSignature* VFXPipeline::GetRootSignature()const { return mRootSignature.Get(); }
    ID3D12PipelineState* VFXPipeline::GetPipelineState()const { return mPipelineState.Get(); }

    D3D_PRIMITIVE_TOPOLOGY VFXPipeline::GetTopology()const
    {
        // CreateMeshの設計（4頂点）に合わせる
        //return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    }
}