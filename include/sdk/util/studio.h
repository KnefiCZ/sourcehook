#ifndef SDK_STUDIO_H
#define SDK_STUDIO_H
#pragma once

#include "../math/mathlib.h"
#include "../math/qangle.h"

#define MAX_QPATH  260

#define BONE_CALCULATE_MASK 0x1F
#define BONE_PHYSICALLY_SIMULATED 0x01
#define BONE_PHYSICS_PROCEDURAL 0x02
#define BONE_ALWAYS_PROCEDURAL 0x04
#define BONE_SCREEN_ALIGN_SPHERE 0x08
#define BONE_SCREEN_ALIGN_CYLINDER 0x10

#define BONE_USED_MASK 0x0007FF00
#define BONE_USED_BY_ANYTHING 0x0007FF00
#define BONE_USED_BY_HITBOX 0x00000100
#define BONE_USED_BY_ATTACHMENT 0x00000200
#define BONE_USED_BY_VERTEX_MASK 0x0003FC00
#define BONE_USED_BY_VERTEX_LOD0 0x00000400
#define BONE_USED_BY_VERTEX_LOD1 0x00000800    
#define BONE_USED_BY_VERTEX_LOD2 0x00001000  
#define BONE_USED_BY_VERTEX_LOD3 0x00002000
#define BONE_USED_BY_VERTEX_LOD4 0x00004000
#define BONE_USED_BY_VERTEX_LOD5 0x00008000
#define BONE_USED_BY_VERTEX_LOD6 0x00010000
#define BONE_USED_BY_VERTEX_LOD7 0x00020000
#define BONE_USED_BY_BONE_MERGE 0x00040000

#define BONE_USED_BY_VERTEX_AT_LOD(lod) ( BONE_USED_BY_VERTEX_LOD0 << (lod) )
#define BONE_USED_BY_ANYTHING_AT_LOD(lod) ( ( BONE_USED_BY_ANYTHING & ~BONE_USED_BY_VERTEX_MASK ) | BONE_USED_BY_VERTEX_AT_LOD(lod) )

#define MAX_NUM_LODS 8
#define MAXSTUDIOBONES 128

#define BONE_TYPE_MASK 0x00F00000
#define BONE_FIXED_ALIGNMENT 0x00100000

#define BONE_HAS_SAVEFRAME_POS 0x00200000
#define BONE_HAS_SAVEFRAME_ROT64 0x00400000
#define BONE_HAS_SAVEFRAME_ROT32 0x00800000

typedef float Quaternion[4];
struct model_t;

struct mstudiobbox_t
{
	int bone;
	int group;
	Vector bbmin;
	Vector bbmax;
	int szhitboxnameindex;
	int unused[8];
};

struct mstudiobone_t
{
	int sznameindex;
	int parent;
	int bonecontroller[6];

	Vector pos;
	Quaternion quat;
	Vector rot;
	Vector posscale;
	Vector rotscale;

	VMatrix poseToBone;
	Quaternion qAlignment;

	int flags;
	int proctype;
	int procindex;
	int physicsbone;
	int surfacepropidx;
	int contents;
	int unused[8];
};

struct mstudiohitboxset_t
{
	int sznameindex;
	int numhitboxes;
	int hitboxindex;

	mstudiobbox_t* GetHitbox(int i)
	{ 
		return (mstudiobbox_t *)(((char*)this) + hitboxindex) + i;
	};
};

struct studiohdr_t
{
	int id;
	int version;
	long checksum;
	char name[64];
	int length;

	Vector eyeposition;
	Vector illumposition;
	Vector hull_min;
	Vector hull_max;
	Vector view_bbmin;
	Vector view_bbmax;

	int flags;
	int numbones;
	int boneindex;
	int numbonecontrollers;
	int bonecontrollerindex;

	int numhitboxsets;
	int hitboxsetindex;
	
	mstudiohitboxset_t* GetHitboxSet(int i)
	{
		return (mstudiohitboxset_t *)(((char*)this) + hitboxsetindex) + i;
	};
};

#endif