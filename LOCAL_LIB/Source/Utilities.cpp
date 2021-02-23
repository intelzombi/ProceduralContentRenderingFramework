
#include "stdafx.h"
#include <Utilities.h>


// If the argument is TRUE, we create a console and synchronize it so that printf and 
// cin, cout, cerr all redirect to the console.  If you already have the console open 
// pass "FALSE" for the argument
BOOL InitConsoleIOMessageMode( BOOL bCreateConsole )
{
	BOOL bRetVal = TRUE;

	if( bCreateConsole )
	{
		bRetVal = AllocConsole();
	}
#pragma warning( disable : 4996 )
	bRetVal =	freopen( "CONOUT$", "a", stdout ) != NULL &&
				freopen( "CONIN$",  "r", stdin  ) != NULL &&
				freopen( "CONERR$", "a", stderr ) != NULL;
#pragma warning( default : 4996 )
	// synchronize iostreams with standard i/o:
	if ( bRetVal )
		std::ios::sync_with_stdio();

	return bRetVal;
}