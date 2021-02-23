// TreeOrchard.cpp : Defines the entry point for the application.
/*  *********************************************************************************************  *\
Copyright (c) Intel Corporation (2009).
INTEL MAKES NO WARRANTY OF ANY KIND REGARDING THIS CODE.  THIS CODE IS LICENSED
ON AN "AS IS" BASIS AND INTEL WILL NOT PROVIDE ANY SUPPORT, ASSISTANCE, INSTALLATION,
TRAINING OR OTHER SERVICES.  INTEL DOES NOT PROVIDE ANY UPDATES, ENHANCEMENTS
OR EXTENSIONS.  INTEL SPECIFICALLY DISCLAIMS ANY WARRANTY OF MERCHANTABILITY,
NONINFRINGEMENT, FITNESS FOR ANY PARTICULAR PURPOSE, OR ANY OTHER WARRANTY.
Intel disclaims all liability, PARTICULAR PURPOSE, OR ANY OTHER WARRANTY.
including liability for infringement of any proprietary rights, relating to
use of the code. No license, express or implied, by estoppel or otherwise,
to any intellectual property rights is granted herein.

File(Name) : Main.cpp

Abstract:	Program entry point and related functions.

Notes:

\**  *******************************************************************************************  **/


#include <tchar.h>

#include "TreeOrchard.h"

void LoadIniFile(LPWSTR lpCmdLine)
{
    //------Config File Initialization--------------------
        // grab vars from config file
    int iReadVal = 0;
	float fReadVal = 0.0f;
    float farray[4] = {1.0f, 1.0f, 1.0f ,1.0f};
    LPTSTR pConfigFileLoc;
    CINIFileReader CConfigReader;
    int iSizeBuf = 1024;

    pConfigFileLoc                  = new TCHAR[iSizeBuf];
    pFxFileLoc                      = new TCHAR[iSizeBuf];
    pSgFileLoc                      = new TCHAR[iSizeBuf];
    pTex1FileLoc                    = new TCHAR[iSizeBuf];
    pTex2FileLoc                    = new TCHAR[iSizeBuf];
    pCsvFileLoc                     = new TCHAR[iSizeBuf];
    pTechTemp                       = new TCHAR[iSizeBuf];
    pDrawNoSubdivideTechniqueName   = new CHAR[iSizeBuf];
    pDrawTechniqueName              = new CHAR[iSizeBuf];
    pCanopyTechniqueName            = new CHAR[iSizeBuf];
    pStreamOutTechniqueName         = new CHAR[iSizeBuf];

    
    _stprintf_s(pConfigFileLoc, iSizeBuf, lpCmdLine); 
    CConfigReader.SetIniFile( pConfigFileLoc );

    //try reading locally if fail then try to read from the hardcoded loc...if both fail then default values are used
    iReadVal = CConfigReader.ReadInt(L"INITVALS", L"TOTALBUFFERS");

    if (iReadVal == CConfigReader.GetIntErrorValue())
    {
        wsprintf(pConfigFileLoc, L".\TreeOrchard.ini"); 
    }

    iReadVal = CConfigReader.ReadString(L"INITVALS", L"FXFILENAME", pFxFileLoc, iSizeBuf);

    if(( iReadVal == CConfigReader.GetStringErrorValue()) || (wcscmp(pFxFileLoc, DEFAULT_STRING) == 0))
    {//if iReadInt test fails your buffer is not big enough, if the DEFAULT_STRING is in the buffer then your location or key values are wrong
        wsprintf(pFxFileLoc, L"Source\\TreeOrchard.fx");        
    }

    iReadVal = CConfigReader.ReadString(L"INITVALS", L"TREEGRAMMAR", pSgFileLoc, iSizeBuf);

    if(( iReadVal == CConfigReader.GetStringErrorValue()) || (wcscmp(pSgFileLoc, DEFAULT_STRING) == 0))
    {//if iReadInt test fails your buffer is not big enough, if the DEFAULT_STRING is in the buffer then your location or key values are wrong
        wsprintf(pSgFileLoc, L"Source\\STG1.tdf");        
    }

    iReadVal = CConfigReader.ReadString(L"INITVALS", L"CSVLOC", pCsvFileLoc, iSizeBuf);

    if(( iReadVal == CConfigReader.GetStringErrorValue()) || (wcscmp(pCsvFileLoc, DEFAULT_STRING) == 0))
    {//if iReadInt test fails default to csv file 
        wsprintf(pCsvFileLoc, L".\\WickedWeasel.csv");        
    }

    iReadVal = CConfigReader.ReadString(L"INITVALS", L"TEXTURE1", pTex1FileLoc, iSizeBuf);

    if(( iReadVal == CConfigReader.GetStringErrorValue()) || (wcscmp(pTex1FileLoc, DEFAULT_STRING) == 0))
    {//if iReadInt test fails default to csv file 
        wsprintf(pTex1FileLoc, L"..\\TreeOrchard\\media\\testQuad.dds");        
    }

    iReadVal = CConfigReader.ReadString(L"INITVALS", L"TEXTURE2", pTex2FileLoc, iSizeBuf);

    if(( iReadVal == CConfigReader.GetStringErrorValue()) || (wcscmp(pTex2FileLoc, DEFAULT_STRING) == 0))
    {//if iReadInt test fails default to csv file 
        wsprintf(pTex2FileLoc, L"..\\TreeOrchard\\media\\testQuad.dds");        
    }

    iReadVal = CConfigReader.ReadString(L"INITVALS", L"DRAWNOSUBDIVIDETECHNIQUENAME", pTechTemp, iSizeBuf);
    wcstombs(pDrawNoSubdivideTechniqueName,pTechTemp,iSizeBuf);
    if( iReadVal == CConfigReader.GetStringErrorValue())
    {
        sprintf(pDrawNoSubdivideTechniqueName, "NoTree");        
    }

    iReadVal = CConfigReader.ReadString(L"INITVALS", L"DRAWTECHNIQUENAME", pTechTemp, iSizeBuf);
    wcstombs(pDrawTechniqueName,pTechTemp,iSizeBuf);
    if( iReadVal == CConfigReader.GetStringErrorValue())
    {
        sprintf(pDrawTechniqueName, "RenderGeometryIntances");        
    }

    iReadVal = CConfigReader.ReadString(L"INITVALS", L"CANOPYTECHNIQUENAME", pTechTemp, iSizeBuf);
    wcstombs(pCanopyTechniqueName,pTechTemp,iSizeBuf);
    if( iReadVal == CConfigReader.GetStringErrorValue())
    {
        sprintf(pCanopyTechniqueName, "RenderGeometryIntances");        
    }

    iReadVal = CConfigReader.ReadString(L"INITVALS", L"STREAMOUTTECHNIQUENAME", pTechTemp, iSizeBuf);
    wcstombs(pStreamOutTechniqueName,pTechTemp,iSizeBuf);

    if( iReadVal == CConfigReader.GetStringErrorValue())
    {
        sprintf(pStreamOutTechniqueName, "StreamOut");        
    }

    iReadVal = CConfigReader.ReadInt(L"INITVALS", L"TOTALBUFFERS");

    if (iReadVal != CConfigReader.GetIntErrorValue())
    {
        uiNumBuffers = iReadVal;
    }

    iReadVal = CConfigReader.ReadInt(L"INITVALS", L"APPLESEED");

    if (iReadVal != CConfigReader.GetIntErrorValue())
    {
        JohnnyAppleseed = iReadVal;
    }

    fReadVal = CConfigReader.ReadBrace4Float(L"INITVALS", L"BACKCOLOR", g_pfaBackColor);

    fReadVal = CConfigReader.ReadBrace4Float(L"INITVALS", L"SPEC1", farray);
        g_Spec.x = farray[0];
        g_Spec.y = farray[1];
        g_Spec.z = farray[2];

    fReadVal = CConfigReader.ReadBrace4Float(L"INITVALS", L"EL1", farray);

        g_El1.x = farray[0];
        g_El1.y = farray[1];
        g_El1.z = farray[2];

    //fReadVal = CConfigReader.ReadFloat(L"INITVALS", L"SPECR");

 
    //fReadVal = CConfigReader.ReadFloat(L"INITVALS", L"SPECG");


    //fReadVal = CConfigReader.ReadFloat(L"INITVALS", L"SPECB");


    //fReadVal = CConfigReader.ReadFloat(L"INITVALS", L"ELR");

    //    g_El1.x = fReadVal;
 
    //fReadVal = CConfigReader.ReadFloat(L"INITVALS", L"ELG");

    //    g_El1.y = fReadVal;

    //fReadVal = CConfigReader.ReadFloat(L"INITVALS", L"ELB");

    //    g_El1.z = fReadVal;

    fReadVal = CConfigReader.ReadFloat(L"INITVALS", L"POW");

        g_Pow = fReadVal;

    iReadVal = CConfigReader.ReadInt(L"INITVALS", L"POINTSPERBUFFER");

    if (iReadVal != CConfigReader.GetIntErrorValue())
    {
        uiNumPointsBuffer = iReadVal;
        uiMaxNumSOBufferSize = uiNumPointsBuffer * 3;
    }

    iReadVal = CConfigReader.ReadInt(L"INITVALS", L"RESOLUTIONWIDTH");

    if (iReadVal != CConfigReader.GetIntErrorValue())
    {
        gWindowWidth = iReadVal;
    }

    iReadVal = CConfigReader.ReadInt(L"INITVALS", L"RESOLUTIONHEIGHT");

    if (iReadVal != CConfigReader.GetIntErrorValue())
    {
        gWindowHeight = iReadVal;
    }

    iReadVal = CConfigReader.ReadInt(L"INITVALS", L"TOTALSTREAMOUTBUFFERS");

    if (iReadVal != CConfigReader.GetIntErrorValue())
    {
        uiNumSOBuffers = iReadVal;
    }

    iReadVal = CConfigReader.ReadInt(L"INITVALS", L"WINDOWEDMODE");

    if (iReadVal != CConfigReader.GetIntErrorValue())
    {
        g_WindowedMode = (bool) iReadVal;
    }

    

    iReadVal = CConfigReader.ReadInt(L"INITVALS", L"MAXDRAWCALLS");

    if (iReadVal != CConfigReader.GetIntErrorValue())
    {
        uiMaxDrawCalls = iReadVal;
    }

    iReadVal = CConfigReader.ReadInt(L"INITVALS", L"DRAWCALLSTEP");

    if (iReadVal != CConfigReader.GetIntErrorValue())
    {
        uiDrawCallStep = iReadVal;
    }

    iReadVal = CConfigReader.ReadInt(L"INITVALS", L"DRAWCALLSTART");

    if (iReadVal != CConfigReader.GetIntErrorValue())
    {
        uiDrawCallStart = iReadVal;
    }

    iReadVal = CConfigReader.ReadInt(L"INITVALS", L"LOOPORCULDISACK");

    if (iReadVal != CConfigReader.GetIntErrorValue())
    {
        uiStreamOut = iReadVal;
    }

    iReadVal = CConfigReader.ReadInt(L"INITVALS", L"LOCKCAMERA");

    if (iReadVal != CConfigReader.GetIntErrorValue())
    {
        uiLockCamera = iReadVal;
    }



    //no longer need string allocations
    if (pConfigFileLoc != NULL)
    {
        delete[] pConfigFileLoc;
        pConfigFileLoc = NULL;
    }


    if (pTechTemp != NULL)
    {
        delete[] pTechTemp;
        pTechTemp = NULL;
    }


    return;
}

