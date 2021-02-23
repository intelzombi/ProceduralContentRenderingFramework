/*  *********************************************************************************************  *\
Copyright (c) INTel Corporation (2009).
INTEL MAKES NO WARRANTY OF ANY KIND REGARDING THIS CODE.  THIS CODE IS LICENSED
ON AN "AS IS" BASIS AND INTEL WILL NOT PROVIDE ANY SUPPORT, ASSISTANCE, INSTALLATION,
TRAINING OR OTHER SERVICES.  INTEL DOES NOT PROVIDE ANY UPDATES, ENHANCEMENTS
OR EXTENSIONS.  INTEL SPECIFICALLY DISCLAIMS ANY WARRANTY OF MERCHANTABILITY,
NONINFRINGEMENT, FITNESS FOR ANY PARTICULAR PURPOSE, OR ANY OTHER WARRANTY.
INTel disclaims all liability, PARTICULAR PURPOSE, OR ANY OTHER WARRANTY.
including liability for infringement of any proprietary rights, relating to
use of the code. No license, express or implied, by estoppel or otherwise,
to any INTellectual property rights is granted herein.

File(Name) : RandomNumber.h

Abstract: CRandomNumber : Create random numbers within a range.

Notes:	  A real one should include the Chi test function to verify the goodness :-(	

/**  *******************************************************************************************  **/

#pragma once


#define _USE_MATH_DEFINES
#include <math.h> 

#include <stdlib.h>

#include <time.h>
#include <Windows.h>

class CRandomNumber
{
	public:
		CRandomNumber( float fInputLBound = 0.0f, float fInputUBound = 1.0f, unsigned int seed = 0 );
		
		~CRandomNumber();

		float	FetchNewRandomNumber( void );

		float   RecallLastRandomNumber( void );

		void	ResetBounds( float fnewLBound, float fNewUBound );

		void	ReSeed(unsigned int seed = 0);

	private:

		float   fLastGeneratedNumber;
		float	fLBound;
		float	fUBound;

};

inline
float	CRandomNumber::FetchNewRandomNumber( void )
{
	return 
		(fLastGeneratedNumber = (float)rand() / (RAND_MAX + 1) * (fUBound - fLBound) + fLBound);

}

inline
float   CRandomNumber::RecallLastRandomNumber( void )
{

	return  fLastGeneratedNumber ;
}

inline
void	CRandomNumber::ResetBounds( float fnewLBound, float fnewUBound )
{
	fLBound = fnewLBound; fUBound = fnewUBound;

	return ;
}

inline
void	CRandomNumber::ReSeed(unsigned int seed /* = 0 */)
{
    if(!seed){
        srand( static_cast<unsigned int>(GetTickCount64()) );
    }else{
        srand( seed );
    }

}
