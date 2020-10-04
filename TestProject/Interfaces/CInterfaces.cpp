#include <Windows.h>
#include <stdio.h>
#include <iostream>

#include "CInterfaces.h"

CInterfaces::CInterfaces() {
	this->Setup();
}

void CInterfaces::Setup() {
	this->pCVar = (CCVar*)this->GetInterface("vstdlib.dll", "VEngineCvar");
}

void* CInterfaces::GetInterface(const char* pModule, const char* pInterface) {
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
			Interface = (void*)CreateInterface(current->InterfaceName, NULL);

			std::cout << "FOUND INTERFACE " << std::hex << Interface << std::endl;

			break;
		}

	}

	return Interface;
}

CCVar* CInterfaces::GetCCVar() {
	return this->pCVar;
}