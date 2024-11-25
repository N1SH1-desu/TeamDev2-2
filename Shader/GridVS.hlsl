cbuffer gViewUniforms : register(b0)
{
    row_major float4x4 gViewProj;
    row_major float4x4 gViewProjInverse;
}

#include "Grid.hlsli"

static const float3 gridPlane[6] =
{
    float3(1, 1, 0), float3(-1, -1, 0), float3(-1, 1, 0),
    float3(-1, -1, 0), float3(1, 1, 0), float3(1, -1, 0)
};

float3 UnprojectPoint(float x, float y, float z, float4x4 viewProjInverse)
{
    float4 unprojectedPoint = mul(float4(x, y, z, 1.0f), viewProjInverse);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

VertexOut main(uint vertexId : SV_VertexID)
{
    VertexOut vout;
    
    float3 p = gridPlane[vertexId].xyz;
    
    vout.viewProj = gViewProj;
    
    vout.nearPoint = UnprojectPoint(p.x, p.y, 0.0f, gViewProjInverse).xyz;
    vout.farPoint = UnprojectPoint(p.x, p.y, 1.0f, gViewProjInverse).xyz;
    
    vout.Position = float4(p, 1.0f);
    return vout;
}
