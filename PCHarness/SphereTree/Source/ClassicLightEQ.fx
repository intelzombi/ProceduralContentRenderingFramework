//------
// File:  ClassicLighting.fx
//
//--------------

//----- Constants----------
static const uint MAXLIGHTS = 1;
static const float pi = 3.14159265;
static const float piInv = 0.31830988;

//*************  RENDER STATES **********************************************************
RasterizerState SetCullMode
{
    //CullMode = FRONT;
    //CullMode = BACK;
    CullMode = NONE;
};
RasterizerState rsWireframe { FillMode = WireFrame; };

//------
// Input and output structures
//-----------




struct VSSpherePointIn
{
    float3 pos   : POSITION;
    float3 norm  : NORMAL;
    float2 tex   : TEXCOORD;
    uint VertexId : SV_VertexID;
	//float2 sizeW : SIZE;
    //uint InstanceId : SV_InstanceID;
};


struct PSPointIn
{
    float4 pos   : SV_Position;
    float3 norm  : NORMAL;    
    float2 tex   : TEXTURE0;
    float3 vPos  : TEXTURE1;
	float3 ePos  : TEXTURE2;
    float3 ltDir : TEXTURE3;
    float4 color : COLOR0;

    
    //uint vid     : vid;
};

//--------------------------------------------------------------------------------------
// Constant buffers 
//--------------------------------------------------------------------------------------
Texture1D gRandomTex;

Texture2D gTex;
SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

cbuffer ceveryframe
{
	float4 gEyePosW;  
	//float gGameTime;
	//float gTimeStep;
	float4 gLightDir1W;
	float4 gLightDir2W;
	float4 gLightDir3W;
    float4x4 gWorld;
    float4x4 gView;
    float4x4 gProj;
	float4x4 gViewProj; 
    //int   gFrameParam1;
    //float gFrameParam2;
	//float gQuadSize;
	float  gmPow;
	float3 gSpec;
	float3 gEl;
};

cbuffer cmultipleperframe
{
    float3 g_iFrameParam1;
};

cbuffer cbFixed
{
	//float3 Kd;
	//float3 Ks;
	
	// Texture coordinates used to stretch texture over quad 
	// when we expand point particle into a quad.
	float2 gQuadTexC[4] = 
	{
		float2(0.0f, 1.0f),
		float2(1.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 0.0f)
	};
	
};


//--------------------------------------------------------------------------------------
// State structures
//--------------------------------------------------------------------------------------


DepthStencilState DisableDepth
{
    DepthEnable = FALSE;
    DepthWriteMask = ZERO;
};

DepthStencilState EnableDepth
{
    DepthEnable = TRUE;
    DepthWriteMask = ZERO;
};

DepthStencilState NoDepthWrites
{
    DepthEnable = TRUE;
    DepthWriteMask = ZERO;
};


DepthStencilState EnableDepthTestWrite
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
};

BlendState NoBlending
{
    AlphaToCoverageEnable = FALSE;
    BlendEnable[0] = FALSE;
};

BlendState AdditiveBlending
{
    AlphaToCoverageEnable = FALSE;
    BlendEnable[0] = TRUE;
    SrcBlend = SRC_ALPHA;
    DestBlend = ONE;
    BlendOp = ADD;
    SrcBlendAlpha = ZERO;
    DestBlendAlpha = ZERO;
    BlendOpAlpha = ADD;
    RenderTargetWriteMask[0] = 0x0F;
};



//-------- Sphere ---------

//-------------------
// VS
//-------------------
PSPointIn SphereVS( VSSpherePointIn input)
{
	PSPointIn vOut;
	float4x4 WVP = gViewProj;
	vOut.pos = mul(float4(input.pos,1.0f), WVP);
	vOut.vPos = vOut.pos;
	vOut.tex   = input.tex;
	//vOut.ltDir = mul(gLightDir1W, WVP).xyz;
	vOut.ltDir =gLightDir1W.xyz;
	float4 LightColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	
	// simple lambert
	vOut.norm = input.norm;
	vOut.color = LightColor;
	vOut.ePos  = mul(gEyePosW,WVP).xyz;
	return vOut;
}


//-------- Light Equation ---------
float3 Shade(float3 v,
			 uniform float3 n,
			 uniform float3 Kd,
			 uniform float3 Ks,
			 uniform float m,
			 uniform uint lightCount,
			 uniform float3 l[MAXLIGHTS],
			 uniform float3 EL[MAXLIGHTS])
{
	// float3 v = normalize(pv - p);
	float3 Lo = float3(0.0f, 0.0f, 0.0f);
	for (uint k=0; k < lightCount; k++)
	{
		float3 h = normalize(v + l[k]);
		float cosTh = saturate(dot(n,h));
		float cosTi = saturate(dot(n, l[k]));
		Lo += ((Kd + Ks * pow(cosTh,m)) * EL[k] * cosTi);
	}
	return Lo;
}


//--------------------------------------------------------------------------------------
// PS 
//--------------------------------------------------------------------------------------
float4 PSPlainOldMain(PSPointIn input) : SV_Target
{
    //float3 spec = {0.3f,0.3f,0.3f};
	float4 Kd = gTex.Sample( samLinear, input.tex) * piInv;
	float3 Ks = ((gmPow + 8) * 0.125f * piInv) * gSpec;
	float3 v = normalize(input.vPos-input.ePos);
	float3 lt[1] = {normalize(input.ltDir.xyz)};
    float3  el[1] = {gEl};
    float3 color = Shade( v, normalize(input.norm),Kd.rgb,Ks,gmPow,1,lt,el);
	return float4(color,1.0f);
}


//--------------------------------------------------------------------------------------
// Render  Spheres
//--------------------------------------------------------------------------------------
technique10 SphereTech
{
    pass p0
    {
		SetRasterizerState( SetCullMode );
		//SetRasterizerState( rsWireframe );
		
        SetVertexShader( CompileShader( vs_4_0, SphereVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSPlainOldMain() ) );
        
        SetBlendState(AdditiveBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
        //SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
        //SetDepthStencilState( NoDepthWrites, 0 );
        SetDepthStencilState( EnableDepth, 0 );
        //SetDepthStencilState( EnableDepthTestWrite, 0 );
    }  
}	

