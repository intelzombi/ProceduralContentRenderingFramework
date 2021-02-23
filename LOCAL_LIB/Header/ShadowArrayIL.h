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

File(Name) : ShadowArrayIL.h

Abstract:	Declaration of ShadowArray with Insecure Lock class.

Notes:
    The lock is insecure - there is no preventing the user from ignoring and overriding the lock.

\**  *******************************************************************************************  **/


#pragma  once

#include <intrin.h>


template <typename ELEMENT_TYPE>
class	CShadowArrayIL 
{
public:

    enum    { IL_OPEN = 0,  IL_SET	};

	CShadowArrayIL();	
	CShadowArrayIL(int iSize);
	~CShadowArrayIL();

	bool	ReleaseAllocatedMemory( void );
	bool	AllocateMemory( int iMemSize );

	bool    SetILock();
    bool    ReleaseILock();
    bool    GetLockSetStatus();

    ELEMENT_TYPE& operator[](const int location);

	const	ELEMENT_TYPE*	const 	GetPtrToCurrent() const;
	ELEMENT_TYPE*			const	GetPtrToShadow()  const;
	
	void			UpdateShadow();
	void			SwapCurrentAndShadow();

protected:


private:
    int             iArraySize;

	ELEMENT_TYPE*   pCurrentArray;
	ELEMENT_TYPE*	pShadowArray;
	ELEMENT_TYPE*	pArray0;
	ELEMENT_TYPE*   pArray1;

    long   volatile lvLockFlag;

};

/***************************************************************************************************\
Function Name: CShadowArrayIL

Description:	Default constructor.   

Input: 
    NONE

Return: 
	NONE
\***************************************************************************************************/

template <typename ELEMENT_TYPE>
CShadowArrayIL<ELEMENT_TYPE>::CShadowArrayIL( void )
	:	pCurrentArray(0), pShadowArray(0), pArray0(0), pArray1(0), iArraySize(0)
{
	
}    // CShadowArrayIL


/***************************************************************************************************\
Function Name: CShadowArrayIL

Description:	Constructor

Input: 
    NONE

Return: 
	NONE
\***************************************************************************************************/

template <typename ELEMENT_TYPE>
CShadowArrayIL<ELEMENT_TYPE>::CShadowArrayIL(int iSize)
    :   iArraySize(iSize),  lvLockFlag(IL_OPEN) 
{
	pArray0			=	new ELEMENT_TYPE[iSize];
	pCurrentArray	=	pArray0;
	pArray1			=	new ELEMENT_TYPE[iSize];
	pShadowArray	=   pArray1;
}

/***************************************************************************************************\
Function Name: ~CShadowArrayIL

Description:	Destructor

Input: 
    NONE

Return: 
	NONE
\***************************************************************************************************/

template <typename ELEMENT_TYPE>
CShadowArrayIL<ELEMENT_TYPE>::~CShadowArrayIL()
{
	delete	[]	pArray0;
	delete	[]	pArray1;
}

/***************************************************************************************************\
Function Name:	ReleaseAllocatedMemory

Description:	Delete the memory that has been allocated for the array   

Input: 
    NONE

Return: 
	bool	True if the memory allocated earlier had not been released.
\***************************************************************************************************/

template <typename ELEMENT_TYPE>
inline  bool  
CShadowArrayIL<ELEMENT_TYPE>::ReleaseAllocatedMemory( void )
{
	bool	bRetVal = false;
	if( pArray0 )
	{
		delete	[]	pArray0;
		delete  []  pArray1;
		iArraySize	= 0;

		bRetVal = true;
	}

	return	bRetVal;

}    // bool	ReleaseAllocatedMemory



/***************************************************************************************************\
Function Name: bool	AllocateMemory

Description:	Allocate memory for the shadow and the current arrays. 

Input: 
    int iMemSize

Return: 
	bool	True if both allocations succeeded.
\***************************************************************************************************/
template <typename ELEMENT_TYPE>
inline  bool  
CShadowArrayIL<ELEMENT_TYPE>::AllocateMemory( int iMemSize )
{
	if( pArray0 )
	{
		delete  [] pArray0;
		pArray0	=	0;
	}

	if( pArray1 )
	{
		delete	[]	pArray1;
		pArray1	=	0;
	}

	pArray0	=	new ELEMENT_TYPE[iMemSize];
	pArray1	=	new ELEMENT_TYPE[iMemSize];

	if( !pArray1 )
	{
		// second array allocation failed; delete the first one if it has succeeded
		if( pArray0 ) 
			delete	[]	pArray0;
			;
	}

	return		(pArray0 && pArray1);

}    // bool	AllocateMemory


/***************************************************************************************************\
Function Name: SetILock

Description:  
        Check if the lock set, and IF IT IS NOT SET, set it - both in one atomic operation

Input: 
    NONE

Return: 
    bool    TRUE if the lock status changed.

\***************************************************************************************************/
template <typename ELEMENT_TYPE>
inline  bool  
CShadowArrayIL<ELEMENT_TYPE>::SetILock( void )
{
    long    volatile lvDestValue = IL_SET;

    return
        ( (long)IL_OPEN ==
        _InterlockedCompareExchange( &lvLockFlag, lvDestValue, IL_OPEN ) );

}

/***************************************************************************************************\
Function Name: ReleaseILock

Description:   
        Test if the lock is set, and if it is set, release it.  This is an atomic operation.
Input: 
    NONE

Return: 
        bool    Return TRUE if the lock is successfully released.
\***************************************************************************************************/

template <typename ELEMENT_TYPE>
bool  
CShadowArrayIL<ELEMENT_TYPE>::ReleaseILock( void )
{
    long    volatile lvDestValue = IL_OPEN;

    return
        ( (long)IL_SET ==
        _InterlockedCompareExchange( &lvLockFlag, lvDestValue, IL_SET ) );
}

/***************************************************************************************************\
Function Name: GetLockSetStatus

Description:   
        Return the status of whether the lock is set.
Input: 
    NONE

Return: 
        bool    TRUE if the lock is set; else FALSE
\***************************************************************************************************/

template <typename ELEMENT_TYPE>
inline  bool  
CShadowArrayIL<ELEMENT_TYPE>::  GetLockSetStatus( void )
{
    return      ( (lvLockFlag == static_cast<long>(IL_OPEN)) ? false : true ) ;
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
CShadowArrayIL<ELEMENT_TYPE>::operator[]( const int location )
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
CShadowArrayIL<ELEMENT_TYPE>::GetPtrToCurrent()    const
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
CShadowArrayIL<ELEMENT_TYPE>::GetPtrToShadow()	const 
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
CShadowArrayIL<ELEMENT_TYPE>::UpdateShadow()
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
CShadowArrayIL<ELEMENT_TYPE>::SwapCurrentAndShadow()
{
	ELEMENT_TYPE*	pTmp	= pCurrentArray;
	pCurrentArray			= pShadowArray;
	pShadowArray			= pTmp;

}

//------------------------------------------------------------------------------------------------

