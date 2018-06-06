cbuffer cbChangesPerFrame : register(b0) {
	float4 screenDimension;
	matrix wvp;
	float4 data;
};

struct Sprite {
	float2 position;
	float4 textureRect;
	float2 scaling;
	float rotation;
	float4 color;
};

StructuredBuffer<Sprite> SpritesRO : register(t1);

struct GSPS_INPUT {
	float4 Pos : SV_POSITION;
	float4 Tex : COLOR0;
	float3 Size : NORMAL0;
	float4 Color : COLOR1;
};

struct PS_Input {
	float4 pos  : SV_POSITION;
	float2 tex0 : TEXCOORD0;
	float4 color : COLOR0;
};

PS_Input VS_Main(uint id:SV_VERTEXID) {
	PS_Input vsOut = (PS_Input)0;
	uint spriteIndex = id / 4;
	uint vertexIndex = id % 4;	
	float2 t[4];
	float4 ret = SpritesRO[spriteIndex].textureRect;
	float u1 = ret.x / screenDimension.z;
	float v1 = ret.y / screenDimension.w;
	float width = ret.z / screenDimension.z;
	float height = ret.w / screenDimension.w;
	float u2 = u1 + width;
	float v2 = v1 + height;
	t[0] = float2(0, 0);
	t[1] = float2(1, 0);
	t[2] = float2(0, 1);
	t[3] = float2(1, 1);

	float3 position;
	position.x = (vertexIndex % 2) ? 0.5 : -0.5;
	position.y = (vertexIndex & 2) ? -0.5 : 0.5;
	position.z = 0.0;

	position.x *= ret.z;
	position.y *= ret.w;

	float rot = SpritesRO[spriteIndex].rotation;
	float s = sin(rot);
	float c = cos(rot);

	float sx = position.x * SpritesRO[spriteIndex].scaling.x;
	float sy = position.y * SpritesRO[spriteIndex].scaling.y;

	float xt = c * sx - s * sy;
	float yt = s * sx + c * sy;

	float3 sp = float3(SpritesRO[spriteIndex].position, 0.0);

	sp.x -= screenDimension.x;
	sp.y -= screenDimension.y;

	vsOut.pos = mul(float4(xt + sp.x, yt + sp.y, 0.0, 1.0f), wvp);
	vsOut.pos.z = 1.0;
	vsOut.tex0 = t[vertexIndex];
	vsOut.color = SpritesRO[spriteIndex].color;
	return vsOut;
}

Texture2D colorMap : register(t0);
SamplerState colorSampler : register(s0);

float4 PS_Main(PS_Input frag) : SV_TARGET{
	float4 c = colorMap.Sample(colorSampler, frag.tex0) * frag.color;
	float2 nt = frag.tex0 + float2(0.5,0);

	float4 N = (2.0 * (colorMap.Sample(colorSampler, nt))) - 1.0;

	float2 refractOffset = (N.xy + 1) * 60 / 1024;


	//c.rgb *= c.a;
	float2 cp = 2.0 * frag.tex0 - 1.0;

	float r = sqrt(cp.x * cp.x + cp.y * cp.y);
	float4 ret = float4(0,0,0,0);//frag.color;
	if ( r <= 0.3) {
		ret.a = 1.0 - 3.0 * r * r;
		ret.g = (1.0 - r / 0.3) * 0.75;
	}
	else if ( r < 1 ){
		ret.a = 3.0/2.0 *(1.0 - r)*(1.0 - r);
	}
	else {
		ret.a = 0;
	}
	ret.r = 1.0 - r;//ret.a;
	return ret;
	//return colorMap.Sample(colorSampler, frag.tex0 + refractOffset) * frag.color;
	//return float4(refractOffset.r,refractOffset.g,0,1);
}
