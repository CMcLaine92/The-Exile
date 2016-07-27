/***********************************************
* Filename:  		HeavyWeaponChest.cpp
* Date:      		9/2/2015
* Mod. Date: 		9/2/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Represents a heavy weapon upgrade chest
************************************************/

#include "HeavyWeaponChest.h"
#include "../../../Engine/Asset Manager/AssetManager.h"
#include "../../../Engine/Core/stdafx.h"
#include "../../../Engine/Core/Collision/Capsule.h"
#include "../../../Engine/Core/Messages/AddObjectMessage.h"

CHeavyWeaponChest::CHeavyWeaponChest(EDirection _direction, XMFLOAT3 _pos, string _szTag) : IChest(eShieldChest, _direction, _pos, "ShieldChest")
{

}

CHeavyWeaponChest::~CHeavyWeaponChest()
{

}

