#ifndef SDK_ENTITY_H
#define SDK_ENTITY_H
#pragma once

#include "sdk/interfaceinfo.h"
#include "sdk/common.h"
#include "sdk/util/clientclass.h"
#include "sdk/util/studio.h"
#include "sdk/util/flags.h"
#include "netvar.h"

class IClientEntity {};

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

#define	FL_ONGROUND (1<<0)
#define FL_DUCKING (1<<1)
#define	FL_WATERJUMP (1<<2)
#define FL_ONTRAIN (1<<3)
#define FL_INRAIN (1<<4)
#define FL_FROZEN (1<<5)
#define FL_ATCONTROLS (1<<6)
#define	FL_CLIENT (1<<7)
#define FL_FAKECLIENT (1<<8)
#define	FL_INWATER (1<<9)

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
    inline type& name##() \
	{ \
        static unsigned int offset_##name = g_pNetvar->GetOffset(table, netvar); \
        return *(type*)((DWORD)this + offset_##name); \
    }

#define PNETVAR(type, name, table, netvar) \
    inline type* name##() \
	{ \
        static unsigned int offset_##name = g_pNetvar->GetOffset(table, netvar); \
        return (type*)((DWORD)this + offset_##name); \
    }

#define ENTVAR(type, name, offset) \
    inline type& name##() \
	{ \
        return *(type*)((DWORD)this + offset); \
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
	bool IsPlayer();
public:
	NETVAR(int, m_nModelIndex, "DT_BaseEntity", "m_nModelIndex");
	NETVAR(Vector, m_vecVelocity, "DT_BaseEntity", "m_vecVelocity[0]");
};

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

class CBaseCombatWeapon : public CBaseEntity
{
public:
	Vector& GetBulletSpread()
	{
		return CallVFunction<Vector&(__thiscall*)(void*)>(this, CBASECOMBATWEAPON_INDEX_GETBULLETSPREAD)(this);
	}
public:
	
};

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

class CBasePlayer : public CBaseEntity
{
public:
	float ServerTime();
	bool GetHitboxPos(int hitbox, Vector& pos);
	bool IsAlive();
	QAngle& ViewPunchAngle();
	Vector GetEyePos();
	void InvalidateBoneCache();
public:
	NETVAR(float, m_flSimulationTime, "DT_BaseEntity", "m_flSimulationTime");
	NETVAR(int, m_nTickBase, "DT_LocalPlayerExclusive", "m_nTickBase");
	NETVAR(int, m_iHealth, "DT_BaseEntity", "m_iHealth");
	NETVAR(int, m_iTeamNum, "DT_BaseEntity", "m_iTeamNum");
	NETVAR(CBaseHandle, m_hActiveWeapon, "DT_BaseCombatCharacter", "m_hActiveWeapon");
	NETVAR(CBaseHandle, m_hObserverTarget, "DT_BasePlayer", "m_hObserverTarget");
	NETVAR(CUtlFlags<int>, m_fFlags, "DT_BasePlayer", "m_fFlags");
	NETVAR(Vector, m_vecViewOffset, "DT_LocalPlayerExclusive", "m_vecViewOffset[0]");
};

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

#endif