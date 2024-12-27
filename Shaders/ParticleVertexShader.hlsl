cbuffer ParticleBuffer
{
    matrix wvp;
    float4 colour;
};

struct VOut
{
    float4 position : SV_Position;
    float4 colour : COLOR0;
    float2 uv : TEXCOORD0;
};

VOut main(float3 position : POSITION)
{
    VOut output;
    
    output.position = mul(wvp, float4(position, 1.0f));
    output.colour = colour;
    output.uv = position.xy;
    
    return output;
}