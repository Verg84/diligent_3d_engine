cbuffer Constants
{
    float4x4 g_ViewProj;
    float4x4 g_Rotation;
};

struct VSInput
{
    float3 Pos : ATTRIB0;
   
    
    float4 MatrRow0 :   ATTRIB1;
    float4 MatrRow1 :   ATTRIB2;
    float4 MatrRow2  :   ATTRIB3;
    float4 MatrRow3  :    ATTRIB4;
};

struct PSInput
{
    float4 Pos :    SV_POSITION;
    
};

void main(in VSInput VSIn,out PSInput PSIn)
{
    float4x4 InstanceMatrix = MatrixFromRows(VSIn.MatrRow0, VSIn.MatrRow1, VSIn.MatrRow2, VSIn.MatrRow3);
    
    float4 TransformedPosition = mul(float4(VSIn.Pos, 1.0), g_Rotation);
    TransformedPosition = mul(TransformedPosition, InstanceMatrix);
    
    PSIn.Pos = mul(TransformedPosition, g_ViewProj);
    

}
    