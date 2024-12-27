cbuffer SkyboxBuffer // UnlitBuffer
{
	matrix wvp;
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
    float4 position : SV_POSITION;
    float4 colour : COLOR0;
    float3 uvw : TEXCOORD0;
};

VOut main(VIn input)
{
    VOut output;
    
    output.position = mul(wvp, float4(input.position, 1.0f));
    output.colour = input.colour;
    output.uvw = input.position.xyz;

    return output;
}