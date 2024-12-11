struct VertexOut
{
    float4 Position : SV_Position;
    float3 nearPoint : TEXCOORD1;
    float3 farPoint : TEXCOORD2;
    float4x4 viewProj : MATRIX;
};