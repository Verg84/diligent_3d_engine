// input per vertex data
struct VSInput
{
    float4 pos : ATTRIB0;
};

// output to the geometry shader
struct VSOutput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR0;
};

void main(
    in VSInput VSIn,
    out VSOutput VSOut
)
{
    VSOut.pos = VSIn.pos;
    VSOut.color = float4(0.0f, 1.0f, 0.0f, 1.0f);
}

