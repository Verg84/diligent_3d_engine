struct PSInput
{
    float4 Pos : SV_POSITION;
    float3  Color : COLOR;
};

void main(in uint VertId:SV_VertexID, out PSInput PSIn)
{
    float4 Pos[3];
    Pos[0] = float4(-0.5, -0.5, 0.0, 1.0);
    Pos[1] = float4(0.0, 0.5, 0.0, 1.0);
    Pos[2] = float4(0.5, -0.5, 0.0, 1.0);

    float3 Col[3];
    Col[0] = float4(1.0, 0.0, 0.0);
    Col[1] = float4(0.0, 1.0, 0.0);
    Col[2] = float4(0.0, 0.0, 1.0);

    PSIn.Pos = Pos[VertId];
    PSIn.Color = Col[VertId];
}