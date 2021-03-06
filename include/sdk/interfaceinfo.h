#ifndef SDK_INTERFACE_INFO
#define SDK_INTERFACE_INFO
#pragma once

/*--------------------------------IVEngineClient--------------------------------*/
#define IVENGINECLIENT_INDEX_GETSCREENSIZE 5
#define IVENGINECLIENT_INDEX_GETPLAYERINFO 8
#define IVENGINECLIENT_INDEX_GETLOCALPLAYER 12
#define IVENGINECLIENT_INDEX_GETMAXCLIENTS 21
#define IVENGINECLIENT_INDEX_ISINGAME 26
#define IVENGINECLIENT_INDEX_ISCONNECTED 27
#define IVENGINECLIENT_INDEX_ISDRAWINGLOADINGIMAGE 28
#define IVENGINECLIENT_INDEX_WORLDTOSCREENMATRIX 36
#define IVENGINECLIENT_INDEX_GETNETCHANNELINFO 72
#define IVENGINECLIENT_INDEX_ISPAUSED 84 // used for clientstate

#define IVENGINECLIENT_INTERFACE "VEngineClient013"
/*------------------------------------------------------------------------------*/



/*---------------------------------IEngineTrace---------------------------------*/
#define IENGINETRACE_INDEX_TRACERAY 4

#define IENGINETRACE_INTERFACE "EngineTraceClient003"
/*------------------------------------------------------------------------------*/



/*--------------------------------IBaseClientDLL--------------------------------*/
#define IBASECLIENTDLL_INDEX_GETALLCLASSES 8
#define IBASECLIENTDLL_INDEX_CREATEMOVE 21
#define IBASECLIENTDLL_INDEX_FRAMESTAGENOTIFY 35

#define IBASECLIENTDLL_INTERFACE "VClient017"
/*------------------------------------------------------------------------------*/



/*------------------------------IClientEntityList-------------------------------*/
#define ICLIENTENTITYLIST_INDEX_GETCLIENTENTITY 3
#define ICLIENTENTITYLIST_INDEX_GETHIGHESTENTITYINDEX 6

#define ICLIENTENTITYLIST_INTERFACE "VClientEntityList003"
/*------------------------------------------------------------------------------*/



/*--------------------------------IVModelInfo-----------------------------------*/
#define IVMODELINFO_GETMODEL 1
#define IVMODELINFO_GETMODELINDEX 2
#define IVMODELINFO_GETMODELNAME 3
#define IVMODELINFO_GETSTUDIOMODEL 28

#define IVMODELINFOCLIENT_INTERFACE "VModelInfoClient006"
/*------------------------------------------------------------------------------*/



/*--------------------------------IClientMode-----------------------------------*/
#define ICLIENTMODE_INDEX_CREATEMOVE 21
#define ICLIENTMODE_INDEX_LEVELINIT 22
#define ICLIENTMODE_INDEX_LEVELSHUTDOWN 23
/*------------------------------------------------------------------------------*/



/*--------------------------------IPrediction-----------------------------------*/
#define IPREDICTION_INDEX_UPDATE 3
#define IPREDICTION_INDEX_SETUPMOVE 18
#define IPREDICTION_INDEX_FINISHMOVE 19

#define IPREDICTION_INTERFACE "VClientPrediction001"
/*------------------------------------------------------------------------------*/



/*-------------------------------IGameMovement-----------------------------------*/
#define IGAMEMOVEMENT_INDEX_PROCESSMOVEMENT 1
#define IGAMEMOVEMENT_INDEX_STARTTRACKPREDICTIONERRORS 2
#define IGAMEMOVEMENT_INDEX_FINISHTRACKPREDICTIONERRORS 3

#define IGAMEMOVEMENT_INTERFACE "GameMovement001"
/*------------------------------------------------------------------------------*/



/*------------------------------IClientRenderable-------------------------------*/
#define ICLIENTRENDERABLE_INDEX_GETRENDERORIGIN 1
#define ICLIENTRENDERABLE_INDEX_GETRENDERANGLES 2
#define ICLIENTRENDERABLE_INDEX_GETMODEL 9
#define ICLIENTRENDERABLE_INDEX_SETUPBONES 16
#define ICLIENTRENDERABLE_INDEX_GETRENDERBOUNDS 20
/*------------------------------------------------------------------------------*/



/*------------------------------IClientNetworkable------------------------------*/
#define ICLIENTNETWORKABLE_INDEX_GETCLIENTCLASS 2
#define ICLIENTNETWORKABLE_INDEX_ISDORMANT 8
#define ICLIENTNETWORKABLE_INDEX_ENTINDEX 9
/*------------------------------------------------------------------------------*/



/*--------------------------------C_BaseEntity----------------------------------*/
#define CBASEENTITY_INDEX_GETABSORIGIN 9
#define CBASEENTITY_INDEX_GETABSANGLES 10
#define CBASEENTITY_INDEX_USESLUA 170
/*------------------------------------------------------------------------------*/



/*-----------------------------C_BaseCombatWeapon-------------------------------*/
#define CBASECOMBATWEAPON_INDEX_GETBULLETSPREAD 325
/*------------------------------------------------------------------------------*/



/*------------------------------IDirect3DDevice9--------------------------------*/
#define IDIRECT3DDEVICE9_INDEX_ENDSCENE 42
#define IDIRECT3DDEVICE9_INDEX_RESET 16
/*------------------------------------------------------------------------------*/


/*
Dear archi...
FUCKING REMEMBER TO DIVIDE THE ADDRESS BY 4 TO GET THE INDEX!!!!
*/

#endif