/***********************************************
* Filename:  		LightWeaponChest.cpp
* Date:      		9/2/2015
* Mod. Date: 		9/2/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Represents a light weapon upgrade chest
************************************************/

#include "LightWeaponChest.h"
#include "../../../Engine/Asset Manager/AssetManager.h"
#include "../../../Engine/Core/stdafx.h"
#include "../../../Engine/Core/Collision/Capsule.h"
#include "../../../Engine/Core/Messages/AddObjectMessage.h"

CLightWeaponChest::CLightWeaponChest(EDirection _direction, XMFLOAT3 _pos, string _szTag) : IChest(eWeaponChest, _direction, _pos, "WeaponChest")
{
	
}

CLightWeaponChest::~CLightWeaponChest()
{

}
