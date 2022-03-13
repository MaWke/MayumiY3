#pragma once

#include "XYZ.h"
#include "offsets.h"


/*															*\
	*													*
	*					MAYUMIY3						*
	*													*
\*															*/


class Entity 
//
//Abstract Entity base class
//You are not supposed to/you can't "construct Entities" as they are already in the games memory. You point to them and their values
//
{
public:
	operator bool() const;
	uintptr_t operator* () const;			//override the dereference operator with ReadProcessMemory

	protected:
	Entity() {};
};

class Player : public Entity
//
//CSPlayer
//
{
public:
	Vec3 getOrigin()			const;		
	Vec3 getViewOffset()		const;
	Vec3 getEyePos()			const;
	Vec3 getBonePos(int boneID)	const;
	int getHealth()				const;
	int getTeam()				const;
	int getGlowIndex()			const;
	
};

class LocalPlayer : public Player
//
//CSLocalPlayer
//
{
public:
	void forceJump();
	void setViewAngles(ViewAngle angle);

	ViewAngle getViewAngles()	const;		//from engine.dll
	int getfFlags()				const;
};

//
//EntityList
//
namespace EntityList {
	Player* get(int index);										//Player from CSGO's EntityList	
	ViewAngle calcViewAnglesToBestTarget(float fov, ViewAngle lpViewAngles, Vec3 lpEyePos);	//Return best target that's in the fov based on
																//player distance from localPlayer and on the screen

	LocalPlayer* getLocalPlayer();								//LocalPlayer is in the EntityList
}

enum class EntityFlags
//
//Entity flags from csgosimple's "Enums.hpp"
//
{
	FL_ONGROUND = (1 << 0), 	// At rest / on the ground
	FL_DUCKING = (1 << 1),		// Player flag -- Player is fully crouched
	FL_WATERJUMP = (1 << 2),	// player jumping out of water
	FL_ONTRAIN = (1 << 3),		// Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
	FL_INRAIN = (1 << 4),		// Indicates the entity is standing in rain
	FL_FROZEN = (1 << 5),		// Player is frozen for 3rd person camera
	FL_ATCONTROLS = (1 << 6),	// Player can't move, but keeps key inputs for controlling another entity
	FL_CLIENT = (1 << 7),		// Is a player
	FL_FAKECLIENT = (1 << 8),	// Fake client, simulated server side; don't send network messages to them
	FL_INWATER = (1 << 10),		// In water
};

//
//Glow
//
struct GlowStruct
{
	unsigned char base[8];	//Padding
	float red;
	float green;
	float blue;
	float alpha;
	unsigned char buffer2[16];	//Padding
	bool renderWhenOccluded;
	bool renderWhenUnOccluded;
	bool fullBloom;
	unsigned char buffer3[5];	//Padding 
	int glowStyle;

	//void operator = (const GlowStruct& glowStruct);

	GlowStruct operator* ();
};

namespace GlowObjectManager {
	GlowStruct* get(int index);
}

//
//BoneMatrix
//
struct BoneInBoneMatrix
{
	unsigned char buffer[12];	//Padding
	float x;
	unsigned char buffer2[12];	//Padding
	float y;
	unsigned char buffer3[12];	//Padding
	float z;
};

namespace BoneMatrix {
	BoneInBoneMatrix getBoneInMatrix(int boneID, uintptr_t boneMatrix);
}

//
//Server
// 
namespace Server {
	int getMaxPlayer();
}