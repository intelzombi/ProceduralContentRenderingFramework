
#include "StdAfx.h"
#include <atlstr.h>
#include "Defines.h"
#include "INIFileReader.h"
#include "RenderWindow.h"


LPCTSTR CRenderWindow::sm_pszWndClass = _T( "LRBDXPerfTest" );


/**************************************************************************************************\
Name: 
Description: 
Input: 
Returns: 
\**************************************************************************************************/
CRenderWindow::CRenderWindow(
    HINSTANCE hInstance,
    LPCTSTR pszCfgFileName
    )
{
    m_hInstance = hInstance;
    m_pCfg = NULL;

    //
    // Get the config file.
    //
    TCHAR szCfgFileName[ MAX_PATH ];
    if ( SUCCEEDED( FindResource( szCfgFileName, _T( "DataFiles" ), pszCfgFileName ) ) )
    {
        m_pCfg = new CINIFileReader( szCfgFileName );
    }

    m_MinTime = FLT_MAX;
    m_MaxTime = FLT_MIN;
    m_AvgTime = 0.0f;
    m_FrameCount = 0.0f;
}


/**************************************************************************************************\
Name: 
Description: 
Input: 
Returns: 
\**************************************************************************************************/
CRenderWindow::~CRenderWindow(
    void
    )
{
    if ( m_pCfg != NULL )
    {
        delete m_pCfg;
    }

    if ( m_hWnd != NULL )
    {
        ::DestroyWindow( m_hWnd );
        m_hWnd = NULL;
    }

    if ( m_hClass != 0 )
    {
        ::UnregisterClass( sm_pszWndClass, m_hInstance );
        m_hClass = 0;
    }
}


/**************************************************************************************************\
Name: 
Description: 
Input: 
Returns: 
\**************************************************************************************************/
HRESULT
CRenderWindow::Initialize(
    void
    )
{
    //
    // Get the config information.
    //
    if ( m_pCfg == NULL )
    {
        return E_FAIL;
    }

    TCHAR szOutputCSV[ MAX_PATH ];
    m_pCfg->ReadString( _T( "Logging" ), _T( "OutputFile" ), szOutputCSV, MAX_PATH, _T( "" ) );
    CStringA sOutputCSV = szOutputCSV;
    if ( !sOutputCSV.IsEmpty() )
    {
        m_OutputCsv.open( sOutputCSV );
    }

    m_bFPSLogging = m_pCfg->ReadInt( _T( "Logging" ), _T( "FPS" ), 0 ) != 0;

    TCHAR szWindowName[ MAX_PATH ];
    m_pCfg->ReadString( _T( "Window" ), _T( "Name" ), szWindowName, MAX_PATH, _T( "Window" ) );
    m_WindowWidth = m_pCfg->ReadInt( _T( "Window" ), _T( "Width" ), 640 );
    m_WindowHeight = m_pCfg->ReadInt( _T( "Window" ), _T( "Height"), 480 );

    m_RunTime = m_pCfg->ReadFloat( _T( "Window" ), _T( "RunTime" ), FLT_MAX );

    //
    // Register the window's class.
    //
    WNDCLASSEX wcex;
    ZERO_STRUCT( wcex );

    wcex.cbSize = sizeof wcex;
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInstance;
    wcex.hIcon = NULL; //LoadIcon( m_hInstance, ( LPCTSTR )IDI_DX10BENCHMARK );
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = sm_pszWndClass;
    wcex.hIconSm = NULL; //LoadIconEx( m_hInstance, ( LPCTSTR )IDI_DX10BENCHMARK );

    if ( (m_hClass = RegisterClassEx( &wcex )) == 0 )
    {
        return E_FAIL;
    }

    //
    // Create the window.
    //
    RECT rcWnd = { 0, 0, m_WindowWidth, m_WindowHeight };
    ::AdjustWindowRect( &rcWnd, WS_DLGFRAME | WS_SYSMENU, FALSE );

    m_hWnd = ::CreateWindow( sm_pszWndClass, szWindowName,
                             WS_DLGFRAME | WS_SYSMENU,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top,
                             NULL, NULL, m_hInstance, NULL );

    if ( m_hWnd == NULL )
    {
        return E_FAIL;
    }

    //
    // Show the window.
    //
    ::ShowWindow( m_hWnd, SW_SHOW );
    ::UpdateWindow( m_hWnd );

    //
    // Update the output.
    //
    if ( m_OutputCsv.is_open() )
    {
        m_OutputCsv << "Window";
        m_OutputCsv << ",Width," << m_WindowWidth << std::endl;
        m_OutputCsv << ",Height," << m_WindowHeight << std::endl;
        m_OutputCsv << std::endl;
        m_OutputCsv.flush();
    }

    return S_OK;
}


