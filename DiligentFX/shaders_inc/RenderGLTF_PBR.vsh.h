"#include \"BasicStructures.fxh\"\n"
"#include \"GLTF_PBR_VertexProcessing.fxh\"\n"
"\n"
"struct GLTF_VS_Input\n"
"{\n"
"    float3 Pos     : ATTRIB0;\n"
"    float3 Normal  : ATTRIB1;\n"
"    float2 UV0     : ATTRIB2;\n"
"    float2 UV1     : ATTRIB3;\n"
"    float4 Joint0  : ATTRIB4;\n"
"    float4 Weight0 : ATTRIB5;\n"
"};\n"
"\n"
"cbuffer cbCameraAttribs\n"
"{\n"
"    CameraAttribs g_CameraAttribs;\n"
"}\n"
"\n"
"cbuffer cbTransforms\n"
"{\n"
"    GLTFNodeShaderTransforms g_Transforms;\n"
"}\n"
"\n"
"#ifndef MAX_JOINT_COUNT\n"
"#   define MAX_JOINT_COUNT 64\n"
"#endif\n"
"\n"
"cbuffer cbJointTransforms\n"
"{\n"
"    float4x4 g_Joints[MAX_JOINT_COUNT];\n"
"}\n"
"\n"
"void main(in  GLTF_VS_Input  VSIn,\n"
"          out float4 ClipPos  : SV_Position,\n"
"          out float3 WorldPos : WORLD_POS,\n"
"          out float3 Normal   : NORMAL,\n"
"          out float2 UV0      : UV0,\n"
"          out float2 UV1      : UV1)\n"
"{\n"
"    // Warning: moving this block into GLTF_TransformVertex() function causes huge\n"
"    // performance degradation on Vulkan because glslang/SPIRV-Tools are apparently not able\n"
"    // to eliminate the copy of g_Transforms structure.\n"
"    float4x4 Transform = g_Transforms.NodeMatrix;\n"
"    if (g_Transforms.JointCount > 0)\n"
"    {\n"
"        // Mesh is skinned\n"
"        float4x4 SkinMat =\n"
"            VSIn.Weight0.x * g_Joints[int(VSIn.Joint0.x)] +\n"
"            VSIn.Weight0.y * g_Joints[int(VSIn.Joint0.y)] +\n"
"            VSIn.Weight0.z * g_Joints[int(VSIn.Joint0.z)] +\n"
"            VSIn.Weight0.w * g_Joints[int(VSIn.Joint0.w)];\n"
"        Transform = mul(Transform, SkinMat);\n"
"    }\n"
"\n"
"    GLTF_TransformedVertex TransformedVert = GLTF_TransformVertex(VSIn.Pos, VSIn.Normal, Transform);\n"
"\n"
"    ClipPos  = mul(float4(TransformedVert.WorldPos, 1.0), g_CameraAttribs.mViewProj);\n"
"    WorldPos = TransformedVert.WorldPos;\n"
"    Normal   = TransformedVert.Normal;\n"
"    UV0      = VSIn.UV0;\n"
"    UV1      = VSIn.UV1;\n"
"}\n"
