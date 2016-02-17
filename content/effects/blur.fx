uniform extern float4x4 gWVP;
uniform extern texture gTex;
#define NSAMPLES 16
uniform float BlurStart = 1.0;
uniform float BlurWidth = -0.04;
sampler TexS = sampler_state {
	Texture = <gTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU  = CLAMP;
	AddressV  = CLAMP;
};
struct OutputVS {
	float4 posH   : POSITION0;
	float2 Tex   : TEXCOORD0;
	float4 color0 : COLOR0;
};
OutputVS BlurVS(float3 posL : POSITION0,float2 tex0 : TEXCOORD0 , float4 color : COLOR0) {
	OutputVS outVS = (OutputVS)0;	
	outVS.posH = mul(float4(posL, 1.0f), gWVP);		
	outVS.Tex = tex0;
	outVS.color0 = color;
	return outVS;
}
float4 BlurPS(OutputVS input) : COLOR {
	float4 c = 0;    
	float2 Center = float2(0.5,0.5);
	input.Tex.xy -= Center;
    for(int i=0; i < NSAMPLES; i++) {
    	float scale = BlurStart + BlurWidth*(i/(float) (NSAMPLES-1.0));
    	c += tex2D(TexS, input.Tex.xy*scale + Center );
    }
    c /= NSAMPLES;
    return c;
}
technique BlurTech {
	pass P0 {
		vertexShader = compile vs_2_0 BlurVS();
		pixelShader = compile ps_2_0 BlurPS();
	}
};