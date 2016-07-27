
#ifndef CPARTICLE_H
#define CPARTICLE_H

/***********************************************
* Filename:  		Particle.h
* Date:      		08/25/2015
* Mod. Date: 		----------
* Mod. Initials:	--
* Author:    		Matthew Zanini
* Purpose:   		Individual Particle Behaviors
*					based on flyweight data from emitter
************************************************/
class CEmitter;
class CParticle
{

private:
	//Particle is Active
	bool m_bIsActive;
	//Particle's Emitter
	CEmitter* m_cEmitter;
	//Position
	float m_f3Position[3];
	//Start Velocity
	float m_f3StartVelocity[3];
	//End Velocity
	float m_f3EndVelocity[3];
	//Current Velocity
	float m_f3CurrentVelocity[3];
	//Start Life, Current Life
	float m_f2Life[2];
	//Start Size, End Size
	float m_f2Size[2];

	
	
	/// <summary>
	/// Returns the ratio of current life to total life.
	/// </summary>
	/// <param> none</param>
	float GetLifetimeRatio();

public:

	//Accessor/Mutators
#pragma region Accessor/Mutators
	bool& GetActive()
	{
		return m_bIsActive;
	}
	CEmitter*& GetEmitter()
	{
		return m_cEmitter;
	}
	float& GetPosition(unsigned int unXYZ)
	{
		return m_f3Position[unXYZ];
	}
	float& GetStartVelocity(unsigned int unXYZ)
	{
		return m_f3StartVelocity[unXYZ];
	}
	float& GetEndVelocity(unsigned int unXYZ)
	{
		return m_f3EndVelocity[unXYZ];
	}
	float& GetCurrentVelocity(unsigned int unXYZ)
	{
		return m_f3CurrentVelocity[unXYZ];
	}
	float& GetStartLife(void)
	{
		return m_f2Life[0];
	}
	float& GetCurrentLife(void)
	{
		return m_f2Life[1];
	}
	float& GetStartSize(void)
	{
		return m_f2Size[0];
	}
	float& GetEndSize(void)
	{
		return m_f2Size[1];
	}
#pragma endregion

	/// <summary>
	/// Updates Velocity, Position and Lifetime if active.
	/// </summary>
	/// <param> none</param>
	void Update(void);

	/// <summary>
	/// Initializes default Values.
	/// </summary>
	/// <param name = "cEmitter"> The owning emitter of the particle.</param>
	CParticle(CEmitter* cEmitter);
	
	CParticle();
	~CParticle() = default;

	/// <summary>
	/// Initializes Particle Data and sets it as active
	/// </summary>
	/// <param> none</param>
	void SpawnParticle();

	/// <summary>
	/// Sets Particle as inactive and resets position to unused
	/// </summary>
	/// <param> none</param>
	void DespawnParticle();
};

#endif

