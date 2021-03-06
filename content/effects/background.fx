uniform extern float4x4 gWVP;
float4 start_color = float4(0.086,0.203,0.588,1);
float4 end_color = float4(0.301,0.462,1,1);
float stepY = 10.0;

struct OutputVS {
	float4 posH   : POSITION0;
	float2 tex0   : TEXCOORD0;
	float4 color0 : COLOR0;
};

OutputVS BackgroundVS(float3 posL : POSITION0, float2 tex0 : TEXCOORD0, float4 color : COLOR0) {
	OutputVS outVS = (OutputVS)0;
	outVS.posH = mul(float4(posL, 1.0f), gWVP);
	outVS.tex0 = tex0;
	outVS.color0 = color;
	return outVS;
}

float4 BackgroundPS(OutputVS input) : COLOR{
	float delta = input.tex0.y * 1024.0 / stepY;
	if ( delta < 0.5) {
		return lerp(start_color,end_color,delta * 2.0);
	}
	return lerp(end_color,start_color,(delta - 0.5) / 0.5);
}

technique BackgroundTech {
	pass P0 {
		vertexShader = compile vs_2_0 BackgroundVS();
		pixelShader = compile ps_2_0 BackgroundPS();
	}
};