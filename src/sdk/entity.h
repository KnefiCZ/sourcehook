#ifndef SDK_ENTITY_H
#define SDK_ENTITY_H
#pragma once

#include "sdk/interfaceinfo.h"
#include "sdk/common.h"
#include "sdk/util/clientclass.h"
#include "sdk/util/studio.h"
#include "netvar.h"

class IClientEntity {};

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

class IClientNetworkable
{
public:
	ClientClass* GetClientClass()
	{
		return CallVFunction<ClientClass*(__thiscall*)(void*)>(this, ICLIENTNETWORKABLE_INDEX_GETCLIENTCLASS)(this);
	}

	bool IsDormant()
	{
		return CallVFunction<bool(__thiscall*)(void*)>(this, ICLIENTNETWORKABLE_INDEX_ISDORMANT)(this);
	}

	int EntIndex()
	{
		return CallVFunction<int(__thiscall*)(void*)>(this, ICLIENTNETWORKABLE_INDEX_ENTINDEX)(this);
	}
};

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

class IClientRenderable
{
public:
	Vector& GetRenderOrigin()
	{
		return CallVFunction<Vector&(__thiscall*)(void*)>(this, ICLIENTRENDERABLE_INDEX_GETRENDERORIGIN)(this);
	}

	QAngle& GetRenderAngles()
	{
		return CallVFunction<QAngle&(__thiscall*)(void*)>(this, ICLIENTRENDERABLE_INDEX_GETRENDERANGLES)(this);
	}

	model_t* GetModel()
	{
		return CallVFunction<model_t*(__thiscall*)(void*)>(this, ICLIENTRENDERABLE_INDEX_GETMODEL)(this);
	}

	bool SetupBones(matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		return CallVFunction<bool(__thiscall*)(void*, matrix3x4_t*, int, int, float)>(this, ICLIENTRENDERABLE_INDEX_SETUPBONES)(this, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	}

	void GetRenderBounds(Vector& mins, Vector& maxs)
	{
		return CallVFunction<void(__thiscall*)(void*, Vector&, Vector&)>(this, ICLIENTRENDERABLE_INDEX_GETRENDERBOUNDS)(this, mins, maxs);
	}
};

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

#define NETVAR(type, name, table, netvar) \
    type& name##() \
	{ \
        static int offset_##name = g_pNetvar->GetOffset(table, netvar); \
        return *(type*)((DWORD)this + offset_##name); \
    }

#define PNETVAR(type, name, table, netvar) \
    type* name##() \
	{ \
        static int offset_##name = g_pNetvar->GetOffset(table, netvar); \
        return (type*)((DWORD)this + offset_##name); \
    }

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

class CBaseHandle
{
public:
	IClientEntity* Get();
public:
	unsigned long m_Index;
};

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

class CBaseEntity : IClientEntity
{
public:
	IClientRenderable* GetRenderable() const
	{
		return (IClientRenderable*)(this + 0x4);
	}

	IClientNetworkable* GetNetworkable() const
	{
		return (IClientNetworkable*)(this + 0x8);
	}
public:
	Vector& GetAbsOrigin()
	{
		return CallVFunction<Vector&(__thiscall*)(void*)>(this, CBASEENTITY_INDEX_GETABSORIGIN)(this);
	}

	QAngle& GetAbsAngles()
	{
		return CallVFunction<QAngle&(__thiscall*)(void*)>(this, CBASEENTITY_INDEX_GETABSANGLES)(this);
	}
public:

public:
	NETVAR(float, m_flSimulationTime, "DT_BaseEntity", "m_flSimulationTime");
	NETVAR(int, m_nTickBase, "DT_LocalPlayerExclusive", "m_nTickBase");
	NETVAR(int, m_iHealth, "DT_BaseEntity", "m_iHealth");
	NETVAR(int, m_iTeamNum, "DT_BaseEntity", "m_iTeamNum");
	NETVAR(int, m_nModelIndex, "DT_BaseEntity", "m_nModelIndex");
};

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

class CBasePlayer : public CBaseEntity
{
public:
	float ServerTime();
	bool GetHitboxPos(int hitbox, Vector& pos);
public:
	NETVAR(CBaseHandle, m_hActiveWeapon, "DT_BaseCombatCharacter", "m_hActiveWeapon");
	NETVAR(CBaseHandle, m_hObserverTarget, "DT_BasePlayer", "m_hObserverTarget");
};

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

#endif