#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <cctype>

#include "CInterfaces.h"

CInterfaces* pInterfaces;

CInterfaces::CInterfaces() {}

void CInterfaces::Setup() {
	this->pCVar = (CCVar*)GetInterface("vstdlib.dll", "VEngineCvar");
	this->pClient = (CHLClient*)GetInterface("client.dll", "VClient");
	this->pEngine = (CEngineClient*)GetInterface("engine.dll", "VEngineClient");
	this->pClientEntityList = (ClientEntityList*)GetInterface("client.dll", "VClientEntityList");
}

void* CInterfaces::GetInterface(const char* pModule, const char* Interface) {
	CreateInterfaceFn CreateInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(pModule), "CreateInterface");

	uintptr_t jmpAddress = (uintptr_t)CreateInterface + 0x4;
	auto displacement = *(int*)(jmpAddress + 0x1);
	uintptr_t jmpLocation = (jmpAddress + 0x5) + displacement;
	uintptr_t linkedListLocation = jmpLocation + 0x6;

	InterfaceReg* linkedList = **(InterfaceReg***)linkedListLocation;

	for (InterfaceReg* current = linkedList; current; current = current->NextInterface) {
		std::string name = current->InterfaceName;
		name.erase(
			std::remove_if(name.begin(), name.end(),
				[](char c) {
					return std::isdigit((uint8_t)c);
				}
		), name.end());

		if (Interface == name) {
			std::cout << "Interface found (" << current->InterfaceName << ") = 0x" << std::hex << (void*)current->Interface() << std::endl;
			return (void*)current->Interface();
		}

	}

	return nullptr;
}