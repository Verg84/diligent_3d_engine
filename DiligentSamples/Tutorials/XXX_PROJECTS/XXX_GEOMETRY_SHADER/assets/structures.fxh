struct VSOutput
{
    float4 Pos : SV_POSITION;
    float2 UV : TEX_COORD;
};
struct GSOutput
{
    VSOutput VSOut;
    float3 DistToEdges : DIST_TO_EDGES;
};
struct Constants
{
    float4x4 MVP;
    float4 ViewportSize;
    float4 LineWidth;
};