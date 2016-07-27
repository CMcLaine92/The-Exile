#ifndef CPARTICLEMANAGER_H
#define CPARTICLEMANAGER_H


/***********************************************
* Filename:  		ParticleManager.h
* Date:      		08/25/2015
* Mod. Date: 		----------
* Mod. Initials:	--
* Author:    		Matthew Zanini
* Purpose:   		Keeps track of all emitters
*					Updating Emitters and deallocating
*					Emitters
************************************************/
#include "Flyweight.h"
#include <string>
#include <vector>
class CEmitter;
class CObjectManager;
class CParticleManager
{

private:
	std::vector<CEmitter*> m_vpEmitters;
	CObjectManager* m_pObjectManager;
	ID3D11Buffer* m_pVertexBuffer;
public:
	/// <summary>
	/// Creates the Particle Manager.
	/// </summary>
	/// <param name = "pObject"> The object manager.</param>
	CParticleManager(CObjectManager* pObject);

	/// <summary>
	/// Deletes all data that needs to be deleted.
	/// </summary>
	/// <param> none</param>
	~CParticleManager();

	/// <summary>
	/// Updates all emitters
	/// </summary>
	/// <param> none</param>
	void Update(void);

	/// <summary>
	/// Loads emitter and returns value to access it later
	/// </summary>
	/// <param name = "szFilepath"> The filepath relative to Scripts/Particles.</param>
	unsigned int LoadEmitter(std::string szFilepath);

	/// <summary>
	/// Returns specified emitter
	/// </summary>
	/// <param name = "unID"> The ID of the emitter.</param>
	CEmitter* GetEmitter(unsigned int unID);
	ID3D11Buffer*& GetVertexBuffer()
	{
		return m_pVertexBuffer;
	}
};

#endif
