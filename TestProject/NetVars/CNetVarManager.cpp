#include <vector>
#include <Windows.h>
#include <iostream>

#include "../SDK/Client/CHLClient.h"
#include "../SDK/ClientClass.h"
#include "CNetVarManager.h"

CNetVarManager* pNetVarManager;

CNetVarManager::CNetVarManager(CHLClient* pClientClass) {
	classes = pClientClass->GetAllClasses();

	while (classes) {
		RecvTable* recvTable = classes->m_pRecvTable;
		m_tables.push_back(recvTable);
		classes = classes->m_pNext;
	}
}

void CNetVarManager::Setup() {

	for (RecvTable* table : m_tables) {
		if (!table)
			continue;

		for (int i = 0; i < table->m_nProps; i++) {
			RecvProp* pProp = &table->m_pProps[i];
			if (!pProp) continue;

			nVar nv;

			nv.pName = pProp->m_pVarName;
			nv.tName = table->m_pNetTableName;

			m_vars.push_back(nv);
		}
	}

	for (nVar nv : m_vars) {
		netVars.push_back(this->GetOffset(nv.tName, nv.pName));
	}

	netVars.push_back(this->GetOffset("DT_GMOD_Player", "m_nTickBase"));
}

int CNetVarManager::GetNVOffset(const char* tName, const char* pName) {
	auto ret = std::find_if(netVars.begin(), netVars.end(),
		[tName, pName](const nvInfo d) {
			return strstr(d.pName, pName) != nullptr && strstr(d.tName, tName) != nullptr;
		});

	if (ret != netVars.end()) {
		//std::cout << "FF " << (*ret).m_Offset << std::endl;
		return (*ret).m_Offset;
	}

	return 0;
}

nvInfo CNetVarManager::GetOffset(const char* tableName, const char* propName) {
	nvInfo ret;

	ret.m_Offset = this->sGetProp(tableName, propName);
	ret.pName = propName;
	ret.tName = tableName;

	if (!ret.m_Offset)
		return ret;

	//std::cout << "NV Offset Found (" << tableName << ") - " << propName << " = {0x" << std::hex << (void*)ret.m_Offset << "}" << std::endl;

	return ret;
}

int CNetVarManager::sGetProp(RecvTable* recvTable, const char* propName, RecvProp** prop = 0) {
	int extraOffset = 0;

	for (int i = 0; i < recvTable->m_nProps; ++i) {
		RecvProp* recvProp = &recvTable->m_pProps[i];
		RecvTable* child = recvProp->m_pDataTable;

		if (child && (child->m_nProps > 0)) {
			int tmp = this->sGetProp(child, propName, prop);

			if (tmp)
				extraOffset += (recvProp->m_Offset + tmp);
		}

		if (_stricmp(recvProp->m_pVarName, propName))
			continue;

		if (prop)
			*prop = recvProp;

		return recvProp->m_Offset + extraOffset;
	}
	return extraOffset;
}

int CNetVarManager::sGetProp(const char* tableName, const char* propName) {
	RecvTable* recvTable = GetTable(tableName);

	if (!recvTable)
		return 0;

	int ret = this->sGetProp(recvTable, propName);

	if (!ret)
		return 0;

	return ret;
}

RecvTable* CNetVarManager::GetTable(const char* tableName) {
	if (m_tables.empty())
		return 0;

	for (RecvTable* table : m_tables) {
		if (!table)
			continue;

		if (_stricmp(table->m_pNetTableName, tableName) == 0) {
			return table;
		}
	}
	return 0;
}