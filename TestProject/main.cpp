#pragma once

#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <stdint.h>
#include <cctype>

#include "VMTHook/VMTHook.h"
#include "Interfaces/CInterfaces.h"
#include "NetVars/CNetVarManager.h"

FILE* stream;
CCVar* CVAR;

typedef void(__cdecl* MsgFn)(const char* str, ...);
template< class ...Args >
void msg_wrapper(const char* fmt, Args && ...args) {
	static MsgFn fn = (MsgFn)GetProcAddress(GetModuleHandleA("tier0.dll"), "Msg");
	if (!fn)
		return;

	fn(fmt, std::forward<Args>(args)...);
}

#define getebp() stack *_bp; __asm mov _bp, ebp;

typedef int(__thiscall* GetScreenWidthFn)();
GetScreenWidthFn oGetScreenWidth = nullptr;

struct stack {
	stack* next;
	char* ret;

	template<typename T> inline 
		T arg(unsigned int i) {
		return *(T*)((void**)this + i + 2);
	}
};

int __fastcall hGetScreenWidth() {
	
	std::cout << "CALLED GETSCREENWIDTH" << std::endl;

	return oGetScreenWidth();
};

void main() {
	AllocConsole();
	SetConsoleTitleA("Title");

	freopen_s(&stream, "CONIN$", "r", stdin);
	freopen_s(&stream, "CONOUT$", "w", stdout);
	freopen_s(&stream, "CONOUT$", "w", stderr);

	std::cout << "Loaded.." << std::endl;
	msg_wrapper("Loaded..\n");

	pInterfaces = new CInterfaces();
	pInterfaces->Setup();

	pNetVarManager = new CNetVarManager(pInterfaces->pClient);
	pNetVarManager->Setup();

	//player_info_t player_info;

	//pInterfaces->pEngine->GetPlayerInfo(pInterfaces->pEngine->GetLocalPlayer(), &player_info);

	//std::cout << player_info.name << std::endl;

	//std::cout << pInterfaces->pClientEntityList->GetClientEntity(pInterfaces->pEngine->GetLocalPlayer())->GetBaseEntity()->GetHealthNV() << std::endl;

	//std::cout << pInterfaces->pClientEntityList->GetClientEntity(pInterfaces->pEngine->GetLocalPlayer())->GetBaseEntity()->m_nTickBase() << std::endl;

	VMTHook* tVMT = new VMTHook(pInterfaces->pClient);
	oGetScreenWidth = (GetScreenWidthFn)tVMT->Hook(56, hGetScreenWidth);

	pInterfaces->pClient->GetScreenWidth();

	
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID lpreserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)main, nullptr, 0, nullptr);
	}

	return true;
}