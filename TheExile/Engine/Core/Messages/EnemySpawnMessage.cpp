/***********************************************
* Filename:  		HowToPlayState.cpp
* Date:      		8/20/2015
* Mod. Date: 		8/20/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			The message for spawning an enemy
************************************************/

#include "EnemySpawnMessage.h"

/*****************************************************************
* CEnemySpawnMessage()	The construcotr for EnemySpawnMessage
*
* Ins:					enemyType,
*						position
*
* Outs:					None
*
* Returns:				None
*
* Mod. Date:		    08/20/2015
* Mod. Initials:	    NH
*****************************************************************/
CEnemySpawnMessage::CEnemySpawnMessage(int enemyType, XMFLOAT3 position) : CMessage(MessageID::MSG_SPAWN_ENEMY)
{
	m_nType = enemyType;
	m_f3Position = position;
}

/*****************************************************************
* GetEnemySpawnType()	Returns the Type of enemy that this meassage 
*						is going to spawn
*
* Ins:					None
*
* Outs:					None
*
* Returns:				int
*
* Mod. Date:		    08/20/2015
* Mod. Initials:	    NH
*****************************************************************/
int CEnemySpawnMessage::GetEnemySpawnType() const
{
	return m_nType;
}

/*****************************************************************
* GetEnemySpawnPosition()	The construcotr for EnemySpawnMessage
*
* Ins:						None
*
* Outs:						None
*
* Returns:					XMFLOAT3
*
* Mod. Date:				08/20/2015
* Mod. Initials:			NH
*****************************************************************/
XMFLOAT3 CEnemySpawnMessage::GetEnemySpawnPosition() const
{
	return m_f3Position;
}
