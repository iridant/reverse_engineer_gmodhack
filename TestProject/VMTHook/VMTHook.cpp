#include <Windows.h>
#include <iostream>
#include <bcrypt.h>
#include <winnt.h>

#include "VMTHook.h"

VMTHook::VMTHook(void* pInstance) {
	pVTInstance = (void***)pInstance;
	pOriginalVT = *pVTInstance;

	iSize = 0;
	while (IsSafePointer(pOriginalVT[iSize++]));

	std::cout << iSize << std::endl;

	int size = sizeof(int) * iSize;

	pNewVT = (void**)new uintptr_t[size + 1];

	memcpy(pNewVT, pOriginalVT - 1, size + 1);

	*pVTInstance = pNewVT + 1;
}

void* VMTHook::Hook(int index, void* pHook) {
	if (index > iSize)
		return nullptr;

	if (!pOriginalVT[index])
		return nullptr;

	if (pHook == pOriginalVT[index])
		return nullptr;

	pNewVT[index + 1] = pHook;

	return pOriginalVT[index];
}

void* VMTHook::Unhook(int index) {
	if (index > iSize)
		return nullptr;

	if (!pOriginalVT[index])
		return nullptr;

	pNewVT[index] = pOriginalVT[index];

	return pOriginalVT[index];
}

bool VMTHook::IsSafePointer(void* addr) {
	/*MEMORY_BASIC_INFORMATION mbi;

	constexpr unsigned long OFF_LIMITS_MASK = (PAGE_NOACCESS | PAGE_GUARD);
	constexpr unsigned long READ_MASK = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
	*/
	if (!addr)
		return false;

	/*VirtualQuery(addr, &mbi, sizeof(mbi));

	if ((mbi.Protect & OFF_LIMITS_MASK))
		return false;

	if (!(mbi.Protect & READ_MASK))
		return false;*/

	return true;
}