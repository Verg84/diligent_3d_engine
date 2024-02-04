cbuffer Constants
{   // per vertex attributes
    float4x4 ViewProj;
    float4x4 Rotation;
};

struct VSInput
{
    float3 Pos:ATTRIB0;
    float2 UV : ATTRIB1;
    // per instance attributes
    float4 MatRow0 : ATTRIB2;
    float4 MatRow1 : ATTRIB3;
    float4 MatRow2 : ATTRIB4;
    float4 MatRow3 : ATTRIB5;
};

struct PSInput
{
    float4 Pos : SV_POSITION;
    float2 UV : TEX_COORD;
};

void main(in VSInput VSIn, out PSInput PSIn)
{
    float4x4 InsModel = MatrixFromRows(VSIn.MatRow0, VSIn.MatRow1, VSIn.MatRow2, VSIn.MatRow3);
    float4   TransformPos = mul(float4(VSIn.Pos,1.0), Rotation);
    TransformPos             = mul(TransformPos, InsModel);
    PSIn.Pos              = mul(TransformPos, ViewProj);
    PSIn.UV               = VSIn.UV;
}