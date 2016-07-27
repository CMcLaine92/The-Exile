/***********************************************
* Filename:  		SpawnerObject.cpp
* Date:      		9/2/2015
* Mod. Date: 		9/2/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			Represents a spawn point
************************************************/

#include "SpawnerObject.h"

CSpawnerObject::CSpawnerObject(string szTag) 
{
	tag = szTag;
	m_f3SpawnPosition = { 0, 0, 0 };
	m_f3SpawnDirection = { 0, 0, 0 };
	m_fRotationSpeed = 0.0f;
	m_f3StartPoint = { 0, 0, 0 };
	m_f3EndPoint = { 0, 0, 0 };
	m_unID = 0;
	m_fStartTime = 0.0f;
	m_fOffsetTime = 0.0f;
}

CSpawnerObject::~CSpawnerObject()
{
}


void CSpawnerObject::SetWorldMatrix(const XMFLOAT4X4 matrix)
{
	m_mWorld = matrix;
	m_f3SpawnPosition = float3(matrix._41, matrix._42, matrix._43);
}


void CSpawnerObject::SetAttributes(XMFLOAT3 position, XMFLOAT3 direction)
{
	m_f3SpawnPosition = position;
	m_f3SpawnDirection = direction;
}

void CSpawnerObject::SetAttributes(XMFLOAT3 position, float rotationSpeed, XMFLOAT3 startPoint, XMFLOAT3 endPoint, unsigned int ID)
{
	m_f3SpawnPosition = position;
	m_fRotationSpeed = rotationSpeed;
	m_f3StartPoint = startPoint;
	m_f3EndPoint = endPoint;
	m_unID = ID;
}

void CSpawnerObject::SetAttributes(XMFLOAT3 position, float startTime, float offsetTime)
{
	m_f3SpawnPosition = position;
	m_fStartTime = startTime;
	m_fOffsetTime = offsetTime;
}


