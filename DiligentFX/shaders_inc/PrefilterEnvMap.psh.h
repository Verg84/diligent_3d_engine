"#include \"GLTF_PBR_PrecomputeCommon.fxh\"\n"
"\n"
"#ifndef OPTIMIZE_SAMPLES\n"
"#   define OPTIMIZE_SAMPLES 1\n"
"#endif\n"
"\n"
"TextureCube  g_EnvironmentMap;\n"
"SamplerState g_EnvironmentMap_sampler;\n"
"\n"
"cbuffer FilterAttribs\n"
"{\n"
"    float4x4 g_RotationUnused;\n"
"\n"
"    float    g_Roughness;\n"
"    float    g_EnvMapDim;\n"
"    uint     g_NumSamples;\n"
"    float    Dummy;\n"
"}\n"
"\n"
"// https://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf\n"
"float3 PrefilterEnvMap( float Roughness, float3 R )\n"
"{\n"
"    float3 N = R;\n"
"    float3 V = R;\n"
"    float3 PrefilteredColor = float3(0.0, 0.0, 0.0);\n"
"    float TotalWeight = 0.0;\n"
"    for( uint i = 0u; i < g_NumSamples; i++ )\n"
"    {\n"
"        float2 Xi = Hammersley2D( i, g_NumSamples );\n"
"        float3 H  = ImportanceSampleGGX( Xi, Roughness, N );\n"
"        float3 L  = 2.0 * dot(V, H) * H - V;\n"
"        float NoL = clamp(dot(N, L), 0.0, 1.0);\n"
"        float VoH = clamp(dot(V, H), 0.0, 1.0);\n"
"        if(NoL > 0.0 && VoH > 0.0)\n"
"        {\n"
"#if OPTIMIZE_SAMPLES\n"
"            // https://placeholderart.wordpress.com/2015/07/28/implementation-notes-runtime-environment-map-filtering-for-image-based-lighting/\n"
"\n"
"            float NoH = clamp(dot(N, H), 0.0, 1.0);\n"
"\n"
"            // Probability Distribution Function\n"
"            float pdf = max(NormalDistribution_GGX(NoH, Roughness) * NoH / (4.0 * VoH), 0.0001);\n"
"            // Slid angle of current smple\n"
"            float OmegaS = 1.0 / (float(g_NumSamples) * pdf);\n"
"            // Solid angle of 1 pixel across all cube faces\n"
"            float OmegaP = 4.0 * PI / (6.0 * g_EnvMapDim * g_EnvMapDim);\n"
"            // Do not apply mip bias as this produces result that are not cosistent with the reference\n"
"            float MipLevel = (Roughness == 0.0) ? 0.0 : max(0.5 * log2(OmegaS / OmegaP), 0.0);\n"
"#else\n"
"            float MipLevel = 0.0;\n"
"#endif\n"
"            PrefilteredColor += g_EnvironmentMap.SampleLevel(g_EnvironmentMap_sampler, L, MipLevel).rgb * NoL;\n"
"            TotalWeight += NoL;\n"
"        }\n"
"    }\n"
"    return PrefilteredColor / TotalWeight;\n"
"}\n"
"\n"
"\n"
"void main(in float4  Pos      : SV_Position,\n"
"          in float3  WorldPos : WORLD_POS,\n"
"          out float4 Color    : SV_Target)\n"
"{\n"
"    float3 R = normalize(WorldPos);\n"
"    Color.rgb = PrefilterEnvMap(g_Roughness, R);\n"
"    Color.a = 0.0;\n"
"}\n"
