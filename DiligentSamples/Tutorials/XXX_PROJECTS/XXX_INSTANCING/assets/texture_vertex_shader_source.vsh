cbuffer Vertex_Uniform_Variables
{
    float4x4 VP;
    float4x4 Rotation;
    
};

struct VSInput 
{
    // Vertex Attributes
    float3 Pos : ATTRIB0;
    // Per Instance Attributes
    float4 Row0 : ATTRIB1;
    float4 Row1 : ATTRIB2;
    float4 Row2 : ATTRIB3;
    float4 Row3 : ATTRIB4;
    
};

struct PSInput
{
    float4 Pos : SV_POSITION;
};

void main(in VSInput VSIn,out PSInput PSIn)
{
    float4x4 InstanceMat = MatrixFromRows(VSIn.Row0, VSIn.Row1, VSIn.Row2, VSIn.Row3);
    
    // Apply Rotation
    float4 Position = mul(float4(VSIn.Pos, 1.0), Rotation);
    
    // Apply Instance Transform
    Position = mul(Position, InstanceMat);
    
    // Apply View - Projection
    PSIn.Pos = mul(Position, VP);

}