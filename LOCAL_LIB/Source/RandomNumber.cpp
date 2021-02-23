
#include "stdafx.h"
#include <RandomNumber.h>


CRandomNumber::CRandomNumber(float fInputLBound /* = 0.0f */, float fInputUBound /* = 1.0f */, unsigned int seed /* = 0 */ )
: fLBound(fInputLBound), fUBound(fInputUBound), fLastGeneratedNumber(0)
{
	//  Seed the random number generator
    if(!seed){
        srand( static_cast<unsigned int>(GetTickCount64()) );
    }else{
        srand( seed );
    }

}


CRandomNumber::~CRandomNumber() {  }

