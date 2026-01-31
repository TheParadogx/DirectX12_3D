#include "Header3D.hlsli"

//	メインテクスチャ
Texture2D MainTexture : register(t0);
SamplerState Sampler : register(s0);

float ToonShadow(float DotValue)
{
    if (DotValue >= 0.5f)
    {
        return 1.0f;
    }
    else
    {
        return 0.5f;
    }
}

float4 main(VStoPS input) : SV_Target0
{
// 1. テクスチャの色を取得
    float4 texColor = MainTexture.SampleLevel(Sampler, input.UV, 0);
    
    // 2. テクスチャの色に定数バッファの色(MeshColor)を掛ける
    // これで SetColor の色が反映されるようになります！
    float4 OutputColor = texColor * input.Color;
    
    // --- ライティング計算 ---
    float3 LightVector = normalize(float3(1.0f, 1.0f, 1.0f)); // 逆転計算済みとする
    float d = dot(LightVector, input.Normal);
    d = d * 0.5f + 0.5f;
    float shadow = saturate(d + 0.2f);
    
    OutputColor.rgb *= shadow;
    
    return OutputColor;
}
