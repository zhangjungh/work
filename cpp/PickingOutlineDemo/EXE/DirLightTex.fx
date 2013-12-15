///////////////////////////////////////////////////////////////////////////////
// Per-pixel light effect with texture
///////////////////////////////////////////////////////////////////////////////

struct Material //the same with D3D Material
{
	float4 diffuse;
	float4 ambient;
	float4 spec;
	float4 emissive;
	float  specPower;
};

struct DirLight //light information
{
	float4 ambient;
	float4 diffuse;
	float4 spec;
	float3 dir;  
};

uniform extern float4x4 gWorld;
uniform extern float4x4 gWorldInvTrans;
uniform extern float4x4 gWVP;
uniform extern Material gMtrl;
uniform extern DirLight gLight;
uniform extern float3   gCameraPos;
uniform extern texture  gTex;

sampler2D TexSample = sampler_state
{
	Texture = <gTex>;
	MinFilter = Anisotropic;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	MaxAnisotropy = 16;
};
 
struct OutputVS
{
    float4 position    	: POSITION0;
    float3 normal 		: TEXCOORD0;
    float3 viewDir  	: TEXCOORD1;
    float2 texUV   		: TEXCOORD2;
};

OutputVS DirLightTexVS(float3 position : POSITION0, float3 normal : NORMAL0, float2 texUV: TEXCOORD0)
{
	OutputVS OUT;
	
	float3 worldPos  = mul(float4(position, 1.0f), gWorld).xyz;
	OUT.viewDir = gCameraPos - worldPos;
	
	OUT.normal = mul(float4(normal, 0.0f), gWorldInvTrans).xyz;
	OUT.position = mul(float4(position, 1.0f), gWVP);
	OUT.texUV = texUV;

    return OUT;
}

float4 DirLightTexPS(float3 normal : TEXCOORD0, float3 viewDir  : TEXCOORD1, float2 texUV : TEXCOORD2) : COLOR
{
	normal = normalize(normal);
	viewDir  = normalize(viewDir);
	
	float3 lightVecW = -gLight.dir;
	
	float3 r = reflect(-lightVecW, normal);
	float t  = pow(max(dot(r, viewDir), 0.0f), gMtrl.specPower);
	float s = max(dot(lightVecW, normal), 0.0f);
	
	float3 spec = t*(gMtrl.spec*gLight.spec).rgb;
	float3 diffuse = s*(gMtrl.diffuse*gLight.diffuse).rgb;
	float3 ambient = gMtrl.ambient*gLight.ambient;
	
	float4 texColor = tex2D(TexSample, texUV);
	
	// Combine the color from lighting with the texture color.
	float3 color = (ambient + diffuse)*texColor.rgb + spec;
		
	// Sum all the terms together and copy over the diffuse alpha.
    return float4(color, gMtrl.diffuse.a*texColor.a);
}

technique DirLightTexTech
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
        vertexShader = compile vs_2_0 DirLightTexVS();
        pixelShader  = compile ps_2_0 DirLightTexPS();
    }
}