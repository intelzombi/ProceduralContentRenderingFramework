#ifndef	__GLOBAl_H__
#define __GLOBAL_H__

#include <windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include "resource.h"
#include "SPOT.h"
#include "Camera.h"
#include "Light.h"
#include "SceneGraph.h"
//#include "FSTree.h"
#include "Tree.h"


#pragma warning( disable : 4996 ) 
#include <strsafe.h>
#pragma warning( default : 4996 )

#include <WinBase.h>

#define _USE_MATH_DEFINES
#include <math.h> 

#include	<HighPerformanceCounter.h>

#include	<iostream>
#include    <INIFileReader.h>





unsigned int   uiNumBuffers =1; //Max of 16
unsigned int   uiNumSOBuffers =1; //Max of 4
const unsigned int uiVerticesPerTriangle = 3;
unsigned int   uiNumPointsBuffer =18750;

unsigned int   uiMaxNumSOBufferSize = uiNumPointsBuffer * 3; //Point/Buffer is TriCount/3 then times 3 to create 3 triangles for every triangle in.  
unsigned int   uiStreamOut = 1;
unsigned int   uiDrawCallStep = 100;
unsigned int   uiDrawCallStart = 1;
unsigned int   uiMaxDrawCalls = 190;
UINT64         ui64MemSize = 1;
unsigned int   uiLockCamera = 0;
bool           g_WindowedMode = true;

float          fxMagnification = 4.3f;
float          fyMagnification = 2.25f;


enum PathType {
    CULDISACK,
    LOOP,
    LINE
};

LPTSTR pFxFileLoc;
LPTSTR pSgFileLoc;
LPTSTR pCsvFileLoc;
LPTSTR pTex1FileLoc;
LPTSTR pTex2FileLoc;
LPSTR  pDrawTechniqueName;
LPSTR  pCanopyTechniqueName;
LPSTR  pDrawNoSubdivideTechniqueName;
LPTSTR pTechTemp;
LPSTR  pStreamOutTechniqueName;

static  bool	volatile    g_sbRenderThreadRunning = false;
static  LONG	volatile	g_lvContinueRunningQ = 1;
HANDLE						g_hRenderThreadHandle = 0;


UINT gWindowWidth       = 640;
UINT gWindowHeight      = 480;
HINSTANCE               g_hInst = NULL;
HWND                    g_hWnd = NULL;
D3D10_DRIVER_TYPE       g_driverType = D3D10_DRIVER_TYPE_NULL;
ID3D10Device*           g_pd3dDevice = NULL;
IDXGISwapChain*         g_pSwapChain = NULL;
ID3D10Texture2D*        g_pDepthStencilBuffer;
ID3D10RenderTargetView* g_pRenderTargetView = NULL;
ID3D10DepthStencilView* g_pDepthStencilView;

ID3D10Effect*           g_pEffect = NULL;
ID3D10EffectTechnique*  g_pDrawTechnique = NULL;
ID3D10EffectTechnique*  g_pTreeTechnique = NULL;
ID3D10EffectTechnique*  g_pCanopyTechnique = NULL;
ID3D10EffectTechnique*  g_pStreamOutTechnique = NULL;
ID3D10InputLayout*      g_pVertexLayout = NULL;
ID3D10InputLayout*      g_pCanopyVertexLayout = NULL;
ID3D10InputLayout*      g_pSOVertexLayout = NULL;
ID3D10Buffer*           g_pVertexBuffer = NULL;
ID3D10Buffer**          g_pVertexBufferArray = NULL;
ID3D10Buffer**          g_pIndexBufferArray = NULL;
ID3D10Buffer*           g_pStreamOutVB = NULL;
ID3D10Buffer**          g_pStreamOutVBArray = NULL;

D3D10_BUFFER_DESC*		g_pVbd;
D3D10_BUFFER_DESC*      g_pIbd;
//ID3D10Buffer*          g_pVertexBuffer = NULL;
ID3D10Buffer*          g_pIndexBuffer = NULL;

ID3D10EffectVectorVariable*         g_pEyePosVar                = NULL;
//ID3D10EffectVectorVariable*         g_pLightDirVar              = NULL;
ID3D10EffectVectorVariable*         g_pLightDir1Var              = NULL;
ID3D10EffectVectorVariable*         g_pLightDir2Var              = NULL;
ID3D10EffectVectorVariable*         g_pLightDir3Var              = NULL;
ID3D10EffectVectorVariable*         g_pSpecVariable              = NULL;
ID3D10EffectVectorVariable*         g_pEl1Variable                = NULL;
ID3D10EffectVectorVariable*         g_pEl2Variable                = NULL;
ID3D10EffectVectorVariable*         g_pEl3Variable                = NULL;
ID3D10EffectMatrixVariable*         g_pmatViewProjectionVariable = NULL;
ID3D10EffectShaderResourceVariable* g_pTexArrayVariable         = NULL;
ID3D10EffectScalarVariable*         g_pPowVariable               = NULL;


ID3D10ShaderResourceView*           g_pTexArrayRV               = NULL;
ID3D10ShaderResourceView*           g_pTexCanopyRV               = NULL;
ID3D10Texture2D*                    g_pTex4RV                   = NULL;


RECT					    g_WindowRectangle;

D3DXVECTOR4                 g_EyePos;
//D3DXVECTOR4                 g_LightDir;
D3DXVECTOR4                 g_LightDir1;
D3DXVECTOR4                 g_LightDir2;
D3DXVECTOR4                 g_LightDir3;
D3DXVECTOR3					g_Spec;
D3DXVECTOR3					g_El1;
D3DXVECTOR3					g_El2;
D3DXVECTOR3					g_El3;
float                       g_Pow;
float g_pfaBackColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};

struct CUSTOMVERTEX
{
	D3DXVECTOR3 Position;
    D3DXVECTOR3 Normal;
    D3DXVECTOR2 TexCoord;
};

// Lights
light::Light                        g_Light1;
light::Light                        g_Light2;
light::Light                        g_Light3;

// Camera and movement
camera::Camera                      g_Camera;
D3DXVECTOR3                 g_CameraPos     ;
D3DXVECTOR3                 g_CameraRight   ;
D3DXVECTOR3                 g_CameraUp      ;
D3DXVECTOR3                 g_CameraLook    ;
POINT                       g_OldMousePos;
D3DXVECTOR3                 g_LookAt(0.0f,3.0f,0.0f);
static bool sbLockLook =false;

Tree * g_tree;
unsigned int JohnnyAppleseed =1492;

SG::SceneGraph * g_SceneGraph;

#endif		// __GLOBAL_H__
