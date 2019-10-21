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

#define IVENGINECLIENT_INTERFACE "IVEngineClient0013"
/*------------------------------------------------------------------------------*/



/*--------------------------------IEngineTrace--------------------------------*/
#define IENGINETRACE_INDEX_TRACERAY 4

#define IENGINETRACE_INTERFACE "EngineTraceClient003"
/*----------------------------------------------------------------------------*/



/*--------------------------------IBaseClientDLL--------------------------------*/
#define IBASECLIENTDLL_INDEX_GETALLCLASSES 8
#define IBASECLIENTDLL_INDEX_CREATEMOVE 21
#define IBASECLIENTDLL_INDEX_FRAMESTAGENOTIFY 35

#define IBASECLIENTDLL_INTERFACE "VClient017"
/*------------------------------------------------------------------------------*/

#endif