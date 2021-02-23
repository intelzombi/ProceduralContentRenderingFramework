//
// Copyright (C) 2002 2006, Intel Corp.  All rights reserved.
// Notice:  No warranties are made regarding the functionality or suitability of this code.
//			By using this code, you agree that Intel is not liable for any loss or damage
//			that may result.

// WinCPUID API C++ Interface
// This C++ interface provides a more convenient abstraction of the WinCPUID C API

#pragma once
#include "WinCPUID.h"

class WINCPUID_API CWinCPUID:
	public CPUINFO_T
{
public:
	CWinCPUID();					// All fields initialized except frequency
	CWinCPUID(int msGetFrequency);	// All fields initialized, detect frequency for msGetFrequency milliseconds
	~CWinCPUID();
	void Initialize();				// Explicitly initialize object (useful when casting from C), no frequency
	void Initialize(int msGetFrequency); // Explicitly initialize object (useful when casting from C)
	void Deinit();					// Explicitly deinitialize, freeing memory allocated by constructor or Initialize

	static LONGLONG TestCPUFrequency(int msGetFrequency) {  // Explicitly test frequency for msGetFrequency milliseconds
		return WinCPUID_TestCPUFrequency(msGetFrequency);
	}
	int GetHTResultStr(const char *cstr, int maxlen) {
		return WinCPUID_getHTResultStr(const_cast<char*>(cstr), maxlen, htInfo.htResultCode);
	}
	int GetCurProcIDs(unsigned int &SMT_ID, unsigned int &CORE_ID, unsigned int &PACKAGE_ID)
	{
		return WinCPUID_getCurLogPhysIDEx(this, &SMT_ID, &CORE_ID, &PACKAGE_ID);
	}

	// Common physical/core/logical info
	int GetNumLogical() const {
		return htInfo.nLogicalProcs;
	}
	int GetNumCores() const {
		return htInfo.nCoresAvailable;
	}
	int GetNumPhysical() const {
		return htInfo.nPhysicalProcs;
	}
	int GetLogicalPerCore() const { 
		return htInfo.nLogicalPerCoreAvailable;
	}
	int GetLogicalPerPackage() const {
		return htInfo.nLogicalPerPackage;
	}
	int GetCoresPerPackage() const {
		return htInfo.nCoresPerPackage;
	}

	// Note:  The CPUINFO_T.bHT_Supported flag is set for both HT-enabled processors and for multi-core processors
	// Does this processor have HT?
	int GetHTCapability() const {
		return (htInfo.nLogicalPerCore > 1);
	}
	// Is HT currently enabled on this processor?
	int GetHTAvailable() const {
		return (htInfo.nLogicalPerCoreAvailable > 1);
	}

	const CACHEINFO_T &GetL2CacheInfo()
	{
		return pCacheInfo[L2];
	}

private:
	void InitBase();
};

// Actually Last-Level-Cache (LLC) Sharing
class WINCPUID_API CL2Sharing:
	public CACHESHARING_T
{
public:
	CL2Sharing(const CWinCPUID *pCPUInfo, int numCurThreads = 0);
	~CL2Sharing();
	BOOL IsValid() const { return bIsValid; }
	CPUAFFINITY GetL2Affinity(const CWinCPUID *pCPUInfo, DWORD PackageID, DWORD CoreID);
};

#ifndef WINCPUID_STATIC
namespace WinCPUID {
	extern WINCPUID_API CWinCPUID S_cpuInfo;
}


#endif
