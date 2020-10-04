#ifndef _HACK_INTERFACES
#define _HACK_INTERFACES

#pragma once

#include "../SDK/tier1/convar.h"

typedef void* (*InstantiateInterface)();
typedef void* (*CreateInterfaceFn)(const char* pModule, int pReturn);

class InterfaceReg {
public:
	InstantiateInterface Interface;
	char* InterfaceName;
	InterfaceReg* NextInterface;
};

class CInterfaces {
private:
	CCVar* pCVar;
public:
	CInterfaces();

	CCVar* GetCCVar();
private:
	void Setup();
	void* GetInterface(const char* pModule, const char* pInterface);
};

#endif