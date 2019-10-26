#ifndef SDK_ENTITY_H
#define SDK_ENTITY_H
#pragma once

#include "sdk/interfaceinfo.h"
#include "sdk/common.h"
#include "sdk/util/clientclass.h"
#include "sdk/util/studio.h"
#include "netvar.h"

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
};

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

class IClientEntity {};

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
	NETVAR(QAngle, m_angRotation, "DT_BaseEntity", "m_angRotation");
	NETVAR(float, m_flSimulationTime, "DT_BaseEntity", "m_flSimulationTime");
	NETVAR(int, m_iHealth, "DT_BaseEntity", "m_iHealth");
	NETVAR(int, m_iTeamNum, "DT_BaseEntity", "m_iTeamNum");
	NETVAR(int, m_nModelIndex, "DT_BaseEntity", "m_nModelIndex");
	NETVAR(Vector, m_vecOrigin, "DT_BaseEntity", "m_vecOrigin");
	NETVAR(Vector, m_vecVelocity, "DT_BaseEntity", "m_vecVelocity[0]");
};

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

#endif