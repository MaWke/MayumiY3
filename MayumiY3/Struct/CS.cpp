#include <type_traits>

#include "CS.h"
#include "../process/Process.h"
#include "../utils/math.h"

enum class typeSizes
{
	ENTITY = 0x10,
	GLOWSTRUCT = 0x38,
	BONEINMATRIX = 0x30
};
	
//
//Entity
//
Entity::operator bool() const
{
	return Process::readMem<uintptr_t>(uintptr_t(this));
}

uintptr_t Entity::operator* () const
{
	return Process::readMem<uintptr_t>((uintptr_t)this);
}
//-----------------------------------------------------------------------

//
//Player
//
Vec3 Player::getOrigin() const
{
	return Process::readMem<Vec3>(uintptr_t(**this + offsets::netvars::m_vecOrigin));
}

Vec3 Player::getViewOffset() const
{
	return Process::readMem<Vec3>(uintptr_t(**this + offsets::netvars::m_vecViewOffset));
}

Vec3 Player::getEyePos() const
{
	return getViewOffset() + getOrigin();
}

Vec3 Player::getBonePos(int boneID) const
{
	uintptr_t boneMatrix = Process::readMem<uintptr_t>(uintptr_t(**this + offsets::netvars::m_dwBoneMatrix));

	BoneInBoneMatrix bone = BoneMatrix::getBoneInMatrix(boneID, boneMatrix);
	return Vec3(bone.x, 
				bone.y, 
				bone.z);
}

int Player::getHealth() const
{
	return Process::readMem<int>(uintptr_t(**this + offsets::netvars::m_iHealth));
}

int Player::getTeam() const
{
	return Process::readMem<int>(uintptr_t(**this + offsets::netvars::m_iTeamNum));
}

int Player::getGlowIndex() const
{
	return Process::readMem<int>(uintptr_t(**this + offsets::netvars::m_iGlowIndex));
}
//-----------------------------------------------------------------------

//
//Player : LocalPlayer
//
void LocalPlayer::forceJump()
{
	static uintptr_t forceJumpAddress = (Process::getModuleAddr(Process::Modules::CLIENTDLL) + offsets::signatures::dwForceJump);
	Process::writeMem<int>(forceJumpAddress, 6);	//6 == forcejump
}

void LocalPlayer::setViewAngles(ViewAngle angle)
{
	static uintptr_t dwClientState = Process::readMem<uintptr_t>(uintptr_t(Process::getModuleAddr(Process::Modules::ENGINEDLL) + offsets::signatures::dwClientState));
	Process::writeMem<ViewAngle>(uintptr_t(dwClientState + offsets::signatures::dwClientState_ViewAngles), angle);
}

ViewAngle LocalPlayer::getViewAngles() const
{
	static uintptr_t dwClientState = Process::readMem<uintptr_t>(uintptr_t(Process::getModuleAddr(Process::Modules::ENGINEDLL) + offsets::signatures::dwClientState));
	return Process::readMem<ViewAngle>(uintptr_t(dwClientState + offsets::signatures::dwClientState_ViewAngles));
}

int LocalPlayer::getfFlags() const
{
	return Process::readMem<int>(uintptr_t(**this + offsets::netvars::m_fFlags));
}
//-----------------------------------------------------------------------

//
//EntityList
//
Player* EntityList::get(int index)
{
	static uintptr_t moduleBase = Process::getModuleAddr(Process::Modules::CLIENTDLL);
	static uintptr_t entityList = moduleBase + offsets::signatures::dwEntityList;
	return (Player*)(entityList + index * (int)typeSizes::ENTITY);
}

ViewAngle EntityList::calcViewAnglesToBestTarget(float fov, ViewAngle lpViewAngles, Vec3 lpEyePos)
{
	LocalPlayer* localPlayer = getLocalPlayer();

	float closestDistance = FLT_MAX;
	ViewAngle closestViewAngle(NAN, NAN);

	for (int i = 0; i < Server::getMaxPlayer(); ++i)
	{
		Player* player = get(i);

		if (!player 
			|| player == localPlayer
			|| player->getHealth() <= 0
			|| player->getTeam() == localPlayer->getTeam())
			continue;

		ViewAngle anglesToPlayer = Math::Vector::getViewAngleBetween(
			lpEyePos, player->getBonePos(8));	//@todo change the boneID to something maybe a picker

		float vaDistance = Math::Vector::distanceVA(lpViewAngles, anglesToPlayer);
		
		if (vaDistance < closestDistance && vaDistance < fov) {
			closestViewAngle = anglesToPlayer;
			closestDistance = vaDistance;
		}
	}

	return closestViewAngle;
}

LocalPlayer* EntityList::getLocalPlayer()
{
	static uintptr_t moduleBase = Process::getModuleAddr(Process::Modules::CLIENTDLL);
	static LocalPlayer* lpp = (LocalPlayer*)(moduleBase + offsets::signatures::dwLocalPlayer);
	return lpp;
}
//-----------------------------------------------------------------------

//
//Glow
//

GlowStruct* GlowObjectManager::get(int index)
{
	static uintptr_t moduleBase = Process::getModuleAddr(Process::Modules::CLIENTDLL);
	static uintptr_t glowObjectManager = Process::readMem<uintptr_t>(uintptr_t(moduleBase + offsets::signatures::dwGlowObjectManager));
	return (GlowStruct*)(glowObjectManager + (index * (int)typeSizes::GLOWSTRUCT));	
}

//void GlowStruct::operator = (const GlowStruct& glowStruct) {
//	Process::writeMem((uintptr_t)this, glowStruct);
//}

GlowStruct GlowStruct::operator* () {
	return Process::readMem<GlowStruct>((uintptr_t)this);
}

//
//BoneMatrix
//
BoneInBoneMatrix BoneMatrix::getBoneInMatrix(int boneID, uintptr_t boneMatrix)
{
	return Process::readMem<BoneInBoneMatrix>(boneMatrix + boneID * (int)typeSizes::BONEINMATRIX);
}

//
//Server
//
int Server::getMaxPlayer()
{
	static uintptr_t moduleBase = Process::getModuleAddr(Process::Modules::ENGINEDLL);
	static uintptr_t dwClientState = Process::readMem<uintptr_t>(moduleBase + offsets::signatures::dwClientState);
	return Process::readMem<int>(dwClientState + offsets::signatures::dwClientState_MaxPlayer);
}

