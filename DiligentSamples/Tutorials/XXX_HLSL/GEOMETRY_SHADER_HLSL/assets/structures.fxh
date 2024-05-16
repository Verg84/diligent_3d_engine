struct VSOuput
{
    float4 Pos : SV_POSITION;
    float2 UV : TEX_COORDS;
};

struct GSOutput
{
    VSOutput VSOut;
    float3 DistToEdges;
};

struct Constants
{
    float4x4 MVP;
    float3 ViewPortSize;
    float 
};