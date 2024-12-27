struct PIn
{
    float4 position : SV_Position;
    float4 colour : COLOR0;
    float2 uv : TEXCOORD0;
    float3 reflectedUVW : TEXCOORD1;
};

Texture2D texture0 : register(t0);
TextureCube skybox0 : register(t1);
sampler samplerState : register(s0);

float4 main(PIn input) : SV_TARGET
{
    float4 sampled = texture0.Sample(samplerState, input.uv);
    float4 reflectedSampled = skybox0.Sample(samplerState, input.reflectedUVW);
    // TODO: Change the magic numbers to variables
    float4 combined = ((input.colour * sampled) * 0.9f) + (reflectedSampled * 0.1f);
    
    return saturate(combined);
}