//
// Copyright (C) 2002 2006, Intel Corp.  All rights reserved.
// Notice:  No warranties are made regarding the functionality or suitability of this code.
//			By using this code, you agree that Intel is not liable for any loss or damage
//			that may result.

#ifndef WINCPUID_H_INCLUDE
#define WINCPUID_H_INCLUDE
#pragma pack(push, WINCPUID_H, 16) // explicitly set alignment for compatibilty with projects using a different alignment

//#ifndef _M_X64
#define USE_LIBCPUID
//#endif
#ifdef USE_LIBCPUID
#include "LibCPUID.h"
#endif

// *** See CWinCPUID.h for an easier-to-use C++ interface ***

/* This is a simplified interface to the CPUID package.  It is meant to allow applications
   to easily check for CPU and OS support of MMX, SSE, and SSE-2 instructions as well as
   measure the CPU speed.

   *Basic Usage* To get the CPU ID and Features:
   1.	Declare a variable to hold the info and initalize it:
			CPUINFO_T WinCPUID_Info = CPUINFO_T_INIT_VAL;
   2.	If you want cache info, allocate a buffer for the cache info as follows:
			WinCPUID_Info.pCacheInfo = (CACHEINFO_T*)malloc(sizeof(CACHEINFO_T)*NCACHETYPES);
   3.	Get the info.  If you call this function more than once, it simply passes back existing (cached) data.
		To calculate the CPU frequency, set bGetFreq to 1.  Otherwise llCPUFreq will be zero.
			WinCPUID_Init(bGetFreq, &WinCPUID_Info);

		Note:  If llCPUFreq returns as -1, the CPU does not support the RDTSC needed to compute the frequency.

		** See testCPUID.cpp for usage example.

  *Building* - WinCPUID builds as a dll or as a static lib.
  To use the dll:
	1.	Include WinCPUID.h
	2.	Link to	WinCPUID.lib	(stub lib)
	3.	Put WinCPUID.dll in same directory as executable (or in path)
  To use the static lib:
	1.	Define WINCPUID_STATIC for any .c, .cpp, or .h files accessing WinCPUID.
	    Must be defined before including WinCPUID.h
	2.	Include WinCPUID.h
	3.	Link to WinCPUIDlib.lib (the static lib) and to winmm.lib (a Windows DLL).
	    Either put WinCPUIDlib.lib in the same directory as your source, or add the library path in
		Project->Settings->Link->Input->Additional Library Path
  */


// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the WINCPUID_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// WINCPUID_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef WINCPUID_STATIC
#define WINCPUID_API
#else
#ifdef WINCPUID_EXPORTS
#define WINCPUID_API __declspec(dllexport)
#else
#define WINCPUID_API __declspec(dllimport)
#endif
#endif

#ifndef WINCPUID_STATIC
// For dynamic version-checking
#define WINCPUID_VERSION_STR	"2006.09.21.0"
#endif

typedef unsigned long DWORD;
typedef __int64 LONGLONG;
typedef int BOOL;
typedef unsigned short UWORD;
typedef DWORD_PTR CPUAFFINITY;

// JS - Detected CPU Vendors by getProcStr function
#ifndef USE_LIBCPUID
typedef enum CPU_VENDORS
{
	VENDOR_UNKNOWN = 0,
	VENDOR_INTEL,
	VENDOR_AMD,
	VENDOR_FORCEDWORD = 0xFFFFFFFF
} CPU_VENDORS;

typedef struct CPUCOREINFO_s {
	DWORD	dwCPUType;
	DWORD	dwCPUFamily;
	DWORD	dwCPUModel;
	DWORD	dwCPUExtFamily;
	DWORD	dwCPUExtModel;
	DWORD	dwCPUStepping;
	DWORD	dwCPUFeatures;			// result from edx when cpuid called with eax=1
	DWORD	dwCPUPscNewECX;			// result from ecx when cpuid called with eax=1
	DWORD	dwCPUAMDExtSignature;	// AMD extended features
	DWORD	dwBrandIndex;
	DWORD	dwCLFlushLineSize;
	DWORD	dwCPUExtFeatures;		// result from ebx when cpuid called with eax=1
	DWORD	dwFeatures8_1_edx;		// edx = cpuid(0x80000001), NX / EM64T
	DWORD	dwFeatures8_1_ecx;		// Intel - Reserved.  AMD - "AMD Feature Support", esp. CMP legacy, bit 1
	DWORD	dwFeatures8_8_eax;		// eax = cpuid(0x80000008), Virtual/Physical Address size
	DWORD	dwFeatures8_8_ecx;		// Intel - Reserved.  AMD - [7:0] is "Physical Core Count"
} CPUCOREINFO_T;
#endif

