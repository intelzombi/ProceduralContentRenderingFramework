//------
// File:  TreeOrchardGS.fx
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




struct VSTreePointIn
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
    float2 tex   : TEXTURE0;
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

//-------- Tree ---------
PSPointIn TreeVS( VSTreePointIn input)
{
	PSPointIn vOut;
	float4x4 WVP = gViewProj;
	vOut.pos   = mul(float4(input.pos,1.0f), WVP);
	vOut.tex   = input.tex;
//	if (input.VertexId%121 < 13)
//	{
//		vOut.color = float4(0.0f, 1.0f, 1.0f, 1.0f);
//	}else{
//		vOut.color = float4(1.0f, 0.0f, 0.0f, 1.0f);
//	}
	//float4 LightColor = float4(0.0f, 0.50f, 0.0f, 1.0f);
	//float4 LightColor2 = float4(0.10f, 0.0f, 0.0f, 1.0f);
	float4 LightColor = float4(0.0f, 0.50f, 0.30f, 1.0f);
	float4 LightColor2 = float4(0.0f, 0.10f, 0.10f, 1.0f);
	
	vOut.color = 0.1f *(dot((input.norm*2 - 1), gEyePosW)*LightColor2) + (dot((input.norm*2 - 1), gLightDirW)*LightColor);
	//vOut.color = dot((input.norm*2 - 1), gLightDirW)*LightColor;

	//vOut.color = float4(0.0f, 1.0f, 0.0f, 1.0f);
	return vOut;
}




//--------------------------------------------------------------------------------------
// PS 
//--------------------------------------------------------------------------------------
float4 PSPlainOldMain(PSPointIn input) : SV_Target
{
   // return  gTexArray.Sample( samLinear, float3(input.tex,0)) * input.color;
    return  input.color;
    //float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	//return color;
}


//--------------------------------------------------------------------------------------
// Render  Trees
//--------------------------------------------------------------------------------------
technique10 TreeTech
{
    pass p0
    {
		SetRasterizerState( SetCullMode );
		//SetRasterizerState( rsWireframe );
		
        SetVertexShader( CompileShader( vs_4_0, TreeVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSPlainOldMain() ) );
        
        //SetBlendState(AdditiveBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
        SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
        //SetDepthStencilState( NoDepthWrites, 0 );
        //SetDepthStencilState( EnableDepth, 0 );
        SetDepthStencilState( EnableDepthTestWrite, 0 );
    }  
}	

