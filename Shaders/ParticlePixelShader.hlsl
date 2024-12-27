struct PIn
{
	float4 position : SV_Position;
	float4 colour : COLOR0;
	float2 uv : TEXCOORD0;
};

float4 main(PIn input) : SV_TARGET
{
	float distanceSquared = input.uv.x * input.uv.x + input.uv.y * input.uv.y;
	clip(1.0f - distanceSquared);
	return input.colour;
}