typedef struct {
	DWORD	PhysAddrBits;
	DWORD	VirtAddrBits;
} CPU_ADDR_BITS_T;

typedef struct cacheinfo_s {
	int	sizeKB;
	int	setAssoc;
	int	lineSize;
	int lines;						// JS
	int sets;
	int ThreadsPerCache;			// Number of threads sharing this cache
	BOOL bExpCache;					// Is this from the explicit cache descriptor?
}	CACHEINFO_T;

typedef enum {
	htError = 0,
	htNotDetected,
	htNotEnabled, 
	htNotAvailable,
	htAvailable,					// Hyper-Threaded and/or Multi-Core processors ready for use
	htSystemMismatchError,			// Indicates that the system's CPU count did not correlate with what is detected by WinCPUID
	htNumResults,
	htForceDWORD = 0xFFFFFFFF
} eHTResultCode;

typedef struct {
	CPUAFFINITY dwCoreAffinity;		// Mask for the first logical on core
	CPUAFFINITY dwLogAffinity;		// Mask for the remaining logicals (non-physical) CPU(s) on this core
	int coreID;
	int LLC_ID;						// ID of last-level cache on package
	int LLC_Index;					// Index (zero-based count) of the LLC this core uses
	int LLC_SharedIndex;			// 0 for first core using LLC, 1 for 2nd using same LLC, etc.
} COREPROCAFF_T;

typedef struct {
	CPUAFFINITY dwPhysAffinity;		// Mask for the physical CPU (package)
	CPUAFFINITY dwLogAffinity;		// Mask for the logical CPU(s) on this physical CPU
	COREPROCAFF_T *pCoreProcAff;	// For each core on this physical, map logicals to core
	int physID;						// Physical ID of this CPU, may be non-sequential
									// This value comes from the hardware APIC, and is of little use to the programmer
	int nCores;						// Number of cores found on this physical processor, max index to read pCoreProcAff is nCores-1
} PHYSPROCAFF_T;

typedef struct {
	eHTResultCode htResultCode;
	int nPhysicalProcs;				// Number of physical processors available to this process
	int nLogicalProcs;				// Number of logical processors (including physical processors) available to this process
	int nCores;						// nPhysicalProcs * nCoresPerPackage
	int nCoresAvailable;			// Number of cores available to this process (usually same as nCores, for debug or advanced use)
	int nLogicalPerPackage;			
	int nCoresPerPackage;			
	int nLogicalPerCore;			// nLogicalPerPackage/nCoresPerPackage.  Does not change if HT disabled in BIOS.  Most users should use nLogicalPerCoreAvailable.
	int nLogicalPerCoreAvailable;	// Number of logicals per core booted and available 
	CPUAFFINITY dwPhysicalAffinity;	// Mask for first logical on each package
	CPUAFFINITY dwLogicalAffinity;	// Mask for remaining logicals on each package
	CPUAFFINITY CoreAffinity;		// First logical on each core of each package
	CPUAFFINITY CoreLogicalAffinity;// Remaining logicals on each core of each package
	CPUAFFINITY LLCDomainAffinity;	// Mask for the each logical that is the first of those sharing the associated LLC
	int nMaxPhysGetProcAff;			// Max number of physical processors to get Phys/Log Affinity table
	int nMaxCoreGetProcAff;			// Max number of cores to get Phys/Core Affinity table
	PHYSPROCAFF_T *pPhysProcAff;	// Association between Physical and Logical Processors
									// Allocate nMaxPhysGetProcAff*sizeof(PHYSPROCAFF_T) before calling WinCPUID_Init
	int LLC_Level;					// Level of last-level cache
	int nLastLevelCachesAvailable;	// Number of distinct last-level caches detected
} HTINFO_T;

