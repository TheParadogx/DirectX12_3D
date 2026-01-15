struct VertexInput
{
    float3 Position : POSITION;
    float4 Color : COLOR;
};

cbuffer CameraBuffer : register(b0)
{
	//	ビュー行列
    matrix View;
	//	プロジェクション行列
    matrix Projection;
}

struct VStoPS
{
    float4 Position : SV_Position;
    float4 Color : COLOR;
};

VStoPS VertexMain(VertexInput input)
{
    VStoPS output = (VStoPS)0;
    //カメラ行列適用
    float4 Position = float4(input.Position, 1.0f);
    Position = mul(Position, View);
    Position = mul(Position, Projection);
    output.Position = Position;
    
    output.Color = input.Color;
    
    return output;
}

float4 PixelMain(VStoPS input) : SV_Target0
{
    return input.Color;
}