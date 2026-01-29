// 16バイトアライメントを意識した構造
cbuffer VfxConstantBuffer : register(b0)
{
    float4x4 World; // 自作Mathから送られてくる行列
    float4x4 ViewProj; // カメラのビュー・プロジェクション行列
    float4 Color; // セッターで指定した色（RGBA）
};

struct VS_INPUT
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
    float4 Color : COLOR;
};