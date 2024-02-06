#include <vector>

#include "XXX_Object.hpp"
#include "BasicMath.hpp"
#include "TextureUtilities.h"
#include "GraphicsTypesX.hpp"


namespace Diligent
{
    const std::array<float3, NV> Positions[] = 
    {
        float3{-1, -1, -1}, float3{-1, +1, -1}, float3{+1, +1, -1}, float3{+1, -1, -1}, // Bottom
        float3{-1, -1, -1}, float3{-1, -1, +1}, float3{+1, -1, +1}, float3{+1, -1, -1}, // Front
        float3{+1, -1, -1}, float3{+1, -1, +1}, float3{+1, +1, +1}, float3{+1, +1, -1}, // Right
        float3{+1, +1, -1}, float3{+1, +1, +1}, float3{-1, +1, +1}, float3{-1, +1, -1}, // Back
        float3{-1, +1, -1}, float3{-1, +1, +1}, float3{-1, -1, +1}, float3{-1, -1, -1}, // Left
        float3{-1, -1, +1}, float3{+1, -1, +1}, float3{+1, +1, +1}, float3{-1, +1, +1}  // Top
    };
    
    const std::array<float2, NV> Texcoords[] = 
    {
        float2{0, 1}, float2{0, 0}, float2{1, 0}, float2{1, 1}, // Bottom
        float2{0, 1}, float2{0, 0}, float2{1, 0}, float2{1, 1}, // Front
        float2{0, 1}, float2{1, 1}, float2{1, 0}, float2{0, 0}, // Right
        float2{0, 1}, float2{0, 0}, float2{1, 0}, float2{1, 1}, // Back
        float2{1, 0}, float2{0, 0}, float2{0, 1}, float2{1, 1}, // Left
        float2{1, 1}, float2{0, 1}, float2{0, 0}, float2{1, 0}  // Top
    };
    const std::array<float3, NV> Normals[] = 
    {
        float3{0, 0, -1}, float3{0, 0, -1}, float3{0, 0, -1}, float3{0, 0, -1}, // Bottom
        float3{0, -1, 0}, float3{0, -1, 0}, float3{0, -1, 0}, float3{0, -1, 0}, // Front
        float3{+1, 0, 0}, float3{+1, 0, 0}, float3{+1, 0, 0}, float3{+1, 0, 0}, // Right
        float3{0, +1, 0}, float3{0, +1, 0}, float3{0, +1, 0}, float3{0, +1, 0}, // Back
        float3{-1, 0, 0}, float3{-1, 0, 0}, float3{-1, 0, 0}, float3{-1, 0, 0}, // Left
        float3{0, 0, +1}, float3{0, 0, +1}, float3{0, 0, +1}, float3{0, 0, +1}  // Top
    };

    RefCntAutoPtr<IBuffer> CreateVertexBuffer(IRenderDevice* pDevice, VERTEX_FLAGS VertexFlags, BIND_FLAGS BindFlags, BUFFER_MODE Mode)
    {
        VERIFY_EXPR(VertexFlags != NONE_FLAG);
        const Uint32 TotalVertexComponents =
            ((VertexFlags & POS_FLAG) ? 3 : 0) +
            ((VertexFlags & NORMAL_FLAG) ? 3 : 0) +
            ((VertexFlags & TEXCOORD_FLAG) ? 2 : 0);
        std::vector<float> VertexData(size_t{TotalVertexComponents}*NV);
        auto it=VertexData.begin();
        for (Uint32 v = 0; v < NV; ++v)
        {
            if (VertexFlags & POS_FLAG)
            {
                const auto& Pos{Positions[v]};
                *(it++) = Pos.x;
                *(it++) = Pos.y;
                *(it++) = Pos.z;

            }
        }
    }
    
}
