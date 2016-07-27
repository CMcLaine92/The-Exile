/***********************************************
* Filename:  		HeavyWeaponChest.h
* Date:      		9/2/2015
* Mod. Date: 		9/2/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Represents a heavy weapon upgrade chest
************************************************/

#ifndef CHEAVYWEAPONCHEST_H
#define CHEAVYWEAPONCHEST_H

#include "../../../Engine/Object Manager/IObject.h"
#include <iostream>
#include "IChest.h"


class CHeavyWeaponChest : public IChest
{

public:

	CHeavyWeaponChest(EDirection _direction = eNorth, XMFLOAT3 _pos = { 0.0f, 0.0f, 0.0f }, string _szTag = "WeaponChest");
	~CHeavyWeaponChest();
};

#endif
