#include"structures.fxh"

cbuffer VSConstants
{
    Constants g_constants;
};
struct VSInput
{
    float3 Pos : ATTRIB0;
    float2 UV : ATTRIB1;
};
void main(in VSInput VSIn,out VSOutput VSOut)
{
    VSOut.Pos = mul(float4(VSIn.Pos, 1.0), g_constants.WorldViewProj);
    VSOut.UV = VSIn.UV;

}