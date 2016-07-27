#ifndef DAYNIGHT_H_
#define DAYNIGHT_H_
#include "../../../Engine/Renderer/ParticleMesh.h"
#include "../../../Engine/Renderer/Lights.h"
class CCamera;
class CParticleManager;
class CDayNight
{
private:
	//0-360 0,360 = dawn 180 = start of night
	float m_fTimeOfDay = 0.0f;
	bool m_bIsDay = true;
	
	bool m_bTimeOfDayChanged = false;

	CParticleMesh* m_pcSunMesh = nullptr;
	CParticleMesh* m_pcMoonMesh = nullptr;
	std::vector<TParticleVertex> m_vSunVertex;
	std::vector<TParticleVertex> m_vMoonVertex;
	CCamera* m_pcCamera = nullptr;
	CParticleManager* m_pcParticleManager = nullptr;

	float m_fMinutesPerDay = 0.0f;
	float m_fMinutesPerNight = 0.0f;

	//Dawn, Day, Dusk, Night
	XMFLOAT3 m_f3LightColors[4];

	float m_fDoorTimer = -1.0f;
	std::vector<unsigned int> m_vunParticleIndex;
	std::vector<unsigned int> m_vunBottomParticles;
	IObject* m_pcHavenDoors[4];

	float m_fSkyObjectDistance = 10000.0f;


	void UpdateMeshes();
	void LoadParticles();
public:
	CDayNight(CCamera* pcCamera,CParticleManager* pcParticleManager);
	~CDayNight();

	void SetTimer(float fDoorTimer);
	void Update();
	bool IsDay();
	bool IsNight();
	bool TimeOfDayChanged() { return m_bTimeOfDayChanged; }

	void PlayParticles();
	void AddDoors(unsigned int unIndex,IObject* pcDoor);
	void SendDayNightMessage();
	void UpdateDirectionalLight(TDirectLight* tDirLight);
	void UpdateSkybox(CRenderMesh* pcSkyboxMesh);

	float GetMinutesPerDay(){ return m_fMinutesPerDay; }
	float GetMinutesPerNight(){ return m_fMinutesPerNight; }
	float GetTimeOfDay() { return m_fTimeOfDay; }
	float GetDoorTimer() { return m_fDoorTimer; }

	void SetTimeOfDay(float _time) { m_fTimeOfDay = _time; }
	void SetDay(bool _day) { m_bIsDay = _day; }
};

#endif