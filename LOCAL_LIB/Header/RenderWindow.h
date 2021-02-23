
#pragma once


#include <fstream>


class CRenderWindow
{
protected:

    static LPCTSTR sm_pszWndClass;

    HINSTANCE m_hInstance;
    ATOM m_hClass;
    HWND m_hWnd;

    CINIFileReader* m_pCfg;

    std::ofstream m_OutputCsv;

    UINT m_WindowWidth;
    UINT m_WindowHeight;

    float m_RunTime;

    bool m_bFPSLogging;
    float m_MinTime;
    float m_MaxTime;
    float m_AvgTime;
    float m_FrameCount;


public:

    CRenderWindow( HINSTANCE hInstance, LPCTSTR pszCfgFileName );
    virtual ~CRenderWindow( void );

    HRESULT Initialize( void );

    void ProcessMessageLoop( void );


protected:

    HRESULT FindResource( LPTSTR pszResPathName, LPCTSTR pszDirName, LPCTSTR pszResFileName );

    virtual bool Update( float DeltaTime ) = 0;


private:

    static LRESULT CALLBACK WndProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
};


class CD3D10RenderWindow : public CRenderWindow
{
protected:

    ID3D10Device* m_pDevice;
    IDXGISwapChain* m_pSwapChain;
    ID3D10Texture2D* m_pBackBufferTex;
    ID3D10RenderTargetView* m_pBackBufferRTV;
    ID3D10DepthStencilView* m_pBackBufferDSV;

    DXGI_FORMAT m_Format;
    UINT m_BufferCount;


public:

    CD3D10RenderWindow( HINSTANCE hInstance, LPCTSTR pszCfgFileName );
    virtual ~CD3D10RenderWindow( void );

    HRESULT Initialize( void );


protected:

    DXGI_FORMAT FormatCfgToDXGI( LPCTSTR pszFormat );

    //
    // Subclasses call this AFTER doing their own update processing,
    //  essentially just does a present.
    //
    virtual bool Update( float DeltaTime );
};
