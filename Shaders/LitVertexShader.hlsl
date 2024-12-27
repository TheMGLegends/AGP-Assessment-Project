#define MAX_LIGHTS 8

cbuffer LitBuffer
{
    matrix wvp;
    float4 ambientLightColour;
    float4 directionalLightColour;
    float4 directionalLightDirection;
    // TODO: Point Lights
	// PointLight pointLights[MAX_LIGHTS];
};

// TODO: This when lights made
//struct PointLight
//{
//    float4 position;
//    float4 colour;
//    float strength;
//    bool enabled;
//    float2 padding;
//};

struct VIn
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
    float2 uv : TEXCOORD0;
};

VOut main(VIn input)
{
    VOut output;
    
    output.position = mul(wvp, float4(input.position, 1.0f));
    output.uv = input.uv;

    float diffuseAmount = dot(directionalLightDirection.xyz, input.normal);

    // INFO: Clamp the value between 0 and 1 to avoid negative lighting
    diffuseAmount = saturate(diffuseAmount);

    // INFO: Adjust directional light colour by the diffuse amount
    float3 directionalFinalColour = directionalLightColour.xyz * diffuseAmount;

    // INFO: Calculate the final colour based on the point lights
    //float3 pointFinalColour = float3(0, 0, 0);
    //for (int i = 0; i < MAX_LIGHTS; ++i)
    //{
    //    if (!pointLights[i].enabled)
    //        continue;
    //
    //    float4 pointLightDirection = float4(normalize(pointLights[i].position.xyz - input.position), 0.0f);
    //    float pointLightDistance = length(pointLights[i].position.xyz - input.position);
    //    float pointLightAttenuation = pointLights[i].strength / (pointLightDistance * pointLightDistance + pointLights[i].strength);
    //    float pointDiffuseAmount = dot(pointLightDirection.xyz, input.normal) * pointLightAttenuation;
    //    pointDiffuseAmount = saturate(pointDiffuseAmount);
    //    pointFinalColour += pointLights[i].colour.xyz * pointDiffuseAmount;
    //}
    
    // INFO: Calculate the final colour based on the ambient light
    output.colour = saturate(ambientLightColour + float4(directionalFinalColour, 1.0f) /*+ float4(pointFinalColour, 1.0f)*/);
    
	return output;
}