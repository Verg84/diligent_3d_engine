static const uint SOLID = 0;
static const uint CONVEX = 1;
static const uint CONCAVE = 2;

static const float2 pos[] =
{
    { 0.0000, -1.00 }, // 0
    { 0.150, -0.50 }, // 1
    { 0.150, 0.0 }, // 2
    { 0.150, 0.50 }, // 3
    { 0.00, 1.00 }, // 4
    { -3.00, 1.00 }, // 5
    { -0.165, 0.50 }, // 6
    { -0.165, 0.50 }, // 7
    { -0.165, -0.50 }, // 8
    { -0.300, -1.00 }, // 9
};

static const uint3 tri[] =
{
	// CONVEX CURVE TRIANGLES
    { 0, 1, 2 }, //	0
    { 2, 3, 4 }, //	1
	// CONCANVE CURVE TRIANGLES
    { 5, 6, 7 }, // 2
    { 7, 8, 9 }, // 3
	// FILLED TRIANGLES
    { 4, 5, 6 }, //4
    { 4, 6, 2 }, //5
    { 6, 7, 2 }, //6
    { 7, 8, 2 }, //7
    { 2, 8, 0 }, //8
    { 9, 0, 8 }, //9
};

static uint primAttr[] =
{
    CONVEX, // 0
	CONVEX, // 1
	CONCAVE, // 2
	CONCAVE, // 3
	SOLID, // 4
	SOLID, // 5
	SOLID, // 6
	SOLID, // 7
	SOLID, // 8,
	SOLID // 9
};

struct VertOut
{
    float4 position : SV_POSITION;
};

struct PrimOut
{
    uint triangleType : BLENDINDICES0;
};

struct PixelIn
{
    float4 position : SV_POSITION;
    sample float3 bary : SV_BARYCENTRICS;
    uint triangleType : BLENDINDICES0;
};


float2 computeUV(const float3 bary)
{
    const float u = bary.x * 0 + bary.y * 0.5f + bary.z * 1;
    const float v = bary.x * 0 + bary.y * 0.0f + bary.z * 1;
    return float2(u, v);
}
float computeQuadraticBezierFunction(const float2 uv)
{
    return uv.x * uv.x - uv.y;
}

float4 PSMain(PixelIn p) : SV_TARGET
{
    const uint t = p.triangleType;
    const float2 uv = computeUV(p.bary);
    const float y = computeQuadraticBezierFunction(uv);
    
    if (((t == CONVEX) && (y > 0.0f)) || ((t == CONCAVE) && (y < 0.0f)))
    {
        discard;
    }
    return float4(1, 0, 0, 1);

}