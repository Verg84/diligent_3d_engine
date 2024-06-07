#ifndef GROUP_SIZE
#define GROUP_SIZE 32
#endif

struct DrawTask
{
    float2 BasePos;
    float Scale;
    float TimeOffset;
};

struct CubeData
{
    float4 SphereRadius;
    float4 Positions[24];
    float UVs[24];
    uint4 Indices[36 / 3];
};

struct Constants
{
    float4x4 ViewMat;
    float4x4 ViewProjMat;
    float4 Frustum[6];

    float CoTanHalfFov;
    float CurrTime;
    uint FrustumCulling;
    uint Padding;
};

struct Payload
{
    float PosX[GROUP_SIZE];
    float PosY[GROUP_SIZE];
    float PosZ[GROUP_SIZE];
    float Scale[GROUP_SIZE];
    float LOD[GROUP_SIZE];

};
