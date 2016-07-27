/***********************************************
* Filename:  		SpawnerObject.h
* Date:      		9/2/2015
* Mod. Date: 		9/2/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Represents a spawn point
************************************************/

#ifndef CSPAWNEROBJECT_H
#define CSPAWNEROBJECT_H

#include <iostream>


class CSpawnerObject /*: public IObject*/
{
private:

	std::string tag;

	XMFLOAT4X4 m_mWorld;
	XMFLOAT3 m_f3SpawnPosition;
	XMFLOAT3 m_f3SpawnDirection;//used for special and trap spawners
	float m_fRotationSpeed;//used for traps
	XMFLOAT3 m_f3StartPoint;//used for trap waypoint movement
	XMFLOAT3 m_f3EndPoint;//used for trap waypoint movement
	unsigned int m_unID;//used for SB IDs
	float m_fStartTime;//used for trap activation time
	float m_fOffsetTime;//used for trap activation time
	bool keep = true;

public:

	CSpawnerObject(string szTag);
	~CSpawnerObject();


	//normal spawners and directional object(torch, chest, fire trap, etc...) 
	void SetAttributes(XMFLOAT3 position, XMFLOAT3 direction = { 0, 0, 0 });

	//spinning blade trap
	void SetAttributes(XMFLOAT3 position, float rotationSpeed = 0.0f, XMFLOAT3 startPoint = { 0, 0, 0 }, XMFLOAT3 endPoint = { 0, 0, 0 }, unsigned int ID = 0);

	//spike trap
	void SetAttributes(XMFLOAT3 position, float startTime = 0.0f, float offsetTime = 0.0f);

	

	void SetWorldMatrix(const XMFLOAT4X4 matrix);

	//accessors
	XMFLOAT3& GetSpawnPosition()	{ return m_f3SpawnPosition; }
	XMFLOAT3& GetSpawnDirection()	{ return m_f3SpawnDirection; }
	float& GetRotationSpeed()		{ return m_fRotationSpeed; }
	XMFLOAT3& GetStartPoint()		{ return m_f3StartPoint; }
	XMFLOAT3& GetEndPoint()			{ return m_f3EndPoint; }
	unsigned int& GetID()			{ return m_unID; }
	float& GetStartTime()			{ return m_fStartTime; }
	float& GetOffsetTime()			{ return m_fOffsetTime; }

	const XMFLOAT3& GetSpawnPosition()const	{ return m_f3SpawnPosition; }
	const XMFLOAT3& GetSpawnDirection()const{ return m_f3SpawnDirection; }
	const float& GetRotationSpeed()const	{ return m_fRotationSpeed; }
	const XMFLOAT3& GetStartPoint()const	{ return m_f3StartPoint; }
	const XMFLOAT3& GetEndPoint()const		{ return m_f3EndPoint; }
	const unsigned int& GetID()const		{ return m_unID; }
	const float& GetStartTime()const		{ return m_fStartTime; }
	const float& GetOffsetTime()const		{ return m_fOffsetTime; }




};

#endif