/**************************************************************************************************\
Name: 
Description: 
Input: 
Returns: 
\**************************************************************************************************/
void
CRenderWindow::ProcessMessageLoop(
    void
    )
{
    //
    // Get the performance frequency.
    //
    LARGE_INTEGER PerfFreq;
    ::QueryPerformanceFrequency( &PerfFreq );
    double PerfFrequency = (double)PerfFreq.QuadPart;

    float DeltaTime = 0.0f;

    //
    // Loop until "quit" is received.
    //
    MSG Msg;
    Msg.message = 0;

    while ( WM_QUIT != Msg.message )
    {
        if ( ::PeekMessage( &Msg, NULL, 0, 0, PM_REMOVE ) )
        {
            ::TranslateMessage( &Msg );
            ::DispatchMessage( &Msg );
        }
        else
        {
            LARGE_INTEGER Count;
            LARGE_INTEGER PrevCount;
            ::QueryPerformanceCounter( &PrevCount );

            if ( !Update( DeltaTime ) )
            {
                ::DestroyWindow( m_hWnd );
            }

            ::QueryPerformanceCounter( &Count );
            DeltaTime = static_cast<float>(static_cast<double>(Count.QuadPart - PrevCount.QuadPart) / PerfFrequency);

            if ( m_RunTime != FLT_MAX )
            {
                m_RunTime -= DeltaTime;

                if ( m_RunTime <= 0.0f )
                {
                    ::DestroyWindow( m_hWnd );
                }
            }

            if ( DeltaTime < m_MinTime )
            {
                m_MinTime = DeltaTime;
            }

            if ( DeltaTime > m_MaxTime )
            {
                m_MaxTime = DeltaTime;
            }

            m_AvgTime *= m_FrameCount;
            m_AvgTime += DeltaTime;
            m_FrameCount += 1.0f;
            m_AvgTime /= m_FrameCount;
        }
    }

    if ( m_bFPSLogging )
    {
        m_OutputCsv << "FPS";
        m_OutputCsv << ",Max," << (1.0f / m_MinTime) << std::endl;
        m_OutputCsv << ",Min," << (1.0f / m_MaxTime) << std::endl;
        m_OutputCsv << ",Avg," << (1.0f / m_AvgTime) << std::endl;
        m_OutputCsv << ",Count," << ((int)m_FrameCount) << std::endl;
        m_OutputCsv << std::endl;
    }

    m_hWnd = NULL;
}


HRESULT
CRenderWindow::FindResource(
    LPTSTR pszResPathName,
    LPCTSTR pszDirName,
    LPCTSTR pszResFileName
    )
{
    WIN32_FIND_DATA fd;

    //
    // Check the root first.
    //
    HANDLE hFindFile = ::FindFirstFile( pszResFileName, &fd );

    if ( hFindFile != INVALID_HANDLE_VALUE )
    {
        wcscpy_s( pszResPathName, MAX_PATH, fd.cFileName );
        ::FindClose( hFindFile );
        return true;
    }

    //
    // Check the directories going up one at a time.
    //
    CString sFileName = pszDirName;
    sFileName += "\\";
    sFileName += pszResFileName;

    for ( int cUpLevels=3; cUpLevels > 0; cUpLevels-- )
    {
        hFindFile = ::FindFirstFile( sFileName, &fd );

        if ( hFindFile != INVALID_HANDLE_VALUE )
        {
            ::GetFullPathName( sFileName, MAX_PATH, pszResPathName, NULL );
            ::FindClose( hFindFile );
            return S_OK;
        }

        sFileName = "..\\" + sFileName;
    }

    return E_FAIL;
}


