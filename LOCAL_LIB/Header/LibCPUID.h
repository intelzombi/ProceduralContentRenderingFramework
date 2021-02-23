//
// Copyright (C) 2009, Intel Corp.  All rights reserved.
// Notice:  No warranties are made regarding the functionality or suitability of this code.
//			By using this code, you agree that Intel is not liable for any loss or damage
//			that may result.
//
// Eric L Palmer (eric.l.palmer@intel.com)
// This code does not attempt to provide info for processors that do not support the cpuid instruction (i.e. prior to Intel(r) Pentium(tm))
//
// Note: This code implements some of the functionality from the article/sample code at
//      http://software.intel.com/en-us/articles/intel-64-architecture-processor-topology-enumeration/.
//
// LibCPUID is intended to be a cross-platform (OS independent) library for CPU feature 
// detection and topology enumeration

#ifndef _WIN32
typedef unsigned int DWORD;
typedef unsigned int BOOL;
#else
typedef unsigned long DWORD;
#endif
typedef unsigned char BYTE;

//typedef __int64 LONGLONG;
//typedef unsigned short UWORD;
//typedef DWORD_PTR CPUAFFINITY;

//typedef size_t CPUAFFINITY; // Assume morphing type

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

typedef struct {
	int nLogicalProcs;				// Number of logical processors (including physical processors) available to this process
	int nLogicalPerPackage;			
	int nCoresPerPackage;			
	int nLogicalPerCore;			// nLogicalPerPackage/nCoresPerPackage.  Does not change if HT disabled in BIOS.  Most users should use nLogicalPerCoreAvailable.
	int LLC_Level;					// Level of last-level cache
} BASIC_MC_HT_INFO_T;

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

typedef struct _cpuid_heir_s
{
	BYTE SMT_ID;
	BYTE CORE_ID;
	BYTE LLC_DOMAIN_ID;
	BYTE PACKAGE_ID;
} LOGICAL_HEIR_T;

#define LIBCPUID_MIN_PROCSTR_LEN 49 // 48+NULL

// ********************   Exported functions   ************************
#ifdef __cplusplus
extern "C" {
#endif

int GetCPUIDInstSupport();
CPU_VENDORS GetCPUVendor();
int GetCoreInfo(CPUCOREINFO_T *pCI);
int GetSSE(const CPUCOREINFO_T *pCI);
int GetSSE2(const CPUCOREINFO_T *pCI);
int GetSSE3(const CPUCOREINFO_T *pCI);
int GetSSSE3(const CPUCOREINFO_T *pCI);
int GetSSE4_1(const CPUCOREINFO_T *pCI);
int GetSSE4_2(const CPUCOREINFO_T *pCI);
int GetDAZCapable(const CPUCOREINFO_T *pCI);
int GetProcStr(char *outStr);

DWORD GetOSNumProcs();
int GetBasicHT_MC_Info(const CPUCOREINFO_T *pCI, BASIC_MC_HT_INFO_T *pBasicHTMC);
int GetCurProcHeir(const CPUCOREINFO_T *pCI, const BASIC_MC_HT_INFO_T *pBasicHTMC, LOGICAL_HEIR_T *pLogHeirOut);
DWORD GetMaxNumLPSharingCache(int level, const CPUCOREINFO_T *pCI, const BASIC_MC_HT_INFO_T *pBasicHTMC);

DWORD GetIndexForCacheLevelAndType(int level, int type);
int GetExplicitCache(EXPLICITCACHEINFO_T *expCache, int nExpCache);
int GetExpCacheCommonInfo(EXPCACHE_COMMON_T *pECCmn, EXPLICITCACHEINFO_T *pEC);

// Utility functions
int GetAPIC_ID();
BOOL HasCPUID_Leaf_B();
int LeafBHWMTConsts(int *pLogPerCore);

#ifdef __cplusplus
}
#endif
