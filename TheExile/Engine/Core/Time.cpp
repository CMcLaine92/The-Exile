#include "Time.h"

// Global Accessor //
const void* gGlobalTime = nullptr;

/*****************************************************************
* CTime():			Initializes the CTime class
*					 
* Ins:			    N/A
*					 
* Outs:				N/A
*					 
* Returns:		    N/A
*					 
* Mod. Date:		----------
* Mod. Initials:	--
*****************************************************************/
CTime::CTime()
{
	gGlobalTime = this;

	__int64 nFreq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&nFreq);
	m_dFrequency = 1000.0 / (double)nFreq;

	QueryPerformanceCounter((LARGE_INTEGER*)&m_nStartTime);

	Tick();
}

/*****************************************************************
* Tick():			Updates m_dDeltaTime; Needs called every frame
*
* Ins:			    N/A
*
* Outs:				N/A
*
* Returns:		    N/A
*
* Mod. Date:		----------
* Mod. Initials:	--
*****************************************************************/
void CTime::Tick()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurrentTime);

	m_dDeltaTime = ((m_nCurrentTime - m_nPreviousTime) * m_dFrequency) / 1000.0;

	if (m_dDeltaTime > 0.1)
		m_dDeltaTime = 0.1f;

	m_nPreviousTime = m_nCurrentTime;
}

/*****************************************************************
* Current():		Returns the current time of the OS
*
* Ins:			    N/A
*
* Outs:				N/A
*
* Returns:		    float
*
* Mod. Date:		----------
* Mod. Initials:	--
*****************************************************************/
float CTime::Current()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurrentTime);

	return (float)(((m_nCurrentTime - m_nStartTime) * m_dFrequency) / 1000.0f);
}