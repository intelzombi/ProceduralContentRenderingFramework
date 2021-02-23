//------
// File:  SphereTreeGS.fx
//
//--------------
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
    float3 viewD : TEXTURE1;
    float4 color : COLOR0;

    
    //uint vid     : vid;
};
//--------------------------------------------------------------------------------------
// Constant buffers 
//--------------------------------------------------------------------------------------
Texture1D gRandomTex;

Texture2DArray gTexArray;
SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

cbuffer ceveryframe
{
	float4 gEyePosW;
	float gGameTime;
	float gTimeStep;
	float4 gLightPosW;
	float4 gLightDirW;
    float4x4 gWorld;
    float4x4 gView;
    float4x4 gProj;
	float4x4 gViewProj; 
    int gFrameParam1;
    float gFrameParam2;
	float gQuadSize;
};

cbuffer cmultipleperframe
{
    float3 g_iFrameParam1;
};

cbuffer cbFixed
{
	
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
PSPointIn SphereVS( VSSpherePointIn input)
{
	PSPointIn vOut;
	float4x4 WVP = gViewProj;
	vOut.pos   = mul(float4(input.pos,1.0f), WVP);
	vOut.tex   = input.tex;
	float4 LightColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	
	// simple lambert
	//vOut.color = dot((input.norm), gLightDirW)*LightColor;
	vOut.norm = input.norm;
	vOut.color = LightColor;
	vOut.viewD = vOut.pos.xyz - gEyePosW.xyz;
	return vOut;
}




//--------------------------------------------------------------------------------------
// PS 
//--------------------------------------------------------------------------------------
float4 PSPlainOldMain(PSPointIn input) : SV_Target
{
	float4 vSpecIC = float4(0.1,0.1,0.1,1.0);
	float Diffuse = saturate(dot(input.norm, gLightDirW.xyz));
	float3 Reflection = normalize(2*Diffuse*input.norm-gLightDirW.xyz);
	float Specular = pow(saturate(dot(Reflection,input.viewD)),8); 
	float4 color = input.color;
	color = gTexArray.Sample( samLinear, float3(input.tex,0)) * Diffuse;
	color = (color + input.color) + (vSpecIC * Specular);
	return color;
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
        //SetDepthStencilState( EnableDepth, 0 );
        SetDepthStencilState( EnableDepthTestWrite, 0 );
    }  
}	

