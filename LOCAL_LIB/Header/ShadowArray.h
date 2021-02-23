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

File(Name) : ShadowArray.h

Abstract:	Declaration & definition of ShadowArray class

Notes:

\**  *******************************************************************************************  **/


#pragma  once

template <typename ELEMENT_TYPE>
class	CShadowArray 
{
public:
	
	CShadowArray(int iSize);
	virtual ~CShadowArray();

    ELEMENT_TYPE& operator[](const int location);

	const	ELEMENT_TYPE*	const 	GetPtrToCurrent();
	ELEMENT_TYPE* const	GetPtrToShadow();
	
	void			UpdateShadow();
	void			SwapCurrentAndShadow();

protected:


private:

    int             iArraySize;
	ELEMENT_TYPE*   pCurrentArray;
	ELEMENT_TYPE*	pShadowArray;
	ELEMENT_TYPE*	pArray0;
	ELEMENT_TYPE*   pArray1;

    CShadowArray();
};


/***************************************************************************************************\
Function Name: CShadowArray

Description:	Constructor

Input: 
    NONE

Return: 
	NONE
\***************************************************************************************************/

template <typename ELEMENT_TYPE>
CShadowArray<ELEMENT_TYPE>::CShadowArray(int iSize)
    : iArraySize(iSize)
{
	pArray0			=	new ELEMENT_TYPE[iSize];
	pCurrentArray	=	pArray0;
	pArray1			=	new ELEMENT_TYPE[iSize];
	pShadowArray	=   pArray1;
}

/***************************************************************************************************\
Function Name: ~CShadowArray

Description:	Destructor

Input: 
    NONE

Return: 
	NONE
\***************************************************************************************************/

template <typename ELEMENT_TYPE>
CShadowArray<ELEMENT_TYPE>::~CShadowArray()
{
	delete	[]	pArray0;
	delete	[]	pArray1;
}


/***************************************************************************************************\
Function Name: operator[]

Description:   Overload for access.  This won't show up in the current array unless the 
               UpdateShadow() memeber function is called

Input: 
    const int location

Return: 
    ELEMENT_TYPE&
\***************************************************************************************************/

template <typename ELEMENT_TYPE>
ELEMENT_TYPE&
CShadowArray<ELEMENT_TYPE>::operator[]( const int location )
{
    return  pShadowArray[location];
  
}
/***************************************************************************************************\
Function Name: GetPtrToCurrent

Description:	Get a non-mutable, const pointer to the array that is current.

Input: 
    NONE

Return: 
	ELEMENT_TYPE*
\***************************************************************************************************/

template <typename ELEMENT_TYPE>
inline	const	ELEMENT_TYPE*	const
CShadowArray<ELEMENT_TYPE>::GetPtrToCurrent()
{
	return	pCurrentArray;
}

/***************************************************************************************************\
Function Name:	GetPtrToShadow

Description:	This returns a const pointer to a mutable shadow array.  Use this to
				change the data in the array, and then call ::SwapCurrentAndShadow() to 
				make the shadow to be the current array.

Input: 
    NONE

Return: 
	ELEMENT_TYPE*
\***************************************************************************************************/

template <typename ELEMENT_TYPE>
inline  ELEMENT_TYPE* const
CShadowArray<ELEMENT_TYPE>::GetPtrToShadow()
{
	return	pShadowArray;
}

/***************************************************************************************************\
Function Name: UpdateShadow

Description:	Make a copy of the data in the current to the shadow array.

Input: 
    NONE

Return: 
	NONE
\***************************************************************************************************/


template <typename ELEMENT_TYPE>
inline  void
CShadowArray<ELEMENT_TYPE>::UpdateShadow()
{
	memcpy(	pShadowArray, pCurrentArray, (sizeof  ELEMENT_TYPE) * iArraySize );

}

/***************************************************************************************************\
Function Name: SwapCurrentAndShadow

Description:	The 

Input: 
    NONE

Return: 
	void
\***************************************************************************************************/

template <typename ELEMENT_TYPE>
inline	void 
CShadowArray<ELEMENT_TYPE>::SwapCurrentAndShadow()
{
	ELEMENT_TYPE*	pTmp	= pCurrentArray;
	pCurrentArray			= pShadowArray;
	pShadowArray			= pTmp;

}

//------------------------------------------------------------------------------------------------

