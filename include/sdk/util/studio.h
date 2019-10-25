#ifndef SDK_STUDIO_H
#define SDK_STUDIO_H
#pragma once

#include "../math/mathlib.h"

#define MAX_QPATH  260

#define BONE_CALCULATE_MASK             0x1F
#define BONE_PHYSICALLY_SIMULATED       0x01
#define BONE_PHYSICS_PROCEDURAL         0x02
#define BONE_ALWAYS_PROCEDURAL          0x04
#define BONE_SCREEN_ALIGN_SPHERE        0x08
#define BONE_SCREEN_ALIGN_CYLINDER      0x10

#define BONE_USED_MASK                  0x0007FF00
#define BONE_USED_BY_ANYTHING           0x0007FF00
#define BONE_USED_BY_HITBOX             0x00000100
#define BONE_USED_BY_ATTACHMENT         0x00000200
#define BONE_USED_BY_VERTEX_MASK        0x0003FC00
#define BONE_USED_BY_VERTEX_LOD0        0x00000400
#define BONE_USED_BY_VERTEX_LOD1        0x00000800    
#define BONE_USED_BY_VERTEX_LOD2        0x00001000  
#define BONE_USED_BY_VERTEX_LOD3        0x00002000
#define BONE_USED_BY_VERTEX_LOD4        0x00004000
#define BONE_USED_BY_VERTEX_LOD5        0x00008000
#define BONE_USED_BY_VERTEX_LOD6        0x00010000
#define BONE_USED_BY_VERTEX_LOD7        0x00020000
#define BONE_USED_BY_BONE_MERGE         0x00040000

#define BONE_USED_BY_VERTEX_AT_LOD(lod) ( BONE_USED_BY_VERTEX_LOD0 << (lod) )
#define BONE_USED_BY_ANYTHING_AT_LOD(lod) ( ( BONE_USED_BY_ANYTHING & ~BONE_USED_BY_VERTEX_MASK ) | BONE_USED_BY_VERTEX_AT_LOD(lod) )

#define MAX_NUM_LODS 8
#define MAXSTUDIOBONES 128

#define BONE_TYPE_MASK                  0x00F00000
#define BONE_FIXED_ALIGNMENT            0x00100000

#define BONE_HAS_SAVEFRAME_POS          0x00200000
#define BONE_HAS_SAVEFRAME_ROT64        0x00400000
#define BONE_HAS_SAVEFRAME_ROT32        0x00800000

typedef float Quaternion[4];
typedef float RadianEuler[3];
typedef unsigned short MDLHandle_t;
struct model_t; // no idea... can't find the struct anywhere sorry

enum modtype_t
{
	mod_bad = 0,
	mod_brush,
	mod_sprite,
	mod_studio
};

struct mstudiobone_t
{
	int                    sznameindex;
	inline char* const	   pszName(void) const { return ((char*)this) + sznameindex; }
	int                    parent;
	int                    bonecontroller[6];
	Vector                 pos;
	Quaternion             quat;
	RadianEuler            rot;
	
	Vector                 posscale;
	Vector                 rotscale;

	matrix3x4_t            poseToBone;
	Quaternion             qAlignment;
	int                    flags;
	int                    proctype;
	int                    procindex;
	mutable int            physicsbone;
	inline void *          pProcedure() const { if (procindex == 0) return NULL; else return  (void *)(((byte *)this) + procindex); };
	int                    surfacepropidx;
	inline char * const    pszSurfaceProp(void) const { return ((char *)this) + surfacepropidx; }
	inline int             GetSurfaceProp(void) const { return surfacepropLookup; }

	int                    contents;
	int                    surfacepropLookup;
	int                    m_iPad01[7];
};

struct mstudiobbox_t
{
	int         bone;
	int         group;
	Vector      bbmin;
	Vector      bbmax;
	int         szhitboxnameindex;

	const char* GetName()
	{
		if (!szhitboxnameindex) return nullptr;
		return (const char*)((uint8_t*)this + szhitboxnameindex);
	}
};

struct mstudiohitboxset_t
{
	int    sznameindex;
	int    numhitboxes;
	int    hitboxindex;

	const char* GetName()
	{
		if (!sznameindex) return nullptr;
		return (const char*)((uint8_t*)this + sznameindex);
	}

	mstudiobbox_t* GetHitbox(int i)
	{
		if (i > numhitboxes) return nullptr;
		return (mstudiobbox_t*)((uint8_t*)this + hitboxindex) + i;
	}
};

class studiohdr_t
{
public:
	int id;
	int version;
	long checksum;
	char szName[64];
	int length;
	Vector vecEyePos;
	Vector vecIllumPos;
	Vector vecHullMin;
	Vector vecHullMax; 
	Vector vecBBMin;  
	Vector vecBBMax; 
	int flags;
	int numbones;
	int boneindex;
	int numbonecontrollers;
	int bonecontrollerindex;
	int numhitboxsets;
	int hitboxsetindex;
	int numlocalanim;
	int localanimindex;
	int numlocalseq;
	int localseqindex;
	int activitylistversion;
	int eventsindexed;
	int numtextures;
	int textureindex;

	mstudiohitboxset_t* GetHitboxSet(int i)
	{
		if (i > numhitboxsets) return nullptr;
		return (mstudiohitboxset_t*)((uint8_t*)this + hitboxsetindex) + i;
	}
	mstudiobone_t* GetBone(int i)
	{
		if (i > numbones) return nullptr;
		return (mstudiobone_t*)((uint8_t*)this + boneindex) + i;
	}
};

#endif