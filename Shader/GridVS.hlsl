cbuffer cbPerScene : register(b0)
{
    row_major float4x4 viewProjMat;
    row_major float4x4 invViewProjMat;
}

float4 main(uint vertexID : SV_VertexID) : SV_POSITION
{
    const float3 Pos[4] =
    {
        float3(-1.0f, -1.0f, 0.0f),
        float3(1.0f, -1.0f, 0.0f),
        float3(1.0f, 1.0f, 0.0f),
        float3(-1.0f, 1.0f, 0.0f),
    };

    const int indices[6] = { 0, 2, 1, 2, 0, 3 };
    
    int index = indices[vertexID];
    float4 vertex = float4(Pos[index], 1.0f);
    return vertex;
}