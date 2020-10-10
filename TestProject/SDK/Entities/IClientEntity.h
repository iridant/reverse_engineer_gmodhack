#ifndef _SDK_ICLIENTENTITY_
#define _SDK_ICLIENTENTITY

#pragma once

class CBaseHandle;
class IClientNetworkable;
class IClientRenderable;
class ICollideable;
class IClientEntity;
class IClientThinkable;
class BaseEntity;
class Vector;
class QAngle;

struct SpatializationInfo_t;

class IHandleEntity{
public:
	virtual ~IHandleEntity() = 0;
	virtual void SetRefEHandle(const CBaseHandle& handle) = 0;
	virtual const CBaseHandle& GetRefEHandle() const = 0;
};

class IClientUnknown : public IHandleEntity{
public:
	virtual ICollideable* GetCollideable() = 0;
	virtual IClientNetworkable* GetClientNetworkable() = 0;
	virtual IClientRenderable* GetClientRenderable() = 0;
	virtual IClientEntity* GetIClientEntity() = 0;
	virtual BaseEntity* GetBaseEntity() = 0;
	virtual IClientThinkable* GetClientThinkable() = 0;
};


class IClientEntity : public IClientUnknown
{
public:
	virtual Vector& GetRenderOrigin(void) = 0;
	virtual QAngle& GetRenderAngles(void) = 0;
	virtual void* GetMouth() = 0;
	virtual bool	GetSoundSpatialization(SpatializationInfo_t& info) = 0;
};

#endif