/**************************************************************************************************\
Name: 
Description: 
Input: 
Returns: 
\**************************************************************************************************/
LRESULT CALLBACK
CRenderWindow::WndProc(
    HWND hWnd,
    UINT Msg,
    WPARAM wParam,
    LPARAM lParam
    )
{
    switch( Msg )
    {
    case WM_KEYDOWN:
        if ( wParam == VK_ESCAPE )
        {
            ::PostQuitMessage( 0 );
        }
        break;

    case WM_DESTROY:
        ::PostQuitMessage( 0 );
        break;

    default:
        return ::DefWindowProc( hWnd, Msg, wParam, lParam );
    }

    return 0;
}


/**************************************************************************************************\
Name: 
Description: 
Input: 
Returns: 
\**************************************************************************************************/
CD3D10RenderWindow::CD3D10RenderWindow(
    HINSTANCE hInstance,
    LPCTSTR pszCfgFileName
    )
    : CRenderWindow( hInstance, pszCfgFileName )
{
    m_pDevice = NULL;
    m_pSwapChain = NULL;
    m_pBackBufferTex = NULL;
    m_pBackBufferRTV = NULL;
    m_pBackBufferDSV = NULL;
    m_Format = DXGI_FORMAT_R8G8B8A8_UNORM;
}


/**************************************************************************************************\
Name: 
Description: 
Input: 
Returns: 
\**************************************************************************************************/
CD3D10RenderWindow::~CD3D10RenderWindow(
    void
    )
{
    if ( m_pDevice != NULL )
    {
        m_pDevice->ClearState();
    }

    SAFE_RELEASE( m_pBackBufferDSV );
    SAFE_RELEASE( m_pBackBufferRTV );
    SAFE_RELEASE( m_pBackBufferTex );
    if ( m_pSwapChain != NULL )
    {
        m_pSwapChain->SetFullscreenState( FALSE, NULL );
        SAFE_RELEASE( m_pSwapChain );
    }
    SAFE_RELEASE( m_pDevice );
}

/**************************************************************************************************\
Name: 
Description: 
Input: 
Returns: 
\**************************************************************************************************/
HRESULT
CD3D10RenderWindow::Initialize(
    void
    )
{
    HRESULT hr;

    //
    // Call the base class.
    //
    VR( __super::Initialize() );

    //
    // Get the config information.
    //
    TCHAR szFormat[ 50 ];
    m_pCfg->ReadString( _T( "Direct3D" ), _T( "DisplayFormat" ), szFormat, 50 );
    m_Format = FormatCfgToDXGI( szFormat );

    if ( m_Format == DXGI_FORMAT_UNKNOWN )
    {
        return E_FAIL;
    }

    m_BufferCount = m_pCfg->ReadInt( _T( "Direct3D" ) , _T( "BufferCount" ), 2 );
    int Windowed = m_pCfg->ReadInt( _T( "Direct3D" ) , _T( "Windowed" ), 1 );

    //
    // Update the output.
    //
    if ( m_OutputCsv.is_open() )
    {
        CStringA sFormat = szFormat;

        m_OutputCsv << "Direct3D";
        m_OutputCsv << ",DisplayFormat," << sFormat << std::endl;
        m_OutputCsv << ",BufferCount," << m_BufferCount << std::endl;
        m_OutputCsv << ",Windowed," << (Windowed != 0 ? "True" : "False" ) << std::endl;
        m_OutputCsv << std::endl;
        m_OutputCsv.flush();
    }

    //
    // Create the device and the swap chain.
    //
    UINT DeviceFlags = 0;
#ifdef _DEBUG
    DeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif

    DXGI_SWAP_CHAIN_DESC scd;
    ZERO_STRUCT( scd );

    scd.BufferDesc.Width = m_WindowWidth;
    scd.BufferDesc.Height = m_WindowHeight;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferDesc.Format = m_Format;
    scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.BufferCount = m_BufferCount;
    scd.OutputWindow = m_hWnd;
    scd.Windowed = Windowed != 0;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    VR( D3D10CreateDeviceAndSwapChain( NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, DeviceFlags,
                                       D3D10_SDK_VERSION, &scd, &m_pSwapChain, &m_pDevice ) );

    //
    // Get the back buffer and create and RTV for it.
    //
    VR( m_pSwapChain->GetBuffer( 0, __uuidof( ID3D10Texture2D ), (LPVOID*)&m_pBackBufferTex ) );

    VR( m_pDevice->CreateRenderTargetView( m_pBackBufferTex, NULL, &m_pBackBufferRTV ) );

    //
    // Create the depth-stencil view.
    //
    D3D10_TEXTURE2D_DESC dsd;
    ZERO_STRUCT( dsd );

    dsd.Width = m_WindowWidth;
    dsd.Height = m_WindowHeight;
    dsd.MipLevels = 1;
    dsd.ArraySize = 1;
    dsd.Format = DXGI_FORMAT_D32_FLOAT;
    dsd.SampleDesc.Count = 1;
    dsd.SampleDesc.Quality = 0;
    dsd.Usage = D3D10_USAGE_DEFAULT;
    dsd.BindFlags = D3D10_BIND_DEPTH_STENCIL;
    dsd.CPUAccessFlags = 0;
    dsd.MipLevels = 0;

    ID3D10Texture2D* pDepthTex;
    VR( m_pDevice->CreateTexture2D( &dsd, NULL, &pDepthTex ) );

    D3D10_DEPTH_STENCIL_VIEW_DESC dsvd;
    ZERO_STRUCT( dsvd );

    dsvd.Format = dsd.Format;
    dsvd.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
    dsvd.Texture2D.MipSlice = 0;

    VR( m_pDevice->CreateDepthStencilView( pDepthTex, &dsvd, &m_pBackBufferDSV ) );

    SAFE_RELEASE( pDepthTex );

    //
    // Set the view port
    //
    D3D10_VIEWPORT vp;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    vp.Width = m_WindowWidth;
    vp.Height = m_WindowHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    m_pDevice->RSSetViewports( 1, &vp );

    return S_OK;
}


