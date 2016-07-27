#pragma once

/***********************************************
* Filename:  		Time.h
* Date:      		08/05/2015
* Mod. Date: 		----------
* Mod. Initials:	--
* Author:    		Nick Dalton
* Purpose:   		Keep track of time between
*					frames and current time
************************************************/

extern int l;
class CTime
{
	double m_dDeltaTime = 0;
	double m_dFrequency = 0;

	__int64 m_nStartTime = 0;
	__int64 m_nCurrentTime = 0;
	__int64 m_nPreviousTime = 0;

public:

	CTime();
	~CTime() = default;

	void Tick();
	float Current();
	float Delta() { return (float)m_dDeltaTime; }
};