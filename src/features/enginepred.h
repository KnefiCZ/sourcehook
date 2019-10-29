#ifndef FEATURE_ENGINE_PREDICTION_H
#define FEATURE_ENGINE_PREDICTION_H
#pragma once

#include "sdk/util/usercmd.h"
#include "sdk/interfaces/igamemovement.h"

class CEnginePrediction
{
	float m_flOldCurtime;
	float m_flOldFrametime;
	CMoveData m_MoveData;
public:
	int* m_pPredictionRandomSeed;

	void StartPrediction(CUserCmd* pCmd);
	void EndPrediction(CUserCmd* pCmd);
};

#define ENGINEPREDICTION_RANDOMSEED_PATTERN "C7 05 ?? ?? ?? ?? ?? ?? ?? ?? 5D C3 8B 40 30"
inline CEnginePrediction* g_pEnginePred;

#endif