struct VS_INPUT
{
    float3 Position : POSITION;
    float2 UV : TEXCOORD;
    float4 Color : COLOR;
};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD;
    float4 Color : COLOR;
};

cbuffer VfxBuffer : register(b0)
{
    row_major float4x4 matWVP;
    float2 uvOffset;
    float intensity; // 輝度（光の強さ）
};

Texture2D tex : register(t0);
SamplerState sam : register(s0);

// --- 頂点シェーダー ---
PS_INPUT vs_main(VS_INPUT input)
{
    PS_INPUT output;
    // 3D空間への対応：行列演算を必須とする
    output.Position = mul(float4(input.Position, 1.0f), matWVP);
    // UVスクロール：WRAP（繰り返し）前提の加算
    output.UV = input.UV + uvOffset;
    output.Color = input.Color;
    return output;
}

// --- ピクセルシェーダー ---
float4 ps_main(PS_INPUT input) : SV_TARGET
{
    float4 texColor = tex.Sample(sam, input.UV);
    
    // 出力Alphaは 0 に固定（または無視される設定にする）。
    // RGBに「テクスチャ × 頂点カラー × 輝度」を全て乗算して出力する。
    // 描画側（PSO）では「DestColor = SrcColor + DestColor」のBlendStateを必ず使う。    
    float3 rgb = texColor.rgb * input.Color.rgb * input.Color.a * intensity;
    return float4(rgb, 0.0f);
}