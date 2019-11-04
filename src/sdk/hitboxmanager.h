#ifndef SDK_HITBOX_MANAGER_H
#define SDK_HITBOX_MANAGER_H
#pragma once

#include <unordered_map>

#include "sdk/util/studio.h"
#include "entity.h"
#include "sdk.h"

#define GROUP_HEAD 1
#define GROUP_CHEST 2
#define GROUP_STOMACH 3
#define GROUP_LHAND 4
#define GROUP_RHAND 5
#define GROUP_LLEG 6
#define GROUP_RLEG 7

struct HitboxList
{
	bool bBadModel = false;
	mstudiobbox_t* boxHead = nullptr;
	mstudiobbox_t* boxStomach = nullptr;

	mstudiobbox_t* boxLHand = nullptr;
	mstudiobbox_t* boxRHand = nullptr;

	mstudiobbox_t* boxLLeg = nullptr;
	mstudiobbox_t* boxRLeg = nullptr;
};

class CHitboxManager
{
	std::unordered_map<int, HitboxList*> m_HitboxSets;
public:
	mstudiobbox_t* FindInHitboxSet(mstudiohitboxset_t* pHitboxSet,int iGroup);
	bool ParseModel(CBasePlayer* pPlayer);
public:
	HitboxList* GetHitboxList(CBasePlayer* pPlayer);
	void InvalidateHitboxLists();
};

#endif