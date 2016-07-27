/***********************************************
* Filename:  		HUD.h
* Date:      		8/18/2015
* Mod. Date: 		9/08/2015
* Mod. Initials:	NH
* Author:    		Nick Hryshko
* Purpose:			This is the player HUD class
************************************************/

#ifndef CHUD_H
#define CHUD_H

#include "../../../Engine/Renderer/RenderMesh.h"
#include "../../../Engine/Renderer/HUDMesh.h"

class CHUDMesh;
class CDayNight;

#define LOW_HEALTH 25
#define FLASH_TIME 0.5f

#define HEALTH_BAR_START 0.472f
#define HEALTH_BAR_XOFFSET 0.046f
#define	HEALTH_OUTLINE_RATIO 0.1f

struct TPlane
{
	CHUDMesh* m_cpRenderMesh;
	vector<TVertex2D> m_cvVertecies;
	vector<unsigned int> m_nvIndicies;
};


class CHUD
{
private:

	//health bar
	int m_nPlayerHealth;
	TPlane m_tHealthBar;
	TPlane m_tHealthFill;
	TPlane m_tStaminaFill;
	bool m_bHealthFlashON;
	bool m_bLightFlashON;
	float m_fHealthFlashTimer;
	float m_fStamina;
	int m_nMaxHealth;
	float m_fHealthBarOffset;

	//health vignette
	TPlane m_tHealthVignette;

	//time of day
	float m_fWorldTime;
	TPlane m_tCircle;//opaque quarter circle in top right corner
	TPlane m_tIcons;//sun and moon icons for top right corner
	unsigned int m_unDayCounter;
	CDayNight* m_cpDayNight;

	//inventory
	TPlane m_tInventory;

	//health potions
	TPlane m_tHealthPotionIcon;

	//exit key icon
	TPlane m_tExitKeyIcon;

public:

	bool bOff = false;

	/// <summary>
	/// The constructor
	/// </summary>
	/// <param> none</param>
	CHUD();

	/// <summary>
	/// The destructor
	/// </summary>
	/// <param> none</param>
	~CHUD();

	/// <summary>
	/// Updates all the HUD elements
	/// </summary>
	/// <param name = "playerHealth"> The amount of health that player has. </param>
	void Update(int playerHealth, int maxHealth, int numHealthPotions, float stamina);

	/// <summary>
	/// Sets the world time variable
	/// </summary>
	/// <param name = "time"> The amount of time that the world time will be set to. </param>
	void SetWorldTime(float time);

	/// <summary>
	/// Sets the day counter variable
	/// </summary>
	/// <param name = "day"> The day that the counter will be set to. </param>
	void SetDayCounter(int day);

	/// <summary>
	/// Adds 1 day to the day counter
	/// </summary>
	/// <param> none</param>
	void IncrementDayCounter();

	void SetMaxHealth(int _nMaxHealth);
	void SetDayNight(CDayNight* dayNight) { m_cpDayNight = dayNight; }
	float GetWorldTime() { return m_fWorldTime; }
	int GetCurrentDay() {return (int)m_unDayCounter;}

	void ExitKeyFound();

	void AddToRenderer();
};

#endif
