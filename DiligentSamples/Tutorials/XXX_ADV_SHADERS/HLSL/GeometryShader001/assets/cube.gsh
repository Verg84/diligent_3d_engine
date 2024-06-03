struct VSOutput
{
    float4 Pos : SV_POSITION;
};
struct GSOutput
{
    VSOutput VSOut;
  
};
    [maxvertexcount(3)] 
    void main(
	triangle VSOutput In[3],
	inout TriangleStream<GSOutput> triStream)
{
    GSOutput Out;
   
    
    Out.VSOut = In[0];
    triStream.Append(Out);
    Out.VSOut = In[1];
    triStream.Append(Out);
    Out.VSOut = In[2];
    triStream.Append(Out);
    
    }
    