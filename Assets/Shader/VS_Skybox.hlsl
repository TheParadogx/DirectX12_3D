cbuffer Transform : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
};

struct VS_INPUT
{
    float4 Pos : POSITION;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float3 TexCoord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    // 座標変換
    // C++側でView行列の移動成分を消しているので回転だけになる
    float4 wPos = mul(input.Pos, World);
    float4 vPos = mul(wPos, View);
    output.Position = mul(vPos, Projection);

    // 深度を 1.0 に固定する
    // z = w にすることで、同次座標系での深度が1.0に。
    output.Position.z = output.Position.w;

    // 頂点のローカル座標を、そのままテクスチャサンプリング用の方向ベクトルとして渡す
    output.TexCoord = input.Pos.xyz;

    return output;
}