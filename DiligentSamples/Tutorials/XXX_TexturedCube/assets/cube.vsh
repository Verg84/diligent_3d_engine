cbuffer Constants
{
    float4x4 mvp;
};

struct VSInput
{
    float3 Pos : ATTRIB0;
    
};
struct PSInput
{
    float4 Pos : SV_POSITION;
};
void main(in VSInput VSIn,out PSInput PSIn)
{
    PSIn.Pos = mul(float(VSIn.Pos, 1.0), mvp);

}