DXGI_FORMAT
CD3D10RenderWindow::FormatCfgToDXGI(
    LPCTSTR pszFormat
    )
{
    CString sFormat = pszFormat;

    if ( sFormat == _T( "UNKNOWN" ) )
    {
        return DXGI_FORMAT_UNKNOWN;
    }
    else if ( sFormat == _T( "R32G32B32A32_TYPELESS" ) )
    {
        return DXGI_FORMAT_R32G32B32A32_TYPELESS;
    }
    else if ( sFormat == _T( "R32G32B32A32_FLOAT" ) )
    {
        return DXGI_FORMAT_R32G32B32A32_FLOAT;
    }
    else if ( sFormat == _T( "R32G32B32A32_UINT" ) )
    {
        return DXGI_FORMAT_R32G32B32A32_UINT;
    }
    else if ( sFormat == _T( "R32G32B32A32_SINT" ) )
    {
        return DXGI_FORMAT_R32G32B32A32_SINT;
    }
    else if ( sFormat == _T( "R32G32B32_TYPELESS" ) )
    {
        return DXGI_FORMAT_R32G32B32_TYPELESS;
    }
    else if ( sFormat == _T( "R32G32B32_FLOAT" ) )
    {
        return DXGI_FORMAT_R32G32B32_FLOAT;
    }
    else if ( sFormat == _T( "R32G32B32_UINT" ) )
    {
        return DXGI_FORMAT_R32G32B32_UINT;
    }
    else if ( sFormat == _T( "R32G32B32_SINT" ) )
    {
        return DXGI_FORMAT_R32G32B32_SINT;
    }
    else if ( sFormat == _T( "R16G16B16A16_TYPELESS" ) )
    {
        return DXGI_FORMAT_R16G16B16A16_TYPELESS;
    }
    else if ( sFormat == _T( "R16G16B16A16_FLOAT" ) )
    {
        return DXGI_FORMAT_R16G16B16A16_FLOAT;
    }
    else if ( sFormat == _T( "R16G16B16A16_UNORM" ) )
    {
        return DXGI_FORMAT_R16G16B16A16_UNORM;
    }
    else if ( sFormat == _T( "R16G16B16A16_UINT" ) )
    {
        return DXGI_FORMAT_R16G16B16A16_UINT;
    }
    else if ( sFormat == _T( "R16G16B16A16_SNORM" ) )
    {
        return DXGI_FORMAT_R16G16B16A16_SNORM;
    }
    else if ( sFormat == _T( "R16G16B16A16_SINT" ) )
    {
        return DXGI_FORMAT_R16G16B16A16_SINT;
    }
    else if ( sFormat == _T( "R32G32_TYPELESS" ) )
    {
        return DXGI_FORMAT_R32G32_TYPELESS;
    }
    else if ( sFormat == _T( "R32G32_FLOAT" ) )
    {
        return DXGI_FORMAT_R32G32_FLOAT;
    }
    else if ( sFormat == _T( "R32G32_UINT" ) )
    {
        return DXGI_FORMAT_R32G32_UINT;
    }
    else if ( sFormat == _T( "R32G32_SINT" ) )
    {
        return DXGI_FORMAT_R32G32_SINT;
    }
    else if ( sFormat == _T( "R32G8X24_TYPELESS" ) )
    {
        return DXGI_FORMAT_R32G8X24_TYPELESS;
    }
    else if ( sFormat == _T( "D32_FLOAT_S8X24_UINT" ) )
    {
        return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
    }
    else if ( sFormat == _T( "R32_FLOAT_X8X24_TYPELESS" ) )
    {
        return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
    }
    else if ( sFormat == _T( "X32_TYPELESS_G8X24_UINT" ) )
    {
        return DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;
    }
    else if ( sFormat == _T( "R10G10B10A2_TYPELESS" ) )
    {
        return DXGI_FORMAT_R10G10B10A2_TYPELESS;
    }
    else if ( sFormat == _T( "R10G10B10A2_UNORM" ) )
    {
        return DXGI_FORMAT_R10G10B10A2_UNORM;
    }
    else if ( sFormat == _T( "R10G10B10A2_UINT" ) )
    {
        return DXGI_FORMAT_R10G10B10A2_UINT;
    }
    else if ( sFormat == _T( "R11G11B10_FLOAT" ) )
    {
        return DXGI_FORMAT_R11G11B10_FLOAT;
    }
    else if ( sFormat == _T( "R8G8B8A8_TYPELESS" ) )
    {
        return DXGI_FORMAT_R8G8B8A8_TYPELESS;
    }
    else if ( sFormat == _T( "R8G8B8A8_UNORM" ) )
    {
        return DXGI_FORMAT_R8G8B8A8_UNORM;
    }
    else if ( sFormat == _T( "R8G8B8A8_UNORM_SRGB" ) )
    {
        return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    }
    else if ( sFormat == _T( "R8G8B8A8_UINT" ) )
    {
        return DXGI_FORMAT_R8G8B8A8_UINT;
    }
    else if ( sFormat == _T( "R8G8B8A8_SNORM" ) )
    {
        return DXGI_FORMAT_R8G8B8A8_SNORM;
    }
    else if ( sFormat == _T( "R8G8B8A8_SINT" ) )
    {
        return DXGI_FORMAT_R8G8B8A8_SINT;
    }
    else if ( sFormat == _T( "R16G16_TYPELESS" ) )
    {
        return DXGI_FORMAT_R16G16_TYPELESS;
    }
    else if ( sFormat == _T( "R16G16_FLOAT" ) )
    {
        return DXGI_FORMAT_R16G16_FLOAT;
    }
    else if ( sFormat == _T( "R16G16_UNORM" ) )
    {
        return DXGI_FORMAT_R16G16_UNORM;
    }
    else if ( sFormat == _T( "R16G16_UINT" ) )
    {
        return DXGI_FORMAT_R16G16_UINT;
    }
    else if ( sFormat == _T( "R16G16_SNORM" ) )
    {
        return DXGI_FORMAT_R16G16_SNORM;
    }
    else if ( sFormat == _T( "R16G16_SINT" ) )
    {
        return DXGI_FORMAT_R16G16_SINT;
    }
    else if ( sFormat == _T( "R32_TYPELESS" ) )
    {
        return DXGI_FORMAT_R32_TYPELESS;
    }
    else if ( sFormat == _T( "D32_FLOAT" ) )
    {
        return DXGI_FORMAT_D32_FLOAT;
    }
    else if ( sFormat == _T( "R32_FLOAT" ) )
    {
        return DXGI_FORMAT_R32_FLOAT;
    }
    else if ( sFormat == _T( "R32_UINT" ) )
    {
        return DXGI_FORMAT_R32_UINT;
    }
    else if ( sFormat == _T( "R32_SINT" ) )
    {
        return DXGI_FORMAT_R32_SINT;
    }
    else if ( sFormat == _T( "R24G8_TYPELESS" ) )
    {
        return DXGI_FORMAT_R24G8_TYPELESS;
    }
    else if ( sFormat == _T( "D24_UNORM_S8_UINT" ) )
    {
        return DXGI_FORMAT_D24_UNORM_S8_UINT;
    }
    else if ( sFormat == _T( "R24_UNORM_X8_TYPELESS" ) )
    {
        return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    }
    else if ( sFormat == _T( "X24_TYPELESS_G8_UINT" ) )
    {
        return DXGI_FORMAT_X24_TYPELESS_G8_UINT;
    }
    else if ( sFormat == _T( "R8G8_TYPELESS" ) )
    {
        return DXGI_FORMAT_R8G8_TYPELESS;
    }
    else if ( sFormat == _T( "R8G8_UNORM" ) )
    {
        return DXGI_FORMAT_R8G8_UNORM;
    }
    else if ( sFormat == _T( "R8G8_UINT" ) )
    {
        return DXGI_FORMAT_R8G8_UINT;
    }
    else if ( sFormat == _T( "R8G8_SNORM" ) )
    {
        return DXGI_FORMAT_R8G8_SNORM;
    }
    else if ( sFormat == _T( "R8G8_SINT" ) )
    {
        return DXGI_FORMAT_R8G8_SINT;
    }
    else if ( sFormat == _T( "R16_TYPELESS" ) )
    {
        return DXGI_FORMAT_R16_TYPELESS;
    }
    else if ( sFormat == _T( "R16_FLOAT" ) )
    {
        return DXGI_FORMAT_R16_FLOAT;
    }
    else if ( sFormat == _T( "D16_UNORM" ) )
    {
        return DXGI_FORMAT_D16_UNORM;
    }
    else if ( sFormat == _T( "R16_UNORM" ) )
    {
        return DXGI_FORMAT_R16_UNORM;
    }
    else if ( sFormat == _T( "R16_UINT" ) )
    {
        return DXGI_FORMAT_R16_UINT;
    }
    else if ( sFormat == _T( "R16_SNORM" ) )
    {
        return DXGI_FORMAT_R16_SNORM;
    }
    else if ( sFormat == _T( "R16_SINT" ) )
    {
        return DXGI_FORMAT_R16_SINT;
    }
    else if ( sFormat == _T( "R8_TYPELESS" ) )
    {
        return DXGI_FORMAT_R8_TYPELESS;
    }
    else if ( sFormat == _T( "R8_UNORM" ) )
    {
        return DXGI_FORMAT_R8_UNORM;
    }
    else if ( sFormat == _T( "R8_UINT" ) )
    {
        return DXGI_FORMAT_R8_UINT;
    }
    else if ( sFormat == _T( "R8_SNORM" ) )
    {
        return DXGI_FORMAT_R8_SNORM;
    }
    else if ( sFormat == _T( "R8_SINT" ) )
    {
        return DXGI_FORMAT_R8_SINT;
    }
    else if ( sFormat == _T( "A8_UNORM" ) )
    {
        return DXGI_FORMAT_A8_UNORM;
    }
    else if ( sFormat == _T( "R1_UNORM" ) )
    {
        return DXGI_FORMAT_R1_UNORM;
    }
    else if ( sFormat == _T( "R9G9B9E5_SHAREDEXP" ) )
    {
        return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
    }
    else if ( sFormat == _T( "R8G8_B8G8_UNORM" ) )
    {
        return DXGI_FORMAT_R8G8_B8G8_UNORM;
    }
    else if ( sFormat == _T( "G8R8_G8B8_UNORM" ) )
    {
        return DXGI_FORMAT_G8R8_G8B8_UNORM;
    }
    else if ( sFormat == _T( "BC1_TYPELESS" ) )
    {
        return DXGI_FORMAT_BC1_TYPELESS;
    }
    else if ( sFormat == _T( "BC1_UNORM" ) )
    {
        return DXGI_FORMAT_BC1_UNORM;
    }
    else if ( sFormat == _T( "BC1_UNORM_SRGB" ) )
    {
        return DXGI_FORMAT_BC1_UNORM_SRGB;
    }
    else if ( sFormat == _T( "BC2_TYPELESS" ) )
    {
        return DXGI_FORMAT_BC2_TYPELESS;
    }
    else if ( sFormat == _T( "BC2_UNORM" ) )
    {
        return DXGI_FORMAT_BC2_UNORM;
    }
    else if ( sFormat == _T( "BC2_UNORM_SRGB" ) )
    {
        return DXGI_FORMAT_BC2_UNORM_SRGB;
    }
    else if ( sFormat == _T( "BC3_TYPELESS" ) )
    {
        return DXGI_FORMAT_BC3_TYPELESS;
    }
    else if ( sFormat == _T( "BC3_UNORM" ) )
    {
        return DXGI_FORMAT_BC3_UNORM;
    }
    else if ( sFormat == _T( "BC3_UNORM_SRGB" ) )
    {
        return DXGI_FORMAT_BC3_UNORM_SRGB;
    }
    else if ( sFormat == _T( "BC4_TYPELESS" ) )
    {
        return DXGI_FORMAT_BC4_TYPELESS;
    }
    else if ( sFormat == _T( "BC4_UNORM" ) )
    {
        return DXGI_FORMAT_BC4_UNORM;
    }
    else if ( sFormat == _T( "BC4_SNORM" ) )
    {
        return DXGI_FORMAT_BC4_SNORM;
    }
    else if ( sFormat == _T( "BC5_TYPELESS" ) )
    {
        return DXGI_FORMAT_BC5_TYPELESS;
    }
    else if ( sFormat == _T( "BC5_UNORM" ) )
    {
        return DXGI_FORMAT_BC5_UNORM;
    }
    else if ( sFormat == _T( "BC5_SNORM" ) )
    {
        return DXGI_FORMAT_BC5_SNORM;
    }
    else if ( sFormat == _T( "B5G6R5_UNORM" ) )
    {
        return DXGI_FORMAT_B5G6R5_UNORM;
    }
    else if ( sFormat == _T( "B5G5R5A1_UNORM" ) )
    {
        return DXGI_FORMAT_B5G5R5A1_UNORM;
    }
    else if ( sFormat == _T( "B8G8R8A8_UNORM" ) )
    {
        return DXGI_FORMAT_B8G8R8A8_UNORM;
    }
    else if ( sFormat == _T( "B8G8R8X8_UNORM" ) )
    {
        return DXGI_FORMAT_B8G8R8X8_UNORM;
    }
    else if ( sFormat == _T( "R10G10B10_XR_BIAS_A2_UNORM" ) )
    {
        return DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM;
    }
    else if ( sFormat == _T( "B8G8R8A8_TYPELESS" ) )
    {
        return DXGI_FORMAT_B8G8R8A8_TYPELESS;
    }
    else if ( sFormat == _T( "B8G8R8A8_UNORM_SRGB" ) )
    {
        return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
    }
    else if ( sFormat == _T( "B8G8R8X8_TYPELESS" ) )
    {
        return DXGI_FORMAT_B8G8R8X8_TYPELESS;
    }
    else if ( sFormat == _T( "B8G8R8X8_UNORM_SRGB" ) )
    {
        return DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;
    }
    else if ( sFormat == _T( "BC6H_TYPELESS" ) )
    {
        return DXGI_FORMAT_BC6H_TYPELESS;
    }
    else if ( sFormat == _T( "BC6H_UF16" ) )
    {
        return DXGI_FORMAT_BC6H_UF16;
    }
    else if ( sFormat == _T( "BC6H_SF16" ) )
    {
        return DXGI_FORMAT_BC6H_SF16;
    }
    else if ( sFormat == _T( "BC7_TYPELESS" ) )
    {
        return DXGI_FORMAT_BC7_TYPELESS;
    }
    else if ( sFormat == _T( "BC7_UNORM" ) )
    {
        return DXGI_FORMAT_BC7_UNORM;
    }
    else if ( sFormat == _T( "BC7_UNORM_SRGB" ) )
    {
        return DXGI_FORMAT_BC7_UNORM_SRGB;
    }

    return DXGI_FORMAT_UNKNOWN;
}


/**************************************************************************************************\
Name: 
Description: 
Input: 
Returns: 
\**************************************************************************************************/
bool
CD3D10RenderWindow::Update(
    float DeltaTime
    )
{
    m_pSwapChain->Present( 0, 0 );
    return true;
}
