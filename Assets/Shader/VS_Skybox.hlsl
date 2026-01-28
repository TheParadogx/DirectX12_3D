// 定数バッファ（RootSignatureのb0）
cbuffer SceneBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
};

struct VS_INPUT
{
    float3 Position : POSITION;
};

struct VS_OUTPUT
{
    float4 SV_Position : SV_POSITION;
    float3 TexCoord    : TEXCOORD; // キューブマップサンプリング用のベクトル
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    // View行列から回転成分だけを取り出す (4x4 を 3x3 にキャスト)
    float3x3 viewRotation = (float3x3) View;
    
    // 回転だけを適用し、移動を無視する
    // mul(input.Position, viewRotation) でもOK。World行列は不要になります。
    float3 rotatedPos = mul(input.Position, viewRotation);
    
    // 投影行列を掛ける
    output.SV_Position = mul(float4(rotatedPos, 1.0f), Projection);

    // 深度値を 1.0 に固定 (これは今のままで完璧です)
    output.SV_Position.z = output.SV_Position.w;

    // ローカル座標をベクトルとして渡す
    output.TexCoord = input.Position;

    return output;
}