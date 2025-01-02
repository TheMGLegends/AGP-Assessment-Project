cbuffer ParticleVSBuffer
{
    matrix wvp;
    float4 colour;
};

struct VIn // Has to match Model vertex buffer layout
{
    float3 position : POSITION;
    float4 colour : COLOR0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
};

struct VOut
{
    float4 position : SV_Position;
    float4 colour : COLOR0;
    float2 uv : TEXCOORD0;
};

VOut main(VIn input)
{
    VOut output;
    
    output.position = mul(wvp, float4(input.position, 1.0f));
    output.colour = colour;
    output.uv = input.position.xy;
    
    return output;
}