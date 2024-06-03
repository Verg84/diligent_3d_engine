// per vertex data from the vertex shader
struct GSInput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR0;
};

// per vertex data passed to the rasterizer
struct GSOutput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR0;

};

[maxvertexcount(3)] 
void main(
    triangle GSInput GSIn[3],
    inout TriangleStream<GSOutput> outStream) 
{
    GSOutput Out;
    [unroll(3)] 
    for (int i = 0; i < 3; ++i)
    {
        Out.pos = GSIn[i].pos;
        if (i == 1)
        {
            Out.color = float4(1.0, 0.0, 0.0, 1.0);
        }
        else

        {
            Out.color = float4(0.0, 0.0, 1.0, 1.0);
        }
             
        
       outStream.Append(Out);
    }
}