/***************************************************************************************************\
Function Name: wWinMain

Description:   

Input: 
    HINSTANCE hInstance:  

	HINSTANCE hPrevInstance:  

	LPWSTR lpCmdLine:  

	int nCmdShow:  

Return: 

\***************************************************************************************************/

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    //_crtBreakAlloc = 201;
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    LoadIniFile(lpCmdLine);
//---------Test Area----------------
    D3DXMATRIX dm1(1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,2.0f,2.0f,3.0f,3.0f,3.0f,3.0f,4.0f,4.0f,4.0f,4.0f);
    //D3DXMATRIX dm2(0.5f,0.5f,0.5f,0.5f,0.25f,0.25f,0.25f,0.25f,0.125f,0.125f,0.125f,0.125f,0.0625f,0.0625f,0.0625f,0.0625f);
    D3DXMATRIX dm2(1.0f,1.0f,1.0f,1.0f,
                   1.0f,1.0f,1.0f,1.0f,
                   1.0f,1.0f,1.0f,1.0f,
                   1.0f,1.0f,1.0f,1.0f);
   dm2 *= dm1;


//--------Radio Active -------------
//------End Config File Initialization--------------------
    if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
        return 0;

    if( FAILED( InitDevice() ) )
    {
        CleanupDevice();
        return 0;
    }

	//  Create Render() thread here:
	DWORD	dwRenderThreadID = 0;
	g_hRenderThreadHandle	=	
		CreateThread( NULL, 0,(LPTHREAD_START_ROUTINE)Render, NULL, 0, &dwRenderThreadID );

    // Main message loop
    MSG msg = {0};

    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }

    }

	// Window received a quit message - terminate the render thread:
	LONG	volatile lvMyDestVal = 0;
	LONG	lvOldValue = g_lvContinueRunningQ;

	while( lvOldValue != InterlockedCompareExchange(  &g_lvContinueRunningQ, lvMyDestVal, (LONG)1 ) )
		;
	
	//  Let the Render() close on its own so that it can cleanup and gracefully exit
	//  then close the thread
	while( g_sbRenderThreadRunning )
		;
	CloseHandle( g_hRenderThreadHandle );
	
	//Clean up and exit
    CleanupDevice();

    return ( int )msg.wParam;
}	// wWinMain( ... )


/***************************************************************************************************\
Function Name: InitWindow

Description:   

Input: 
    HINSTANCE hInstance:  

	int nCmdShow:  

Return: 

\***************************************************************************************************/

HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )IDI_MYTREEORCHARD );
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"ProceduralContent";
    wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )IDI_MYTREEORCHARD );
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // Create window
    g_hInst = hInstance;
    RECT rc = { 0, 0, gWindowWidth, gWindowHeight };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    g_hWnd = CreateWindow( L"ProceduralContent", L"TreeOrchard", WS_OVERLAPPEDWINDOW,
                           1, 1, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
                           NULL );
    if( !g_hWnd )
        return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );

    return S_OK;
}


LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	POINT mousePos;
	int dx = 0;
	int dy = 0;
	switch(message)
	{
	case WM_LBUTTONDOWN:
		if( wParam & MK_LBUTTON && !uiLockCamera )
		{
			SetCapture(hWnd);

			g_OldMousePos.x = LOWORD(lParam);
			g_OldMousePos.y = HIWORD(lParam);
		}
		return 0;

	case WM_LBUTTONUP:
		ReleaseCapture();
		return 0;

	case WM_MOUSEMOVE:
		if( wParam & MK_LBUTTON && !uiLockCamera )
		{
			mousePos.x = (int)LOWORD(lParam); 
			mousePos.y = (int)HIWORD(lParam); 

			dx = mousePos.x - g_OldMousePos.x;
			dy = mousePos.y - g_OldMousePos.y;

			camera::pitch( dy * 0.0087266f );
			camera::rotateY( dx * 0.0087266f );
			
			g_OldMousePos = mousePos;
		}
		return 0;
    //case WM_PAINT:
    //    hdc = BeginPaint( hWnd, &ps );
    //    EndPaint( hWnd, &ps );
    //    break;

	case WM_MOUSEWHEEL:
        if(wParam && !uiLockCamera ){
            short direction = (short)HIWORD(wParam);
            if(direction < 0){
                camera::walk(-20.0f*0.020f);
            }
            if(direction > 0){
                camera::walk(+20.0f*0.020f);
            }
        }
        return 0;
        
    case WM_DESTROY:
        PostQuitMessage( 0 );
        break;

    default:
        return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}

//Currently just grow 1 tree
void GrowTrees()
{
    observer *Overseer = observer::Instance();
    if (Overseer->DXRS) {delete Overseer->DXRS;}
    Overseer->DXRS = new RenderStructure();
    Overseer->DXRS->ptrIBData = 0;
    Overseer->DXRS->vpnt = 0;
    Overseer->DXRS->CurrentIndex	= 0;
    Overseer->DXRS->CurrentVIndex	= 0;
    Overseer->DXRS->BranchCount	= 0;
    Overseer->DXRS->ReverseWindingOrder = false;

    std::wstring fl(pSgFileLoc);
    std::string temp(fl.length(),' ');
    std::copy(fl.begin(),fl.end(),temp.begin());
    g_tree = new Tree(temp,V3(0,0,0),JohnnyAppleseed);

    g_tree->growTree();

    Overseer->DXRS->CurrentIndex	= 0;
    Overseer->DXRS->CurrentVIndex	= 0;
    Overseer->DXRS->BranchCount	= 0;    



}
void CreateSceneGraph()
{
	g_SceneGraph = new SG::SceneGraph(2);
	g_pIbd = new D3D10_BUFFER_DESC[1];
	//g_pIbd[0] = new D3D10_BUFFER_DESC();
	g_pVbd = new D3D10_BUFFER_DESC[1];
	//g_pVbd[0] = new D3D10_BUFFER_DESC();

	GrowTrees();

	unsigned int vertexCount = g_tree->m_EndBranchVertexBuffer;
	vertexCount += g_tree->m_EndCanopyVertexBuffer;
    unsigned int indexCount  = g_tree->m_EndBranchIndexBuffer;
	indexCount += g_tree->m_EndCanopyIndexBuffer;

	
	g_pVbd[0].Usage = D3D10_USAGE_DEFAULT;
	g_pVbd[0].ByteWidth = sizeof( CUSTOMVERTEX ) * vertexCount;
	g_pVbd[0].BindFlags = D3D10_BIND_VERTEX_BUFFER;
	g_pVbd[0].CPUAccessFlags = 0;
	g_pVbd[0].MiscFlags = 0;
			 
	g_pIbd[0].Usage = D3D10_USAGE_IMMUTABLE;
	g_pIbd[0].ByteWidth = sizeof(DWORD) * indexCount;
	g_pIbd[0].BindFlags = D3D10_BIND_INDEX_BUFFER;
	g_pIbd[0].CPUAccessFlags = 0;
	g_pIbd[0].MiscFlags = 0;
			 
	//g_SceneGraph->m_sceneGraph[0].siCBuffers = new SG::BufferInfo();
	g_SceneGraph->m_sceneGraph[0].siCBuffers->ibd = &(g_pIbd[0]);
	g_SceneGraph->m_sceneGraph[0].siCBuffers->vdb = &(g_pVbd[0]);
	g_SceneGraph->m_sceneGraph[0].siCBuffers->IdxBffStart = 0;
	g_SceneGraph->m_sceneGraph[0].siCBuffers->IndexCount = g_tree->m_EndBranchIndexBuffer;
	g_SceneGraph->m_sceneGraph[0].siCBuffers->VtxBffStart = 0;
	g_SceneGraph->m_sceneGraph[0].siCBuffers->VertexCount = g_tree->m_EndBranchVertexBuffer;
	g_SceneGraph->m_sceneGraph[1].siCBuffers->ibd = &(g_pIbd[0]);
	g_SceneGraph->m_sceneGraph[1].siCBuffers->vdb = &(g_pVbd[0]);
	g_SceneGraph->m_sceneGraph[1].siCBuffers->IdxBffStart = g_SceneGraph->m_sceneGraph[0].siCBuffers->IndexCount;
	g_SceneGraph->m_sceneGraph[1].siCBuffers->IndexCount = g_tree->m_EndCanopyIndexBuffer;
	g_SceneGraph->m_sceneGraph[1].siCBuffers->VtxBffStart = g_SceneGraph->m_sceneGraph[0].siCBuffers->VertexCount;
	g_SceneGraph->m_sceneGraph[1].siCBuffers->VertexCount = g_tree->m_EndCanopyVertexBuffer;



}


