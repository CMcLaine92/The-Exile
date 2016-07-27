/***********************************************
* Filename:  		CaveSpawn.h
* Date:      		8/14/2015
* Mod. Date: 		9/10/2015
* Mod. Initials:	CM
* Author:    		Conner McLaine
* Purpose:			Represents inside of orc spawns, orc dont collide with this, player does
************************************************/

#ifndef _CAVESPAWN_H_
#define _CAVESPAWN_H_

#include "../../../Engine/Object Manager/IObject.h"


class CCaveSpawn : public IObject
{
private:

public:

	CCaveSpawn(XMFLOAT3 position);
	~CCaveSpawn() {}
	void Update() {}

};

#endif