typedef struct CPUINFO_s {
	int bIsInitialized;				// Check this to see if structure was successfully initialized
	int bCPUID_Supported;
	union {
		int bCloneFlag;					// 0 -> Intel CPU, non-zero -> non-Intel CPU
		int bNonIntelFlag;
	};
	int bMMX_Supported;
	int bSSE_Supported;
	int bSSE2_Supported;
	union {
		int bPNI_Supported;
		int bSSE3_Supported;
	};
	union {
		int bMNI_Supported;
		int bSSSE3_Supported;
	};
	int bSSE4_1_Supported; // SSE4.1
	int bSSE4_2_Supported;
	int bEM64T_Supported;
	union {
		int bNX_Supported;
		int bXD_Supported;
	};
	int bSSEMMXExt_Supported;		// Some AMD CPUs support SSE MMX (integer) extensions only
	int b3DNow_Supported;
	int bExt3DNow_Supported;
	union {
		int bHTorMC_Supported;			// This flag is set for either HT or Multi-Core (>1 logical per package)
		int bHT_Supported;				// HT supported and available for use, check htInfo for more details
	};
	int bDAZ_Supported;
	int bRDTSC_Supported;
	int bCMOV_Supported;
	int	EmulCode;
	LONGLONG llCPUFreq;
	CPU_VENDORS eCPUVendor;
	CPUCOREINFO_T coreInfo;
	HTINFO_T htInfo;
	CACHEINFO_T *pCacheInfo;
	char *pProcString;
} CPUINFO_T;

#define CPUINFO_T_INIT_VAL	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, VENDOR_UNKNOWN, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { htError, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0, 0}, NULL, NULL }

typedef enum {
	NULL_CACHE = 0,
	L1I,
	L1D,
	L2,
	TC,
	L3,
	ITLB, // Note: TLBs have only the number of entries in the sizeKB field, other fields left as zero
	DTLB,
	NCACHETYPES,
	CACHEID_FORCEDWORD = 0xFFFFFFFF
} eCacheID;

#define PROCSTRING_BYTES 49

#ifndef USE_LIBCPUID
typedef struct {
	DWORD expCI_eax;
	DWORD expCI_ebx;
	DWORD expCI_ecx;
	DWORD dwReserved1;
} EXPLICITCACHEINFO_T;

typedef struct {
	DWORD CacheLevel; // L1, L2, L3
	DWORD CacheType; // 1 = data, 2 = instruction, 3 = unified
	DWORD SizeKB;
	DWORD Assoc;
	DWORD LineBytes;
	DWORD Sets;
	DWORD ThreadsPerCache;
	DWORD CoresPerPackage;	// #Log per Core = #Log per Phys / CoresPerPackage
							// CoreID = int(ProcID / #Log per Core)
	DWORD dwReserved1;
} EXPCACHE_COMMON_T;
#endif

typedef struct _cache_sharing_elem_s {
	UWORD SharedIndex;			// Or "Sharing Level".  Logicals sharing the same LLC will have different values.
	UWORD NonSharedIndex;		// Index of LLC that this logical uses.  Logicals sharing a LLC will have the same value.
	UWORD CoreIndex;			// Index of the core on which this logical resides.
	UWORD LogicalPreference;	// When not using all logicals, use in this order to use the most independent HW resources
								// Physical processors first, then first cores of each set sharing a LLC, then first logical of each core, then rest of logicals
	CPUAFFINITY CoreAffinity;	// Corresponds to CoreIndex - logicals on this core
} CACHESHARINGELEMENT_T;

typedef struct _cache_sharing_s {
	BOOL bIsValid;
	CACHESHARINGELEMENT_T *pSharingList;	// One element for each logical processor
	BOOL bSymmetricSharing;					// Machine is in symmetric configuration
	int CacheSharing;						// Number of logicals sharing LLC
	int NumCaches;							// Number of LLCs
	BOOL bSharedCaches;
	BOOL bNonSharedCaches;
	BOOL bSharedByAllOnPackage;				// Flag to indicate that all logicals on a package share this level of cache
} CACHESHARING_T;

