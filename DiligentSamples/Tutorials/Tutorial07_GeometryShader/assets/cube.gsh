#include"structures.fxh"

cbuffer GSConstants {
    Constants g_Constants;
};

[maxvertexcount(3)] 
void main(triangle VSOutput In[3],inout TriangleStream<GSOuput> triStream)
{
    // Compute Screen-Space Position of each vertex
    float2 v0 = g_Constants.ViewportSize.xyz * In[0].Pos.xy / In[0].Pos.w;
    float2 v1 = g_Constants.ViewportSize.xyz * In[1].Pos.xy / In[1].Pos.w;
    float2 v2 = g_Constants.ViewportSize.xyz * In[2].Pos.xy / In[2].Pos.w;

    float edge0 = v2 - v1;
    float edge1 = v2 - v0;
    float edge2 = v1 - v0;

    // Compute Triangle Area
    float area = abs(edge1.x * edge2.y - edge1.y * edge2.x);

    GSOutput Out;

    Out.VSOut = In[0];
    // Distance to edge 0
    Out.DistToEdges = float3(area / length(edge0), 0.0, 0.0);
    triStream.Append(Out);

    Out.VSOut = In[1];
    // Distance to edge 1
    Out.DistToEdges = float(0.0, area / length(edge1), 0.0);
    triStream.Append(Out);

    Out.VSOut = In[2];
    // Distance to edge 2
    Out.DistToEdges = float(0.0, 0.0, area / length(edge2));
    triStream.Append(Out);
}