cbuffer Constants 
{
    float4x4 mvp;
};

struct VSInput
{
    float3 Position : ATTRIB0;
    float2 UV : ATTRIB1;
};

struct PSInput
{
    float4 Position : SV_POSITION;
    float2 UV : TEX_COORDS;
};

void main(in VSInput VSIn, out PSInput PSIn)
{
    PSIn.Position = mul(float4(VSIn.Position, 1.0), mvp);
    PSIn.UV       = VSIn.UV;
}