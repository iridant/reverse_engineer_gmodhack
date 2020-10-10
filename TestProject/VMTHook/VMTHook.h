#ifndef _HACK_VMTHOOK_
#define _HACK_VMTHOOK_

#pragma once

class VMTHook {
public:
	int iSize;
public:
	void** pOriginalVT;
	void** pNewVT;
	void*** pVTInstance;
public:
	VMTHook(void* pInstance);
	void* Hook(int index, void* pHook);
	void* Unhook(int index);
	bool IsSafePointer(void* addr);
};

#endif