#ifndef _HACK_NETVARMANAGER_
#define _HACK_NETVARMANAGER_

class RecvTable;
class RecvProp;

#include <vector>

#pragma once

struct nVar {
	const char* tName;
	const char* pName;
};

struct nvInfo {
	const char* tName;
	const char* pName;
	int m_Offset = 0;
};

class CNetVarManager {
private:
	std::vector<nvInfo> netVars;
	std::vector<nVar> m_vars;
	ClientClass* classes;
	std::vector<RecvTable*> m_tables;

	int sGetProp(RecvTable* tableName, const char* propName, RecvProp** prop);
	int sGetProp(const char* tableName, const char* propName);

	RecvTable* GetTable(const char* tableName);
	nvInfo GetOffset(const char* tableName, const char* propName);
	//DWORD HookProp(const char* tableName, const char* propName, RecvVarProxyFn function);

public:
	CNetVarManager(CHLClient* c);
	void Setup();
	int GetNVOffset(const char* tName, const char* pName);
}; extern CNetVarManager* pNetVarManager;

#endif