#ifdef __cplusplus
extern "C" {
#endif

// Function Exports
WINCPUID_API int WinCPUID_Init(int msGetFrequency, CPUINFO_T *pInfo);
WINCPUID_API int WinCPUID_Init_NoGetFreq(CPUINFO_T *pInfo); // avoids need to link winmm.lib if using frequency detection
WINCPUID_API LONGLONG WinCPUID_TestCPUFrequency(DWORD msTime);
WINCPUID_API BOOL WinCPUID_PreHTCheck(); // Make sure this is true before relying on results returned by WinCPUID_Init
WINCPUID_API int WinCPUID_getCacheStr(char *p, int len, int id);
WINCPUID_API int WinCPUID_getBrandStrEx(char *p, int len, CPUCOREINFO_T *info);	// Get the processor brand
WINCPUID_API int WinCPUID_getBrandStr(char *p, int len, int id); // Included for backwards compatibility
WINCPUID_API int WinCPUID_getTypeStr(char *p, int len, int id);
WINCPUID_API int WinCPUID_getHTResultStr(char *p, int len, int id);
WINCPUID_API int WinCPUID_getCurLogPhysID(CPUINFO_T *pi, unsigned int *pdwLogID, unsigned int *pdwPhysID);
WINCPUID_API int WinCPUID_getCurLogPhysIDEx(CPUINFO_T *pi, unsigned int *pdwSMTID, unsigned int *pdwCoreID, unsigned int *pdwPkgID);
WINCPUID_API int WinCPUID_GetNumExplicitCaches();
WINCPUID_API BOOL WinCPUID_GetExplicitCache(EXPLICITCACHEINFO_T *expCache, int nExpCache); // get 1 cache info at zero-based nExpCache
WINCPUID_API BOOL WinCPUID_GetExplicitCaches(EXPLICITCACHEINFO_T *expCaches, int nExpCaches); // calls WinCPUID_GetExplicitCache nExpCaches times for 0 to nExpCaches-1
WINCPUID_API BOOL WinCPUID_GetExpCacheCommonInfo(EXPCACHE_COMMON_T *pECCmn, EXPLICITCACHEINFO_T *pEC);
WINCPUID_API BOOL WinCPUID_IsRunningExt64(); // Returns true if 64-bit extension instructions can be executed
WINCPUID_API BOOL WinCPUID_GetAddrBits(CPU_ADDR_BITS_T *pAddrBits, const CPUINFO_T *pi);

/*	********** Start Beta WinCPUID APIs *********************
	*  These APIs are in development.  Use with caution.	*
	*   - Backward compatibility may not be maintained		*
	*	  by future releases.								*
	*	- These functions may not behave as expected.		*/
// Note: The pi->pCacheInfo array must be zero-initialized before calling
WINCPUID_API BOOL WinCPUID_GetL2CacheSharing(CACHESHARING_T *pSharing, int numInSharingArr, const int numCurThreads, const CPUINFO_T *pi); // Really Last-Level-Cache sharing
WINCPUID_API BOOL WinCPUID_GetLogicalPreferredUsageOrder(CACHESHARING_T *pSharing, const CPUINFO_T *pi, int numInSharingArr); // Done by WinCPUID_GetL2CacheSharing, no need to call both.  Use if you only want the preferred order info.

#ifndef USE_LIBCPUID
typedef struct _cpuid_heir_s
{
	BYTE SMT_ID;
	BYTE CORE_ID;
	BYTE LLC_DOMAIN_ID;
	BYTE PACKAGE_ID;
} LOGICAL_HEIR_T;
#endif

WINCPUID_API BOOL WinCPUID_getCurLogPhysIDEx2(const CPUINFO_T *pi, LOGICAL_HEIR_T *pLogHeirOut);
/*	********** End Beta WinCPUID APIs ***********************/

#ifndef WINCPUID_STATIC
// If string passed in matches string stored in DLL, you are using the correct header/DLL pair
WINCPUID_API BOOL WinCPUID_CheckVer(char *ver);
#endif

// Utility macros in Microsoft and Intel compiler
// To set FTZ mode
// _MM_SET_FLUSH_ZERO_MODE(x), where x = _MM_FLUSH_ZERO_ON or _MM_FLUSH_ZERO_OFF
// To get FTZ mode, _MM_GET_FLUSH_ZERO_MODE()
// For DAZ, use _mm_setcsr(x) and _mm_getcsr(x) to set/check bit 6.  Must check bDAZ_Supported first.
#define _WINCPUID_MXCSR_DAZ_BIT		0x20
#define _WINCPUID_MXCSR_DAZ_BIT_ON	0x20
#define _WINCPUID_MXCSR_DAZ_BIT_OFF	0x0
#define _WINCPUID_SET_DAZ_MODE(mode)                               \
            _mm_setcsr((_mm_getcsr() & ~_WINCPUID_MXCSR_DAZ_BIT) | (mode))
#define _WINCPUID_GET_DAZ_MODE(mode)                               \
            (_mm_getcsr() & _WINCPUID_MXCSR_DAZ_BIT)

#ifdef __cplusplus
}
#endif

#pragma pack(pop, WINCPUID_H)
#endif // WINCPUID_H_INCLUDE

