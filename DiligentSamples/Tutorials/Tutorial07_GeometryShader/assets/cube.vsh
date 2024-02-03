#include"structures.fxh"

cbuffer VSConstants 
{
    Constants g_Constants;
};

struct VSInput
{
    float3 Position : ATTRIB0;
    float2 UV:ATTRIB1;
};

void main(in VSInput VSIn, out VSOuput VSOut)
{
    VSOut.Position = mul(float4(VSIn.Position, 1.0), g_Constants.WorldViewProj);
}