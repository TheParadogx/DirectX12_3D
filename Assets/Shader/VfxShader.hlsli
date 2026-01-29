

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


cbuffer VfxConstantBuffer : register(b0)
{  
    //  姿勢行列
    matrix World;
	//	ビュー行列
    matrix View;
	//	プロジェクション行列
    matrix Projection;
    //  色
    float4 Color;
    //  エフェクト用
    float Timer;
    //  パティング
    float3 padding;
};
