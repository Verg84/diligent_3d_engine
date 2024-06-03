struct VSInput
{
    float3 Pos : ATTRIB0;
};

struct VSOutput
{
    float4 Pos : SV_POSITION;
};

void main(
    in VSInput VSIn,
    out VSOutput VSOut
)
{
    VSOut.Pos = float4(VSIn.Pos, 1.0);
}