#include "Grid.hlsli"

static const float near = 0.01f;
static const float far = 100.0f;

float4 Grid(float3 fragPos3D, float scale, bool drawAxis)
{
    float2 coord = fragPos3D.xz * scale;
    
    float2 derivative = fwidth(coord);
    
    float2 grid = abs(frac(coord - 0.5) - 0.5) / derivative;
    
    float line_ = min(grid.x, grid.y);
    
    float minimumz = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);
    
    float4 color = float4(0.2, 0.2, 0.2, 1.0 - min(line_, 1.0));

    
    if (fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx)
    {
        color.z = 1.0;
    }
    
    if (fragPos3D.z > -0.1 * minimumz && fragPos3D.z < 0.1 * minimumz)
    {
        color.x = 1.0;
    }
    
    return color;
}

float ComputeDepth(float3 pos, float4x4 viewProj)
{
    float4 clipSpacePos = mul(float4(pos, 1.0f), viewProj);
    return (clipSpacePos.z / clipSpacePos.w);
}

float ComputeLinearDepth(float3 pos, float4x4 viewProj)
{
    float4 clipSpacePos = mul(float4(pos, 1.0f),viewProj);
    float clipSpaceDepth = (clipSpacePos.z / clipSpacePos.w) * 2.0 - 1.0;
    float linearDepth = (2.0 * near * far) / (far + near - clipSpaceDepth * (far - near));
    return linearDepth / far;
}

struct PixelOut
{
    float4 color : SV_Target;
    float depth : SV_Depth;
};

PixelOut main(VertexOut vin) : SV_TARGET
{
    PixelOut pout;
    
    float t = -vin.nearPoint.y / (vin.farPoint.y - vin.nearPoint.y);
    
    float3 fragPos3D = vin.nearPoint + t * (vin.farPoint - vin.nearPoint);
    
    pout.depth = ComputeDepth(fragPos3D, vin.viewProj);
    
    float linearDepth = ComputeLinearDepth(fragPos3D, vin.viewProj);
    float fading = max(0, (0.5 - linearDepth));
    
    if (t > 0.0)
    {
        pout.color = (Grid(fragPos3D, 10, true) + (Grid(fragPos3D, 1, true))) * 1.0f;
    }
    else
    {
        //pout.color = (Grid(fragPos3D, 10, true) + (Grid(fragPos3D, 1, true) * 0.0));
        pout.color = float4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    pout.color.a *= fading;
    
    return pout;
}