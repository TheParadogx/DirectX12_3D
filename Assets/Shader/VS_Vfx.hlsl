#include "VfxShader.hlsli"

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    
    // ローカル座標をワールド座標へ、さらにスクリーン座標へ変換
    float4 worldPos = mul(World, float4(input.Pos, 1.0f));
    output.Pos = mul(ViewProj, worldPos);
    
    output.UV = input.UV;
    output.Color = Color; // プログラムからの色をピクセルシェーダーへ渡す
    
    return output;
}