HRESULT InitDevice()
{
    HRESULT hr = S_OK;;

    RECT rc;
    GetClientRect( g_hWnd, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    hr = CreateSwapChain();
    
 //------ Test Area ------------   
    float F = 2.2f;
    float *f = &F;
    D3DXVECTOR3 DV3(4.4f,8.8f,11.11f);
    f = DV3;
    if(1);
    f;
//------- Radio Active -------------------
	// Create a render target view
    if( SUCCEEDED(hr) )
	{
        hr = CreateRenderView();
    }
 

	// Create the depth/stencil buffer and view.

    if( SUCCEEDED(hr) )
	{
        hr = CreateDepthStencilView();
    }

	g_pd3dDevice->OMSetRenderTargets( 1, 
                                      &g_pRenderTargetView, 
                                      g_pDepthStencilView );


	
	// create Scene Graph
	CreateSceneGraph();
	// Setup the viewport
    if( SUCCEEDED(hr) )
	{
        hr = SetUpViewPort();
    }
    
    SetViewerTransforms();
    
    //Load and Create the Effect from FX file
    if( SUCCEEDED(hr) )
	{
        hr = CreateEffect();
    }

    // Define the input layout
    if( SUCCEEDED(hr) )
	{
        hr = InitLayouts();
    }

    //g_Camera.Position = D3DXVECTOR3(0.0f, 1.8f, -10.0f);


    // Load Texture
    //g_pTexArrayRV = CreateTextureFromFile(pTex1FileLoc);
	g_SceneGraph->m_sceneGraph[0].pRV = CreateTextureFromFile(pTex1FileLoc);
    //g_pTexCanopyRV = CreateTextureFromFile(pTex2FileLoc);
	g_SceneGraph->m_sceneGraph[1].pRV = CreateTextureFromFile(pTex2FileLoc);
    //g_pTexArrayRV = CreateTextureFromFile(L"..\\Tree\\media\\testQuad.dds");

	// Obtain the technique
    //bind Shader Variables with Application Variables
    BindAppShaderVars();


	// Create vertex buffers
    if(uiStreamOut == 1)
    {
        g_pStreamOutVBArray = new ID3D10Buffer*[uiNumBuffers];
    }

        InitTreeVertexBuffer(&g_pVertexBuffer, &g_pIndexBuffer);
        //InitCanopyVertexBuffer(&g_pVertexBuffer, &g_pIndexBuffer);


    return S_OK;
}

void lockLook(){
    sbLockLook = true;
}

void unlockLook(){
    sbLockLook = false;
}


// Setup Buffers

// UI input
void updateUI(){
        
	if (!uiLockCamera) 
    {
	    if(GetAsyncKeyState('A') & 0x8000)	camera::strafe(-200.0f*0.0020f);
	    if(GetAsyncKeyState('D') & 0x8000)	camera::strafe(+200.0f*0.0020f);
	    if(GetAsyncKeyState('W') & 0x8000)	camera::walk(+200.0f*0.0020f);
	    if(GetAsyncKeyState('S') & 0x8000)	camera::walk(-200.0f*0.0020f);
	    if(GetAsyncKeyState('F') & 0x8000)	camera::rise(+200.0f*0.0020f);
	    if(GetAsyncKeyState('C') & 0x8000)	camera::rise(-200.0f*0.0020f);
	    if(GetAsyncKeyState('L') & 0x8000)	lockLook();
	    if(GetAsyncKeyState('U') & 0x8000)	unlockLook();
	    if(GetAsyncKeyState(VK_LEFT) & 0x8000)	g_Light1.rotateZ(-0.01f);
	    if(GetAsyncKeyState(VK_RIGHT) & 0x8000)	g_Light1.rotateZ(0.01f);
	    if(GetAsyncKeyState(VK_UP) & 0x8000)	g_Light1.rotateY(-0.01f);
	    if(GetAsyncKeyState(VK_DOWN) & 0x8000)	g_Light1.rotateY(0.01f);
    }

}
// Set All Cameras
void updateCameras(){
        if(sbLockLook){
            g_Camera.Look = g_LookAt - g_Camera.Position;
        }
        D3DXVec3Normalize(&g_Camera.Look, &g_Camera.Look);
        camera::rebuildView();
		//Set Eye Position
		g_EyePos = D3DXVECTOR4(g_Camera.Position.x,g_Camera.Position.y,g_Camera.Position.z,1.0f);

}
	
void setLights(){
		//Set Light Directions
		g_LightDir1 = D3DXVECTOR4(g_Light1.Direction.x,g_Light1.Direction.y,g_Light1.Direction.z,1.0f);
		g_LightDir2 = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
		g_LightDir3 = D3DXVECTOR4(-1.0f, 0.0f, 0.0f, 1.0f);
		//g_LightDir = D3DXVECTOR4(0,0.5f,-0.1f,1.0f);
        D3DXVec4Normalize(&g_LightDir1, &g_LightDir1);
        D3DXVec4Normalize(&g_LightDir2, &g_LightDir2);
        D3DXVec4Normalize(&g_LightDir3, &g_LightDir3);

		//setLightColor
		g_El2 = D3DXVECTOR3(0.1f,0.1f,0.1f);
		g_El3 = D3DXVECTOR3(0.1f,0.1f,0.1f);

}

void setConstants(int sceneitem){
        
    g_pmatViewProjectionVariable->SetMatrix( g_Camera.View * g_Camera.Proj );
	g_pEyePosVar->SetFloatVector((float*)&g_EyePos);
    g_pLightDir1Var->SetFloatVector((float*)&g_LightDir1);
    g_pLightDir2Var->SetFloatVector((float*)&g_LightDir2);
    g_pLightDir3Var->SetFloatVector((float*)&g_LightDir3);
	g_pSpecVariable->SetFloatVector(g_Spec);
    g_pTexArrayVariable->SetResource(g_SceneGraph->m_sceneGraph[sceneitem].pRV);
    //g_pTexArrayVariable->SetResource(g_pTexCanopyRV);
    g_pPowVariable->SetFloat(g_Pow);
	g_pEl1Variable->SetFloatVector(g_El1);
	g_pEl2Variable->SetFloatVector(g_El2);
	g_pEl3Variable->SetFloatVector(g_El3);

}
// clear and set Render Buffers
	
// for each item in scene graph
	
//     set constants
	
//     set fx details;  effects, lighting, materials.
	
//     set render targets and  states
	
//     draw object

/***************************************************************************************************\
Function Name: Render

Description:   

Input: 
    NONE

Return: 

\***************************************************************************************************/

LONG	WINAPI	Render()
{

	g_sbRenderThreadRunning = TRUE;

    // Setup Buffers
	// UI input
	// Set All Cameras
	// clear and set Render Buffers
	// for each item in scene graph
	//     set constants
	//     set fx details;  effects, lighting, materials.
	//     set render targets and  states
	//     draw object


	UINT *stride = new UINT[uiNumBuffers];
    UINT *offset = new UINT[uiNumBuffers]; 
    UINT nb;
    for(nb =0;nb < uiNumBuffers;nb++){
        stride[nb] = sizeof(CUSTOMVERTEX);
        offset[nb] = 0;
    }
	 
    nb = uiNumBuffers;
    ID3D10Buffer** bufferArray;
    bufferArray = new ID3D10Buffer*[uiNumBuffers];
    for(int bc =0;bc < uiNumBuffers;bc++)
    {
        bufferArray[bc] = 0;
    }
    int iFrameCounter = 0;
    double dTimeElapsed = 0;


	while( g_lvContinueRunningQ > 0 )
	{
		updateUI();
		updateCameras();

		//clear Render Views

        //float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //red,green,blue,alpha signature Color
        //float ClearColor[4] = { 0.83529f, 0.83529f, 0.88235f, 1.0f }; //red,green,blue,alpha 
    	g_pd3dDevice->ClearRenderTargetView( g_pRenderTargetView, g_pfaBackColor );
        g_pd3dDevice->ClearDepthStencilView( g_pDepthStencilView, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);

        // Set primitive topology
        g_pd3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		//set Output Manager States
	    g_pd3dDevice->OMSetDepthStencilState(0, 0);
	    float blendFactor[] = {0.0f, 0.0f, 0.0f, 0.0f};
	    g_pd3dDevice->OMSetBlendState(0, blendFactor, 0xffffffff);


		
		setLights();
		
        
        
        switch (uiStreamOut)
        {
        case PathType::CULDISACK:
            {
		        g_pd3dDevice->IASetInputLayout( g_pVertexLayout );

                g_pd3dDevice->IASetVertexBuffers(0,uiNumBuffers,g_pVertexBufferArray,stride,offset);

                //Render to RenderTarget
    		    
		        D3D10_TECHNIQUE_DESC techDesc;
		        g_pDrawTechnique->GetDesc( &techDesc );
		        for( UINT p = 0; p < techDesc.Passes; ++p )
		        {
			        g_pDrawTechnique->GetPassByIndex( p )->Apply( 0 );
			        g_pd3dDevice->Draw( uiNumPointsBuffer, 0 );
		        }
            }
            break;
        case PathType::LOOP:
            {
             //------ stream manipulation begins-----------
		        g_pd3dDevice->IASetInputLayout( g_pVertexLayout );


                g_pd3dDevice->IASetVertexBuffers(0,uiNumBuffers,g_pVertexBufferArray,stride,offset);
                g_pd3dDevice->SOSetTargets(nb,g_pStreamOutVBArray,offset);

		        // Render to StreamOut
		        D3D10_TECHNIQUE_DESC techDesc;
		        g_pStreamOutTechnique->GetDesc( &techDesc );
		        for( UINT p = 0; p < techDesc.Passes; ++p )
		        {
			        g_pStreamOutTechnique->GetPassByIndex( p )->Apply( 0 );
			        g_pd3dDevice->Draw( uiNumPointsBuffer, 0 );
		        }
                
	            // done streaming-out--unbind the vertex buffer
	            g_pd3dDevice->SOSetTargets(uiNumBuffers, bufferArray, offset);

                g_pd3dDevice->IASetVertexBuffers(0,uiNumBuffers,g_pStreamOutVBArray,stride,offset);

                //Render to RenderTarget
		        g_pDrawTechnique->GetDesc( &techDesc );
		        for( UINT p = 0; p < techDesc.Passes; ++p )
		        {
			        g_pDrawTechnique->GetPassByIndex( p )->Apply( 0 );
			        g_pd3dDevice->DrawAuto( );
		        }
           }
            break;
        case PathType::LINE:
            {
				for(UINT sgi=0;sgi<2;sgi++){
					setConstants(sgi);
					g_pd3dDevice->IASetInputLayout( g_SceneGraph->m_sceneGraph[sgi].pILO );

					//g_pd3dDevice->IASetVertexBuffers(0,uiNumBuffers,&g_pVertexBuffer,stride,offset);
					//g_pd3dDevice->IASetIndexBuffer(g_pIndexBuffer,DXGI_FORMAT_R32_UINT,0);
					g_pd3dDevice->IASetVertexBuffers(0,uiNumBuffers,&g_SceneGraph->m_sceneGraph[sgi].siCBuffers->pVtxBff,stride,offset);
					g_pd3dDevice->IASetIndexBuffer(g_SceneGraph->m_sceneGraph[sgi].siCBuffers->pIdxBff,DXGI_FORMAT_R32_UINT,0);
					//Render to RenderTarget
					D3D10_BUFFER_DESC ibd;
    				g_SceneGraph->m_sceneGraph[sgi].siCBuffers->pIdxBff->GetDesc( &ibd);

					D3D10_TECHNIQUE_DESC techDesc;
					g_SceneGraph->m_sceneGraph[sgi].pET->GetDesc(&techDesc );
					//g_pTreeTechnique->GetDesc( &techDesc );
					for( UINT p = 0; p < techDesc.Passes; ++p )
					{
						g_SceneGraph->m_sceneGraph[sgi].pET->GetPassByIndex( p )->Apply( 0 );
						//g_pTreeTechnique->GetPassByIndex( p )->Apply( 0 );
						g_pd3dDevice->DrawIndexed( g_SceneGraph->m_sceneGraph[sgi].siCBuffers->IndexCount,
													g_SceneGraph->m_sceneGraph[sgi].siCBuffers->IdxBffStart,g_SceneGraph->m_sceneGraph[sgi].siCBuffers->VtxBffStart);
					}
				}
            }
            break;

        }


	    //RECT R = {5, 5, 0, 0};
	    g_pd3dDevice->RSSetState(0);

    	g_pSwapChain->Present( 0, 0 );
        iFrameCounter++;
        //dTimeElapsed = MyHPCounter.GetLastTimeInterval();

	}   //	while( g_lvContinueRunningQ > 0 )

    delete []stride;
    stride = NULL;
    delete []offset;
    offset = NULL;
    delete []bufferArray;
    bufferArray = NULL;
    
    char* pOutputLoc = new char[1024];
    wcstombs(pOutputLoc, pCsvFileLoc, 1024);

    delete []pOutputLoc;
    pOutputLoc = NULL;
	g_sbRenderThreadRunning = FALSE;

	return	0;

}

ID3D10ShaderResourceView* CreateTextureFromFile(std::wstring TexFileName)
{
    HRESULT hr = S_OK;

    ID3D10Texture2D* pStagingTexture;

	D3DX10_IMAGE_LOAD_INFO loadInfo;

    loadInfo.Width  = D3DX10_FROM_FILE;
    loadInfo.Height = D3DX10_FROM_FILE;
    loadInfo.Depth  = D3DX10_FROM_FILE;
    loadInfo.FirstMipLevel = 0;
    loadInfo.MipLevels = D3DX10_FROM_FILE;
    loadInfo.Usage = D3D10_USAGE_STAGING;
    loadInfo.BindFlags = 0;
    loadInfo.CpuAccessFlags = D3D10_CPU_ACCESS_WRITE | D3D10_CPU_ACCESS_READ;
    loadInfo.MiscFlags = 0;
    loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    loadInfo.Filter = D3DX10_FILTER_NONE;
    loadInfo.MipFilter = D3DX10_FILTER_NONE;
	loadInfo.pSrcInfo  = 0;

    int a =0;
    hr = D3DX10CreateTextureFromFile(g_pd3dDevice, TexFileName.c_str(), 
		&loadInfo, 0, (ID3D10Resource**)&pStagingTexture, 0);
    if(hr == D3D10_ERROR_FILE_NOT_FOUND){
        a = 1;
    }
    if(hr == D3D10_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS){
        a = 2;
    }
    if(hr == D3DERR_INVALIDCALL){
        a = 3;
    }
    if(hr == D3DERR_WASSTILLDRAWING){
        a = 3;
    }
    if(hr == E_FAIL){

		//if( compilationErrors )
		//{
		//	MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
		//	compilationErrors->Release();
		//}

    }
	//
	// Create the texture array.  Each element in the texture 
	// array has the same format/dimensions.
	//

	D3D10_TEXTURE2D_DESC texElementDesc;
	//g_gTex4RV 
    pStagingTexture->GetDesc(&texElementDesc);

	D3D10_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width              = texElementDesc.Width;
	texArrayDesc.Height             = texElementDesc.Height;
	texArrayDesc.MipLevels          = texElementDesc.MipLevels;
	texArrayDesc.ArraySize          = 1;
	texArrayDesc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
	texArrayDesc.SampleDesc.Count   = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage              = D3D10_USAGE_DEFAULT;
	texArrayDesc.BindFlags          = D3D10_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags     = 0;
	texArrayDesc.MiscFlags          = 0;

	ID3D10Texture2D* texArray = 0;
	g_pd3dDevice->CreateTexture2D( &texArrayDesc, 0, &texArray);

	//g_gTex4RV
	// Copy individual texture elements into texture array.
	//

	// for each mipmap level...
	for(UINT j = 0; j < texElementDesc.MipLevels; ++j)
	{
		D3D10_MAPPED_TEXTURE2D mappedTex2D;
		pStagingTexture->Map(j, D3D10_MAP_READ, 0, &mappedTex2D);
                
        g_pd3dDevice->UpdateSubresource(texArray, 
			D3D10CalcSubresource(j, 0, texElementDesc.MipLevels),
            0, mappedTex2D.pData, mappedTex2D.RowPitch, 0);

        pStagingTexture->Unmap(j);
	}
	

	//
	// Create a resource view to the texture array.
	//
	
	D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texArrayDesc.Format;
	//viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2DArray.MostDetailedMip = 0;
	viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
	viewDesc.Texture2DArray.FirstArraySlice = 0;
	viewDesc.Texture2DArray.ArraySize = 1;

	ID3D10ShaderResourceView* texArrayRV = 0;
	g_pd3dDevice->CreateShaderResourceView(texArray, &viewDesc, &texArrayRV);

	//
	// Cleanup--we only need the resource view.
	//

    texArray->Release();

    pStagingTexture->Release();

	return texArrayRV;
}

/***************************************************************************************************\
Function Name: InitVertexBuffer

Description:
		Create a verrtex buffer.

Input: 
    NONE

Return: 

\***************************************************************************************************/


/***************************************************************************************************\
Function Name: InitVertexBuffer

Description:
		Create a verrtex buffer.

Input: 
    NONE

Return: 

\***************************************************************************************************/

void  InitVertexBuffer( ID3D10Buffer** pVBuff, ID3D10Buffer** pIBuff )
{

    observer *Overseer = observer::Instance();
    if (Overseer->DXRS) {delete Overseer->DXRS;}
    Overseer->DXRS = new RenderStructure();
    Overseer->DXRS->ptrIBData = 0;
    Overseer->DXRS->vpnt = 0;
    Overseer->DXRS->CurrentIndex	= 0;
    Overseer->DXRS->CurrentVIndex	= 0;
    Overseer->DXRS->BranchCount	= 0;
    Overseer->DXRS->ReverseWindingOrder = false;

    std::wstring fl(pSgFileLoc);
    std::string temp(fl.length(),' ');
    std::copy(fl.begin(),fl.end(),temp.begin());
    g_tree = new Tree(temp,V3(0,0,0),JohnnyAppleseed);

    g_tree->growTree();

    //g_tree->countSpheres(g_tree->m_nodeTree);
    unsigned int vertexCount = g_tree->m_EndBranchVertexBuffer;
    unsigned int indexCount  = g_tree->m_EndBranchIndexBuffer;
    Overseer->DXRS->CurrentIndex	= 0;
    Overseer->DXRS->CurrentVIndex	= 0;
    Overseer->DXRS->BranchCount	= 0;    
	
	HRESULT hr = S_OK;
	D3D10_BUFFER_DESC vbd;
	vbd.Usage = D3D10_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof( CUSTOMVERTEX ) * vertexCount;
	vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;

    unsigned int uiVBSize = vertexCount * sizeof( CUSTOMVERTEX );
	CUSTOMVERTEX* prgVertices = new CUSTOMVERTEX[vertexCount];
	ZeroMemory(prgVertices, sizeof(CUSTOMVERTEX)*vertexCount);
	if( !prgVertices )
	{
		hr	=	E_FAIL;
	}
	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(DWORD) * indexCount;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;

	DWORD* pIndices = new DWORD[indexCount];
	ZeroMemory(pIndices, sizeof(DWORD)*indexCount);
	if( !pIndices )
	{
		hr	=	E_FAIL;
	}

    
    
	if( SUCCEEDED( hr ) )
	{
		
    Overseer->DXRS->ptrIBData = reinterpret_cast<DWORD*>(pIndices);
    Overseer->DXRS->vpnt = reinterpret_cast<VertexPNT*>(prgVertices);
    g_tree->fillBranches( g_tree->m_nodeTree );

		D3D10_SUBRESOURCE_DATA vInitData;
		vInitData.pSysMem = prgVertices;
		hr = g_pd3dDevice->CreateBuffer( &vbd, &vInitData, pVBuff );

        D3D10_SUBRESOURCE_DATA iInitData;
        iInitData.pSysMem = pIndices;
        hr = g_pd3dDevice->CreateBuffer( &ibd, &iInitData, pIBuff );

        delete []prgVertices;
        delete []pIndices;
	}

}    // InitVertexBuffer
void  InitTreeVertexBuffer( ID3D10Buffer** pVBuff, ID3D10Buffer** pIBuff )
{
	HRESULT hr = S_OK;

    observer *Overseer = observer::Instance();
	unsigned int vertexCount = g_tree->m_EndBranchVertexBuffer;
	vertexCount += g_tree->m_EndCanopyVertexBuffer;
    unsigned int indexCount  = g_tree->m_EndBranchIndexBuffer;
	indexCount += g_tree->m_EndCanopyIndexBuffer;

    unsigned int uiVBSize = vertexCount * sizeof( CUSTOMVERTEX );
	CUSTOMVERTEX* prgVertices = new CUSTOMVERTEX[vertexCount];
	ZeroMemory(prgVertices, sizeof(CUSTOMVERTEX)*vertexCount);
	//if( !prgVertices )
	//{
	//	hr	=	E_FAIL;
	//}

	DWORD* pIndices = new DWORD[indexCount];
	ZeroMemory(pIndices, sizeof(DWORD)*indexCount);
	//if( !pIndices )
	//{
	//	hr	=	E_FAIL;
	//}

    
    
	//if( SUCCEEDED( hr ) )
	//{
		
    Overseer->DXRS->ptrIBData = reinterpret_cast<DWORD*>(pIndices);
    Overseer->DXRS->vpnt = reinterpret_cast<VertexPNT*>(prgVertices);
    g_tree->fillBranches( g_tree->m_nodeTree );
	g_tree->fillCanopies( g_tree->m_nodeTree );

	D3D10_SUBRESOURCE_DATA vInitData;
	vInitData.pSysMem = prgVertices;
	//hr = g_pd3dDevice->CreateBuffer( &vbd, &vInitData, pVBuff );
	//
	hr = g_pd3dDevice->CreateBuffer( g_SceneGraph->m_sceneGraph[0].siCBuffers->vdb, &vInitData, pVBuff );
	g_SceneGraph->m_sceneGraph[0].siCBuffers->pVtxBff = pVBuff[0];
	g_SceneGraph->m_sceneGraph[1].siCBuffers->pVtxBff = pVBuff[0];
    D3D10_SUBRESOURCE_DATA iInitData;
    iInitData.pSysMem = pIndices;
    //
	hr = g_pd3dDevice->CreateBuffer( g_SceneGraph->m_sceneGraph[0].siCBuffers->ibd, &iInitData, pIBuff );
	g_SceneGraph->m_sceneGraph[0].siCBuffers->pIdxBff = pIBuff[0];
	g_SceneGraph->m_sceneGraph[1].siCBuffers->pIdxBff = pIBuff[0];

    delete []prgVertices;
    delete []pIndices;

	//}

}    // InitVertexBuffer


/***************************************************************************************************\
Function Name: InitStreamOutVertexBuffer

Description:
		Create a verrtex buffer.

Input: 
    NONE

Return: 

\***************************************************************************************************/

ID3D10Buffer*  InitStreamOutVertexBuffer( unsigned int uiNumPoints )
{
	HRESULT hr = S_OK;

	D3D10_BUFFER_DESC bd;
	bd.Usage = D3D10_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( CUSTOMVERTEX ) * uiNumPoints;
    ui64MemSize = sizeof( CUSTOMVERTEX ) * uiNumPoints;
	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER | D3D10_BIND_STREAM_OUTPUT;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
    bd.BindFlags = D3D10_BIND_VERTEX_BUFFER | D3D10_BIND_STREAM_OUTPUT;

    ID3D10Buffer* pStreamOutVB=NULL;

	 hr = g_pd3dDevice->CreateBuffer(&bd, 0, &pStreamOutVB);

 
	return	pStreamOutVB;

}    // InitVertexBuffer

//  Initialize Input Layouts
//
//
HRESULT InitLayouts()
{
    HRESULT hr = S_OK;
	D3D10_PASS_DESC PassDesc;

	//
	// Position vertex
	//
 	D3D10_INPUT_ELEMENT_DESC posVertexDesc[] =
	{
		{"POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	//someFX->GetTechniqueByName("SOTech")->GetPassByIndex(0)->GetDesc(&PassDesc);
 //   HR(device->CreateInputLayout(posVertexDesc, sizeof( posVertexDesc ) / sizeof( D3D10_INPUT_ELEMENT_DESC ), PassDesc.pIAInputSignature,
	//	PassDesc.IAInputSignatureSize, &g_pSOVertexLayout));

	//
	// Position-normal-texture vertex
	//

	D3D10_INPUT_ELEMENT_DESC posNormalTexVertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

    g_pEffect->GetTechniqueByName(pDrawTechniqueName)->GetPassByIndex(0)->GetDesc(&PassDesc);
    hr = g_pd3dDevice->CreateInputLayout(posNormalTexVertexDesc, sizeof( posNormalTexVertexDesc ) / sizeof( D3D10_INPUT_ELEMENT_DESC ), PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &g_pVertexLayout);
	g_SceneGraph->m_sceneGraph[0].pILO = g_pVertexLayout;
    g_pEffect->GetTechniqueByName(pCanopyTechniqueName)->GetPassByIndex(0)->GetDesc(&PassDesc);
    hr = g_pd3dDevice->CreateInputLayout(posNormalTexVertexDesc, sizeof( posNormalTexVertexDesc ) / sizeof( D3D10_INPUT_ELEMENT_DESC ), PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &g_pCanopyVertexLayout);
	g_SceneGraph->m_sceneGraph[1].pILO = g_pCanopyVertexLayout;

	// 
	// Position-tangent-normal-texture vertex
	//
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 36, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

  //  someFX->GetTechniqueByName("SomeTech")->GetPassByIndex(0)->GetDesc(&PassDesc);
  //  HR(device->CreateInputLayout(vertexDesc, sizeof( vertexDesc ) / sizeof( D3D10_INPUT_ELEMENT_DESC ), PassDesc.pIAInputSignature,
		//PassDesc.IAInputSignatureSize, &PosTangentNormalTex));

	// 
	// Particle vertex
	//
	D3D10_INPUT_ELEMENT_DESC particleDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"SIZE",     0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"AGE",      0, DXGI_FORMAT_R32_FLOAT,       0, 32, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TYPE",     0, DXGI_FORMAT_R32_UINT,        0, 36, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

	//someFX->GetTechniqueByName("SomeTech")->GetPassByIndex(0)->GetDesc(&PassDesc);
 //   HR(device->CreateInputLayout(particleDesc, sizeof( particleDesc ) / sizeof( D3D10_INPUT_ELEMENT_DESC ), PassDesc.pIAInputSignature,
	//	PassDesc.IAInputSignatureSize, &Particle));

    // 
	// Square vertex
	//
	D3D10_INPUT_ELEMENT_DESC squareDesc[] =
	{
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D10_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
        //{"SIZE",     0, DXGI_FORMAT_R32G32_FLOAT,       0, 28, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

	//g_pEffect->GetTechniqueByName(pDrawTechniqueName)->GetPassByIndex(0)->GetDesc(&PassDesc);
 //   hr = g_pd3dDevice->CreateInputLayout(squareDesc, sizeof( squareDesc ) / sizeof( D3D10_INPUT_ELEMENT_DESC ), PassDesc.pIAInputSignature,
	//	PassDesc.IAInputSignatureSize, &g_pVertexLayout);
    return hr;
}

//
//  An effect (which is often stored in a file with a .fx file extension) declares the pipeline 
//  state set by an effect. Effect state can be roughly broken down into three categories:

//  Variables, which are usually declared at the top of an effect. 
//  Functions, which implement shader code, or are used as helper functions by other functions. 
//  A technique, which implements rendering sequences using one or more effect passes. Each pass 
//  sets one or more state groups and calls shader functions. 

HRESULT CreateEffect()
{

    HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3D10_SHADER_DEBUG;
	dwShaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;;
#endif
	ID3D10Blob* compilationErrors = 0;
    int a =0;

	//hr = D3DX10CreateEffectFromFile(	L"..\\Tree\\Source\\Tree.fx", 
	hr = D3DX10CreateEffectFromFile(	pFxFileLoc, 
                                        NULL, 
                                        NULL, 
                                        "fx_4_0", 
                                        dwShaderFlags, 
                                        0,
										g_pd3dDevice, 
                                        NULL, 
                                        NULL, 
                                        &g_pEffect, 
                                        &compilationErrors, 
                                        NULL );

	

    if(hr == D3D10_ERROR_FILE_NOT_FOUND){
        a = 1;
    }
    if(hr == E_FAIL){

		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			compilationErrors->Release();
		}

    }
	if( FAILED( hr ) )
	{
		MessageBox( NULL,
			L"Can't find the fx file - bailing out!", L"Error", MB_OK );
	}

    return hr;

}

//Create SwapChain
HRESULT CreateSwapChain ()
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect( g_hWnd, &rc );
    gWindowWidth  = rc.right - rc.left;
    gWindowHeight = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif

    D3D10_DRIVER_TYPE driverTypes[] =
    {
        D3D10_DRIVER_TYPE_HARDWARE,
        D3D10_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = sizeof( driverTypes ) / sizeof( driverTypes[0] );

    //Render Target Buffer Description
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width  = gWindowWidth  ;
    sd.BufferDesc.Height = gWindowHeight ;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = g_WindowedMode;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags =0;

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        g_driverType = driverTypes[driverTypeIndex];
        hr = D3D10CreateDeviceAndSwapChain( NULL, 
                                            g_driverType, 
                                            NULL, 
                                            createDeviceFlags,
                                            D3D10_SDK_VERSION, 
                                            &sd, 
                                            &g_pSwapChain, 
                                            &g_pd3dDevice );
        if( SUCCEEDED( hr ) )
            break;
    }

    return hr;
}

//Create Render View
HRESULT CreateRenderView ()
{
    HRESULT hr = S_OK;
    ID3D10Texture2D* pBackBuffer;

	hr = g_pSwapChain->GetBuffer( 0, 
                                  __uuidof( ID3D10Texture2D ), 
                                  ( LPVOID* )&pBackBuffer );

    
	if( SUCCEEDED( hr ) ) {
		hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, 
                                                   NULL, 
                                                   &g_pRenderTargetView );
		pBackBuffer->Release();
	}
    return hr;
}

// Create Depth Stencil View
HRESULT CreateDepthStencilView()
{
    HRESULT hr = S_OK;
	D3D10_TEXTURE2D_DESC depthStencilDesc;
	
	depthStencilDesc.Width     = gWindowWidth  ;
	depthStencilDesc.Height    = gWindowHeight ;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count   = 1; // multisampling must match
	depthStencilDesc.SampleDesc.Quality = 0; // swap chain values.
	depthStencilDesc.Usage          = D3D10_USAGE_DEFAULT;
	depthStencilDesc.BindFlags      = D3D10_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0; 
	depthStencilDesc.MiscFlags      = 0;

	if( SUCCEEDED( hr ) ) {
	
        hr = g_pd3dDevice->CreateTexture2D(&depthStencilDesc, 0, &g_pDepthStencilBuffer);
    }
	if( SUCCEEDED( hr ) ) {
       
        hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencilBuffer, 0, &g_pDepthStencilView);
    }

    return hr;
}

