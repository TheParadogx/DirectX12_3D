#include "pch.h"
#include "VFXPipeline.hpp"
#include "Graphics/DirectX/DirectX12.hpp"
#include <d3dcompiler.h>

namespace
{
    // シェーダーコンパイル用の共通ラムダ
    auto CompileShader = [](const char* FileName, const char* EntryPoint, const char* Target)
        {
            mw::BlobPtr shader = nullptr;
            mw::BlobPtr error = nullptr;

            wchar_t widePath[256];
            size_t converted;
            mbstowcs_s(&converted, widePath, FileName, _TRUNCATE);

            HRESULT hr = D3DCompileFromFile(
                widePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
                EntryPoint, Target,
                D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
                0, &shader, &error);

            if (FAILED(hr)) {
                if (error) {
                    LOG_ERROR((char*)error->GetBufferPointer());
                }
                return mw::BlobPtr(nullptr);
            }
            return shader;
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
        D3D12_ROOT_PARAMETER RootParams[2] = {};

        RootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
        RootParams[0].Descriptor.ShaderRegister = 0;
        RootParams[0].Descriptor.RegisterSpace = 0;
        RootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

        D3D12_DESCRIPTOR_RANGE range = {};
        range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        range.NumDescriptors = 1;
        range.BaseShaderRegister = 0;
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
        sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
        sampler.MaxLOD = D3D12_FLOAT32_MAX;
        sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

        D3D12_ROOT_SIGNATURE_DESC rsDesc = {};
        rsDesc.NumParameters = _countof(RootParams);
        rsDesc.pParameters = RootParams;
        rsDesc.NumStaticSamplers = 1;
        rsDesc.pStaticSamplers = &sampler;
        rsDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

        mw::BlobPtr blob, error;
        if (FAILED(D3D12SerializeRootSignature(&rsDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blob, &error))) return false;

        ID3D12Device* device = DirectX::GetInstance()->GetDevice();
        return SUCCEEDED(device->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&mRootSignature)));
    }

    bool VFXPipeline::CreatePipeline()
    {
        // 先ほど作成したシェーダーファイルとエントリーポイントを指定
        auto VS = CompileShader("Assets/Shader/VS_Vfx.hlsl", "main", "vs_5_0");
        auto PS = CompileShader("Assets/Shader/PS_Vfx.hlsl", "main", "ps_5_0");

        if (VS == nullptr || PS == nullptr) return false;

        // VfxVertex構造体 { Vector3, Vector2 } に合わせる
        D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        };

        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.pRootSignature = mRootSignature.Get();
        psoDesc.VS = { VS->GetBufferPointer(), VS->GetBufferSize() };
        psoDesc.PS = { PS->GetBufferPointer(), PS->GetBufferSize() };
        psoDesc.InputLayout = { inputLayout, _countof(inputLayout) };
        psoDesc.BlendState = GetBlendStateDesc();
        psoDesc.RasterizerState = GetRasterizerDesc();
        psoDesc.DepthStencilState = GetDepthStencilDesc();

        // VfxMesh::RenderのIASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP)に合わせる
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

        psoDesc.NumRenderTargets = 1;
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
        psoDesc.SampleDesc.Count = 1;

        ID3D12Device* device = DirectX::GetInstance()->GetDevice();
        return SUCCEEDED(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPipelineState)));
    }

    D3D12_DEPTH_STENCIL_DESC VFXPipeline::GetDepthStencilDesc()
    {
        D3D12_DEPTH_STENCIL_DESC Desc = {};
        Desc.DepthEnable = TRUE;                            // 背景に隠れるようにテストは有効
        Desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;  // エフェクト自身の厚みはZバッファに書かない
        Desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
        Desc.StencilEnable = FALSE;
        return Desc;
    }

    D3D12_BLEND_DESC VFXPipeline::GetBlendStateDesc()
    {
        D3D12_BLEND_DESC Desc = {};
        Desc.AlphaToCoverageEnable = FALSE;
        Desc.IndependentBlendEnable = FALSE;

        D3D12_RENDER_TARGET_BLEND_DESC& rt = Desc.RenderTarget[0];
        rt.BlendEnable = TRUE;

        // 加算合成 (SrcAlphaに応じて加算)
        rt.SrcBlend = D3D12_BLEND_SRC_ALPHA;
        rt.DestBlend = D3D12_BLEND_ONE;
        rt.BlendOp = D3D12_BLEND_OP_ADD;
        rt.SrcBlendAlpha = D3D12_BLEND_ONE;
        rt.DestBlendAlpha = D3D12_BLEND_ZERO;
        rt.BlendOpAlpha = D3D12_BLEND_OP_ADD;
        rt.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

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
        return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
    }
}