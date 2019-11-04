#include "hitboxmanager.h"
#include "sdk/math/mathlib.h"

mstudiobbox_t* CHitboxManager::FindInHitboxSet(mstudiohitboxset_t* pHitboxSet, int iGroup)
{
	mstudiobbox_t* pLastHitbox = nullptr;
	float iLastHitboxSize = 0;

	for (int i = 0; i < pHitboxSet->numhitboxes; i++)
	{
		mstudiobbox_t* pHitbox = pHitboxSet->GetHitbox(i);

		if (pHitbox->group == iGroup)
		{
			Vector vecSumBBox = pHitbox->bbmin + pHitbox->bbmax;
			float iHitboxSize = vecSumBBox.LengthSqr();

			if (iHitboxSize > iLastHitboxSize)
			{
				iLastHitboxSize = iLastHitboxSize;
				pLastHitbox = pHitbox;
			}
		}
	}

	return pLastHitbox;
}

bool CHitboxManager::ParseModel(CBasePlayer* pPlayer)
{
	model_t* pMdl = pPlayer->GetRenderable()->GetModel();
	if (!pMdl)
		return false;

	studiohdr_t* pStudio = g_pModelInfo->GetStudiomodel(pMdl);
	if (!pStudio)
		return false;

	mstudiohitboxset_t* pHitboxSet = pStudio->GetHitboxSet(pPlayer->m_nHitboxSet());

	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

	mstudiobbox_t* boxHead = FindInHitboxSet(pHitboxSet, GROUP_HEAD);

	mstudiobbox_t* boxChest = FindInHitboxSet(pHitboxSet, GROUP_CHEST);
	mstudiobbox_t* boxStomach = FindInHitboxSet(pHitboxSet, GROUP_STOMACH);

	mstudiobbox_t* boxLHand = FindInHitboxSet(pHitboxSet, GROUP_LHAND);
	mstudiobbox_t* boxRHand = FindInHitboxSet(pHitboxSet, GROUP_RHAND);

	mstudiobbox_t* boxLLeg = FindInHitboxSet(pHitboxSet, GROUP_LLEG);
	mstudiobbox_t* boxRLeg = FindInHitboxSet(pHitboxSet, GROUP_RLEG);

	/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

	HitboxList* pHitboxList = new HitboxList;

	if (!boxHead && !boxLLeg && !boxRLeg)
	{
		pHitboxList->bBadModel = true;
		pHitboxList->boxHead = pHitboxSet->GetHitbox(1);
	}
	else
	{
		pHitboxList->boxHead = boxHead;
		pHitboxList->boxStomach = boxStomach ? boxStomach : boxChest;
		pHitboxList->boxLHand = boxLHand;
		pHitboxList->boxRHand = boxRHand;
		pHitboxList->boxLLeg = boxLLeg;
		pHitboxList->boxRLeg = boxRLeg;
	}
	
	m_HitboxSets[pPlayer->m_nModelIndex()] = pHitboxList;
	return true;
}

HitboxList* CHitboxManager::GetHitboxList(CBasePlayer* pPlayer)
{
	auto it = m_HitboxSets.find(pPlayer->m_nModelIndex());

	if (it == m_HitboxSets.end())
	{
		if (ParseModel(pPlayer))
			return GetHitboxList(pPlayer);
	}

	return it->second;
}

void CHitboxManager::InvalidateHitboxLists()
{
	for (auto it : m_HitboxSets)
	{
		delete it.second;
	}

	m_HitboxSets.clear();
}