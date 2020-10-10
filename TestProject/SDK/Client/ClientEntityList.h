#ifndef _SDK_CLIENTENTITYLIST_
#define _SDK_CLIENTENTITYLIST_

#pragma once

class IClientNetworkable;
class BaseEntity;
class CBaseHandle;
class IClientEntity;

class ClientEntityList {
public:
	virtual ~ClientEntityList();
	virtual IClientNetworkable* GetClientNetworkable(int entnum) = 0;
	virtual IClientNetworkable* GetClientNetworkableFromHandle(unsigned long long hEnt) = 0;
	virtual IClientEntity* GetClientEntity(int entnum) = 0;
	virtual IClientEntity* GetClientEntityFromHandle(unsigned long long hEnt) = 0;
	virtual int					NumberOfEntities(bool bIncludeNonNetworkable) = 0;
	virtual int					GetHighestEntityIndex(void) = 0;
	virtual void				SetMaxEntities(int maxents) = 0;
	virtual int					GetMaxEntities() = 0;
};

#endif