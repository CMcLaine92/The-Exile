/***********************************************
* Filename:  		LightWeaponChest.h
* Date:      		9/2/2015
* Mod. Date: 		9/2/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Represents a light weapon upgrade chest
************************************************/

#ifndef CLIGHTWEAPONCHEST_H
#define CLIGHTWEAPONCHEST_H

#include "../../../Engine/Object Manager/IObject.h"
#include "IChest.h"
#include <iostream>


class CLightWeaponChest : public IChest
{

public:

	CLightWeaponChest(EDirection _direction = eNorth, XMFLOAT3 _pos = { 0.0f, 0.0f, 0.0f }, string _szTag = "WeaponChest");
	~CLightWeaponChest();

};

#endif
