"#ifndef THREAD_GROUP_SIZE\n"
"#   define THREAD_GROUP_SIZE 16\n"
"#endif\n"
"\n"
"Texture3D<float3> g_tex3DCurrentOrderScattering;\n"
"\n"
"RWTexture3D</*format = rgba16f*/float4> g_rwtex3DHighOrderSctr;\n"
"\n"
"[numthreads(THREAD_GROUP_SIZE, THREAD_GROUP_SIZE, 1)]\n"
"void InitHighOrderScatteringCS(uint3 ThreadId  : SV_DispatchThreadID)\n"
"{\n"
"    // Accumulate in-scattering using alpha-blending\n"
"    g_rwtex3DHighOrderSctr[ThreadId] = float4(g_tex3DCurrentOrderScattering.Load( int4(ThreadId, 0) ), 0.0);\n"
"}\n"
