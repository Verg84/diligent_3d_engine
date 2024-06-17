struct ObjectData
{
    float4 Positions[24];
    uint4 Indices[36 / 3]; // 12 primitives
};

struct Constants
{
    float4x4 ViewMat;
    float4x4 ViewProjMat;
};