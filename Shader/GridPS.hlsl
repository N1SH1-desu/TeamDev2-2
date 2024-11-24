#include "Grid.hlsli"

float4 Grid(float3 fragPos3D, float scale)
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

float4 main(VertexOut vin) : SV_TARGET
{
    float t = -vin.nearPoint.y / (vin.farPoint.y - vin.nearPoint.y);
    
    float3 fragPos3D = vin.nearPoint + t * (vin.farPoint - vin.nearPoint);
    
    if (t > 0.0)
    {
        return Grid(fragPos3D, 10);
    }
    else
    {
        return (Grid(fragPos3D, 10) * 0.0);
    }
}