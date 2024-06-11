struct MeshOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
};

float4 PSMain(MeshOutput p) :
    SV_TARGET
{
    return p.color;
}