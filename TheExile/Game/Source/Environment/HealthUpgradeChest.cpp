/***********************************************
* Filename:  		HealthUpgradeChest.cpp
* Date:      		9/2/2015
* Mod. Date: 		9/2/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Represents a health upgrade chest
************************************************/

#include "HealthUpgradeChest.h"
#include "../../../Engine/Asset Manager/AssetManager.h"
#include "../../../Engine/Core/stdafx.h"
#include "../../../Engine/Core/Collision/Capsule.h"
#include "../../../Engine/Core/Messages/AddObjectMessage.h"

CHealthUpgradeChest::CHealthUpgradeChest(EDirection _direction, XMFLOAT3 _pos, string _szTag) : IChest(eHealthChest, _direction, _pos, "HealthChest")
{

}

CHealthUpgradeChest::~CHealthUpgradeChest()
{

}
