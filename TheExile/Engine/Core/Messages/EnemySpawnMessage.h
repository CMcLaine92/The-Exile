/***********************************************
* Filename:  		HowToPlayState.h
* Date:      		8/20/2015
* Mod. Date: 		8/20/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			The message for spawning an enemy
************************************************/

#ifndef CENEMYSPAWNMESSAGE_H
#define CENEMYSPAWNMESSAGE_H


#include "../Messages/Message.h"


class CEnemySpawnMessage : public CMessage
{
private:

	int m_nType;
	XMFLOAT3 m_f3Position;

public:

	/// <summary>
	/// Constructor for EnemySpawnMessage
	/// </summary>
	/// <param name = "enemyType"> The type of enemy being spawned. 1 = skeleton, 2 = orc, 3 = minotaur. </param>
	/// <param name = "position"> The position that the enemy will be spawned at. </param>
	CEnemySpawnMessage(int enemyType, XMFLOAT3 position);

	/// <summary>
	/// Default destructor for EnemySpawnMessage
	/// </summary>
	/// <param> none</param>
	~CEnemySpawnMessage() = default;



	/***************
	*  Accessors
	***************/

	/// <summary>
	/// Returns the Type of enemy that this meassage is going to spawn
	/// </summary>
	/// <param> none</param>
	int GetEnemySpawnType() const;

	/// <summary>
	/// Returns the position that this enemy will spawn at
	/// </summary>
	/// <param> none</param>
	XMFLOAT3 GetEnemySpawnPosition() const;

};

#endif
