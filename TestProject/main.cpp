#pragma once

#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <stdint.h>

#include "SDK/tier1/convar.h"
#include "Interfaces/CInterfaces.h"

FILE* stream;
CCVar* CVAR;

typedef void(__cdecl* MsgFn)(const char* str, ...);
template< class ...Args >
void msg_wrapper(const char* fmt, Args && ...args) {
	static MsgFn fn = (MsgFn)GetProcAddress(GetModuleHandleA("tier0.dll"), "Msg");
	if (!fn)
	{
		return;
	}

	fn(fmt, std::forward<Args>(args)...);
}

void* CreateInterface(const char* pModule, const char* pInterface) {
	CreateInterfaceFn CreateInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(pModule), "CreateInterface");

	uintptr_t jmpAddress = (uintptr_t)CreateInterface + 0x4;
	auto displacement = *(int*)(jmpAddress + 0x1);
	uintptr_t jmpLocation = (jmpAddress + 0x5) + displacement;
	uintptr_t linkedListLocation = jmpLocation + 0x6;

	std::cout << std::hex << jmpAddress << std::endl;
	std::cout << std::hex << displacement << std::endl;
	std::cout << std::hex << jmpLocation << std::endl;
	
	InterfaceReg* linkedList = **(InterfaceReg***)linkedListLocation;

	void* Interface = nullptr;
	for (InterfaceReg* current = linkedList; current; current = current->NextInterface) {
		if (strstr(current->InterfaceName, pInterface) && (strlen(current->InterfaceName) - strlen(pInterface)) < 5) {
			Interface = (int*)CreateInterface(current->InterfaceName, NULL);

			std::cout << "FOUND INTERFACE" << std::endl;

			std::cout << std::hex << Interface;

			break;
		}
			
	}


	return Interface;
}

void main() {
	AllocConsole();
	SetConsoleTitleA("Title");

	freopen_s(&stream, "CONIN$", "r", stdin);
	freopen_s(&stream, "CONOUT$", "w", stdout);
	freopen_s(&stream, "CONOUT$", "w", stderr);

	std::cout << "Output" << std::endl;

	msg_wrapper("Hi");
	CVAR = (CCVar*)CreateInterface("vstdlib.dll", "VEngineCvar");

	IConVar* cvar;

	cvar = (IConVar*)CVAR->FindVar("sv_allowcslua");

	std::cout << std::hex << cvar << std::endl;
	
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID lpreserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)main, nullptr, 0, nullptr);
	}

	return true;
}