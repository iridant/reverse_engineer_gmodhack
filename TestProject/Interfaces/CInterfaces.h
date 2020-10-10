#ifndef _HACK_INTERFACES
#define _HACK_INTERFACES

#pragma once

#include "../SDK/Client/CHLClient.h"
#include "../NetVars/CNetVarManager.h"
#include "../SDK/tier1/convar.h"
#include "../SDK/Client/ClientEntityList.h"
#include "../SDK/Entities/BaseEntity.h"
#include "../SDK/Engine.h"


typedef void* (*InstantiateInterface)();
typedef void* (*CreateInterfaceFn)(const char* pModule, int pReturn);

class InterfaceReg {
public:
	InstantiateInterface Interface;
	char* InterfaceName;
	InterfaceReg* NextInterface;
};

class CInterfaces {
public:
	CCVar* pCVar;	
	CHLClient* pClient;
	ClientEntityList* pClientEntityList;
	CEngineClient* pEngine;
public:
	CInterfaces();
	void Setup();
private:
	void* GetInterface(const char* pModule, const char* pInterface);
}; extern CInterfaces* pInterfaces;

#endif