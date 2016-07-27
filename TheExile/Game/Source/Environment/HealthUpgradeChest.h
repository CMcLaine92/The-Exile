/***********************************************
* Filename:  		HealthUpgradeChest.h
* Date:      		9/2/2015
* Mod. Date: 		9/2/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Represents a health upgrade chest
************************************************/

#ifndef CHEALTHUPGRADECHEST_H
#define CHEALTHUPGRADECHEST_H

#include "../../../Engine/Object Manager/IObject.h"
#include <iostream>
#include "IChest.h"


class CHealthUpgradeChest : public IChest
{

public:

	CHealthUpgradeChest(EDirection _direction = eNorth, XMFLOAT3 _pos = { 0.0f, 0.0f, 0.0f }, string _szTag = "WeaponChest");
	~CHealthUpgradeChest();
};

#endif
