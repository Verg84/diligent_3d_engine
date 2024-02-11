cbuffer Constants
{
    float4x4 ViewProj_Mat;
    float4x4 Rotation_Mat;
};

struct VSInput
{
    float3 Pos  : ATTRIB0;
    float2 UV   : ATTRIB1;
    
    float4 MatRow0 : ATTRIB2;
    float4 MatRow1 : ATTRIB3;
    float4 MatRow2 : ATTRIB4;
    float4 MatRow3 : ATTRIB5;
    
    float TexArrayInd : ATTRIB6;
};

struct PSInput
{
    float4 Pos : SV_POSITION;
    float2 UV : TEX_COORD;
    float TexIndex : TEX_ARRAY_INDEX;
};

void main(in VSInput VSIn,out PSInput PSIn)
{
    float4x4 Inst_Model = MatrixFromRows(VSIn.MatRow0, VSIn.MatRow1, VSIn.MatRow2, VSIn.MatRow3);
    float4 TransPos = mul(float4(VSIn.Pos, 1.0),Rotation_Mat);
    // specific instance transformation
    TransPos = mul(TransPos, Inst_Model);
    
    PSIn.Pos = mul(TransPos, ViewProj_Mat);
    PSIn.UV = VSIn.Pos;
    PSIn.TexIndex = VSIn.TexArrayInd;

}