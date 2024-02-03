struct VSOutput
{
	float4 Position:SV_POSITION;
	float2 UV:TEX_COORD;
};

struct GSOutput
{
	VSOutput VSOut;
	float3 DistToEdge:DIST_TO_EDGES;
};

struct Constants
{
	float4x4 WordlViewProj;
	float ViewportSize;
	float LineWidth;
};