// Set up Light
void setupLight(light::Light & lgt) {

	lgt.initLight();

}

// Set up view port
HRESULT SetUpViewPort()
{
    HRESULT hr = S_OK;
    	// Setup the viewport
	
	D3D10_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width =  gWindowWidth  ;
	vp.Height = gWindowHeight ;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	g_pd3dDevice->RSSetViewports( 1, &vp );
    
    return hr;
}

//View Transforms
void SetViewerTransforms()
{
        camera::initCamera();
		setupLight(g_Light1);
		g_Light1.setDirection(0,0.5f,-0.1f);

        setupLight(g_Light2);
        g_Light2.setDirection(0,-0.5f,-0.1f);


        setupLight(g_Light3);
        g_Light3.setDirection(-0.5f, 0.0f, -0.1f);

		//Set up the matrices - later pass them to the shaders via constant buffer
		//D3DXMatrixIdentity( &g_matWorld );	 // add rotations to the object in Render(...)

        D3DXVECTOR3 lookat(0.0f, 0.20f, 1.0f);
        D3DXVec3Normalize(&lookat, &lookat);
        g_Camera.Look = lookat;
        //g_Camera.Position = D3DXVECTOR3(0.0f, 1.8f, -10.0f);
        g_Camera.Position = D3DXVECTOR3(0.0f, 19.0f, -142.0f);
        //g_Camera.Position = D3DXVECTOR3(0.0f, 0.0f, 6.0f);
        camera::setLens(0.25f*D3DX_PI, (float)gWindowWidth/gWindowHeight, 0.5f, 1000.0f);



}                                                                                     
void BindAppShaderVars()
{
	g_pTreeTechnique           = g_pEffect->GetTechniqueByName("TreeTech");
	g_SceneGraph->m_sceneGraph[0].pET = g_pTreeTechnique;
	g_pCanopyTechnique           = g_pEffect->GetTechniqueByName("CanopyTech");
	g_SceneGraph->m_sceneGraph[1].pET = g_pCanopyTechnique;
	//g_pDrawTechnique              = g_pEffect->GetTechniqueByName( pDrawTechniqueName );
	//g_pStreamOutTechnique         = g_pEffect->GetTechniqueByName( pStreamOutTechniqueName );
	//g_pmatWorldVariable			  = g_pEffect->GetVariableByName( "gWorld" )->AsMatrix();
	//g_pmatViewVariable			  = g_pEffect->GetVariableByName( "gView" )->AsMatrix();
	//g_pmatProjectionVariable	  = g_pEffect->GetVariableByName( "gProj" )->AsMatrix();
    g_pmatViewProjectionVariable  = g_pEffect->GetVariableByName( "gViewProj" )->AsMatrix();
    g_pTexArrayVariable           = g_pEffect->GetVariableByName( "gTex" )->AsShaderResource();
	g_pEyePosVar                  = g_pEffect->GetVariableByName( "gEyePosW")->AsVector();
    //g_pLightDirVar                = g_pEffect->GetVariableByName( "gLightDirW")->AsVector();
    g_pLightDir1Var                = g_pEffect->GetVariableByName( "gLightDir1W")->AsVector();
    g_pLightDir2Var                = g_pEffect->GetVariableByName( "gLightDir2W")->AsVector();
    g_pLightDir3Var                = g_pEffect->GetVariableByName( "gLightDir3W")->AsVector();
	g_pSpecVariable				  = g_pEffect->GetVariableByName("gSpec")->AsVector();
	g_pEl1Variable				  = g_pEffect->GetVariableByName("gEl1")->AsVector();
	g_pEl2Variable				  = g_pEffect->GetVariableByName("gEl2")->AsVector();
	g_pEl3Variable				  = g_pEffect->GetVariableByName("gEl3")->AsVector();
    g_pPowVariable                = g_pEffect->GetVariableByName( "gmPow")->AsScalar();
    //g_pFrameParam1                = g_pEffect->GetVariableByName( "gFrameParam1")->AsScalar();
    //g_pFrameParam2                = g_pEffect->GetVariableByName( "gFrameParam2")->AsScalar();
    //g_pQuadSize                   = g_pEffect->GetVariableByName( "gQuadSize")->AsScalar();
}

