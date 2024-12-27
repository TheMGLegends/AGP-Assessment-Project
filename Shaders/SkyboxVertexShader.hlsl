cbuffer SkyboxBuffer // UnlitBuffer
{
	matrix wvp;
};

struct VIn
{
	float4 position : POSITION;
	float4 colour : COLOR0;
	//float2 uv : TEXCOORD0;
	//float3 normal : NORMAL;
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
    
    output.position = mul(wvp, input.position);
    output.colour = input.colour;
    output.uvw = input.position.xyz;

    return output;
}