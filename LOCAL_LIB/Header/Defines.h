
#define SAFE_DELETE( p )            if ( (p) != NULL ) { delete (p); (p) = NULL; }
#define SAFE_DELETE_ARRAY( p )      if ( (p) != NULL ) { delete[] (p); (p) = NULL; }
#define SAFE_RELEASE( p )           if ( (p) != NULL ) { (p)->Release(); (p) = NULL; }

#define ZERO_STRUCT( s )            ::ZeroMemory( &s, sizeof s );

#define VR( f )                     { hr = (f); if ( FAILED( hr ) ) { return hr; } }