/***************************************************************************************************\
Function Name: CleanupDevice

Description:   

Input: 
    NONE

Return: 

\***************************************************************************************************/

void CleanupDevice()
{
    if (pFxFileLoc != NULL)
    {      
        delete[] pFxFileLoc;
        pFxFileLoc = NULL;
    }

    if (pSgFileLoc != NULL)
    {      
        delete[] pSgFileLoc;
        pSgFileLoc = NULL;
    }

    if (pTex1FileLoc != NULL)
    {      
        delete[] pTex1FileLoc;
        pTex1FileLoc = NULL;
    }

    if (pTex2FileLoc != NULL)
    {      
        delete[] pTex2FileLoc;
        pTex2FileLoc = NULL;
    }
    
    if (pCsvFileLoc != NULL)
    {      
        delete[] pCsvFileLoc;
        pCsvFileLoc = NULL;
    }

    if (pDrawNoSubdivideTechniqueName != NULL)
    {
        delete[] pDrawNoSubdivideTechniqueName;
        pDrawNoSubdivideTechniqueName = NULL;
    }

    if (pDrawTechniqueName != NULL)
    {
        delete[] pDrawTechniqueName;
        pDrawTechniqueName = NULL;
    }

    if (pCanopyTechniqueName != NULL)
    {
        delete[] pCanopyTechniqueName;
        pCanopyTechniqueName = NULL;
    }

    if (pStreamOutTechniqueName != NULL)
    {
        delete[] pStreamOutTechniqueName;
        pStreamOutTechniqueName = NULL;
    }

	if (g_SceneGraph) delete g_SceneGraph;

    if( g_pd3dDevice ) g_pd3dDevice->ClearState();
	if( g_pVertexBuffer ) g_pVertexBuffer->Release();
	if( g_pVertexLayout ) g_pVertexLayout->Release();
	if( g_pIndexBuffer ) g_pIndexBuffer->Release();
	if( g_pEffect ) g_pEffect->Release();
    if( g_pRenderTargetView ) g_pRenderTargetView->Release();
    if( g_WindowedMode == false) {
        g_pSwapChain->SetFullscreenState(FALSE,NULL);
    }
    if( g_pSwapChain ) g_pSwapChain->Release();
    if( g_pd3dDevice ) g_pd3dDevice->Release();
    if(uiStreamOut==1){
        delete []g_pStreamOutVBArray;
    }

	if(g_pIbd) delete []g_pIbd;
	if(g_pVbd) delete []g_pVbd;

    if(g_tree){
        delete g_tree;
        g_tree = NULL;
        observer::deleteInstance();
    }

}

