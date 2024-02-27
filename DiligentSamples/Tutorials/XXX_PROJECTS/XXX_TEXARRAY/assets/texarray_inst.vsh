cbuffer Vertex_Uniform_Variables
{
    float4x4 VP;
    float4x4 Rotation;
};

struct VSInput
{
    // Vertex Attributes
    float3 Pos : ATTRIB0;
    float2 UV : ATTRIB1;
    // Per Instance Attributes
    float4 Row0 : ATTRIB2;
    float4 Row1 : ATTRIB3;
    float4 Row2 : ATTRIB4;
    float4 Row3 : ATTRIB5;
    float TexArrayInt : ATTRIB6;
    
};

struct PSInput
{
    float4 Pos : SV_POSITION;
    float2 UV : TEX_COORD;
    float TexArrayInt : TEX_ARRAY_INDEX;
};

void main(in VSInput VSIn, out PSInput PSIn)
{
    float4x4 InstanceMat = MatrixFromRows(VSIn.Row0, VSIn.Row1, VSIn.Row2, VSIn.Row3);
    float4 Position = mul(float4(VSIn.Pos, 1.0), Rotation);
    Position = mul(Position, InstanceMat);
    PSIn.Pos = mul(Position, VP);
    
    PSIn.UV = VSIn.UV;
    PSIn.TexArrayInt = VSIn.TexArrayInt;
    
    

}