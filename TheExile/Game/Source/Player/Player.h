
#ifndef CPLAYER_H
#define CPLAYER_H

#include "../../../Engine/Object Manager/IObject.h"
#include "../../../Engine/Core/Input/InputManager.h"
#include "../../../Engine/Animation/Animation.h"
#include "../../../Engine/Animation/Mesh.h"
#include "ItemPickupAnimation.h"
#include <queue>

#define EXIT_DOOR_PROMPT_TIME 3.0f

class CHUDMesh;
class CWeapon;
class CAttack;
class CObjectManager; 
class CHUD;
class CAnimationManager;
class CParticleManager;
class CInventory;
class CWorldMap;
class CCamera;
class CCrafting;
class CStorageCrate;
class CGraveStone;
class CExitKey;
class CDayNight;

class CPlayer :
	public IObject
{
	enum EPlayerAnimation { eIdleAnim, eRunAnim, eHeavyRightAttackAnim, eHeavyLeftAttackAnim, eLightLeftAttackAnim, eLightRightAttackAnim, eDeathAnim, eJumpAnim, eHitAnim, eAnimCount };
	enum EPlayerParticles { eBloodParticle, eBloodString, eBloodMist, eBloodMist2, eSwordParticle, eXSlashParticleLeft, eXSlashParticleRight, eHealthPotionYellow, eHealthPotionGreen, eParticleCount };
	enum EShieldType {eNone, eWoodShield, eMetalShield, eObsidianFlameShield};
	enum EWeaponUnlocks {eSword, eAxe};

	CAnimation* m_vpAnimations;
	EPlayerAnimation m_vpCurrAnim;
	EWeaponUnlocks m_eCurrentWeaponLevel = eSword;
	int m_nCurrentHealthLevel = 0;
	//float m_fStamina = 100.0f;

	CRenderMesh* m_cDropShadow = nullptr;
	
	bool m_bHavenExitPrompt = false;
	CHUDMesh* m_cpTextFrame = nullptr;

	//exit door prompt
	float m_fExitDoorPromptTimer = 0.0f;
	bool m_bExitDoorPrompt = false;
	bool m_bLockSoundPlayed = false;

	vector<CItemPickupAnimation*> m_cAnimations;

	CWeapon * m_cSword = nullptr;
	CWeapon * m_cAxe = nullptr;
	CWeapon * m_cSpear = nullptr;
	CWeapon * m_cCurrentWeapon = nullptr;
	CWeapon * m_cHeavyWeapon = nullptr;
	EShieldType m_eCurrentShield = eWoodShield;

	bool m_bAttacking = false;

	CCamera* m_cCamera = nullptr;
	CInputManager * Input;
	POINT m_MousePrevPos;
	CObjectManager * m_cObjectManager;
	CAnimationManager * m_pAnimationManager = nullptr;
	CParticleManager * m_cpParticleManager = nullptr;
	unsigned int m_unParticles[eParticleCount];
	XMFLOAT3 m_tSpawnPoint;
	float m_fCameraAngle = 0.0f;
	bool m_bIsGrounded = true;
	
	int m_nHealth = 100;
	int m_nMaxHealth = 100;
	bool m_bPlayerAlive = true;
	float m_fRespawnTimer = 1.0f;
	float m_fStunTimer = 1.0f;
	bool m_bIsStunned = false;

	bool m_bStaminaDraught = false;
	float m_fStaminaDraughtTimer = 1.0f;

	bool m_bInvincible = false;
	float m_fInvincibleTimer = 0.0f;
	bool m_bAttackingRight = false;

	bool m_bIsBlocking = false;

	CMesh m_cShieldMesh;
	CRenderMesh * m_cShieldRender = nullptr;
	XMFLOAT4X4 m_f4x4ShieldMatrix;

	XMFLOAT3 m_f3JumpVelocity = { 0.0f, 0.0f, 0.0f };

	float HeavyAttackTimer = 0.0f;
	float LightAttackTimer = 0.0f;
	CAttack * m_cLightAttack = nullptr;
	CAttack * m_cAxeAttack = nullptr;
	CAttack * m_cSpearAttack = nullptr;
	CAttack * m_cCurrentAttack = nullptr;
	float m_fSpearDistance = 0.0f;
	XMFLOAT4X4 m_mSpearLocal;
	CHUD* m_cpTheHUD;

	wstring m_sDamageTexture = L"../Game/Assets/Art/2D/Textures/DamageTexture.dds";
	wstring m_sStunTexture = L"../Game/Assets/Art/2D/Textures/StunTexture.dds";
	wstring m_sPlayerTexture = L"../Game/Assets/Art/2D/Textures/PlayerDiffuse.dds";
	wstring m_sCurrentTexture = L"../Game/Assets/Art/2D/Textures/PlayerDiffuse.dds";

	bool m_bBlinkRed = false;
	float m_fBlinkTimer = 0.25f;

	bool m_bBlinkYellow = false;
	float m_fStunBlinkTimer = 0.25f;

	CInventory* m_cpTheInventory = nullptr;
	CCrafting* m_cpCraftingTable = nullptr;
	CStorageCrate* m_cpStorageCrate = nullptr;

	//unsigned int m_unTombstoneIDs;
	vector<CRenderMesh*> m_vpOldGraves;//vector of all old graves that have been picked up. stored to be deleted at end.
	vector<CGraveStone*> m_vpGraveStones;

	CWorldMap* m_cpMap = nullptr;
	AudioSystemWwise* m_cpAudio;

	CHUDMesh* m_cpControllerDoorPopUp;

	CDayNight* m_cpDayNight = nullptr;
	bool m_bLastNight = false;

	float m_fFriction;

	bool m_bIsBuilding;//for building a bridge
	bool m_bCancelBuild;//for bridge building

	bool m_bExitKeyFound;

	//degub mode 
	bool m_bGodMode = false;

	//Turn False to Disable Input
	bool m_bActiveUpdate = true;

	//total play time
	float m_fTotalPlayTime;

	void DebugModeInput();


	void Animation();

	void AnimationTransition(EPlayerAnimation _from, EPlayerAnimation _to);
	void PlayParticle(unsigned int _toPlay, XMFLOAT3 _relativePos);

	void JoystickTurn();

public:

	CPlayer(CObjectManager * _manager = nullptr, CAnimationManager * _animManager = nullptr);
	~CPlayer();

	void Update() override;

	//void UpdatePosition() override;
	void CollisionResponse(IObject * _cCollidedObject) override;
	void MouseTurn();
	void Movement();
	void TakeDamage(int _damage);
	void HandleDeath();
	void Revive();
	void HeavyAttack();
	void LightAttack(bool isRight = false);
	void Attacking();
	void UpdatePosition();
	void Stun(float _fStunTime = 1.0f);
	void SetCraftingTable(CCrafting* _table);
	const int GetHealth() const { return m_nHealth; }

	bool IsAttackingRight() { return m_bAttackingRight; }
	bool IsAttacking() { return m_bAttacking; }
	const bool IsStunned() const { return m_bIsStunned; }
	CHUD* GetHUD();
	CCamera*& GetCamera() { return m_cCamera; }
	void DamageBlink();
	void StunBlink();
	void BlockLogic();
	void AdjustStamina(float _fStamDam);

	bool UnlockWeapon();//returns false if no upgrade occured
	bool UpgradeHealth();
	bool UpgradeShield(); 

	void SwitchWeapon(CWeapon * _nextWeapon);
	bool& GetActive(){ return m_bActiveUpdate; }
	void BlockHandling(IObject * _enemy);
	CWeapon* GetCurrentWep() {return m_cCurrentWeapon;}
	int GetCurrentWepUpgrade() {return (int)m_eCurrentWeaponLevel;}
	int GetCurrentShi() {return (int)m_eCurrentShield;}
	int GetCurrentHealthLevel() {return m_nCurrentHealthLevel;}
	void SetUpgrades(int _wep, int _shi, int _health) { m_eCurrentWeaponLevel = (EWeaponUnlocks)_wep; m_eCurrentShield = (EShieldType)_shi; m_nCurrentHealthLevel = _health;}
	CWorldMap* GetWorldMap(){ return m_cpMap; }
	CInventory* GetInventory() const {return m_cpTheInventory;}
	CHUD* GetHUD() const {return m_cpTheHUD;}
	
	void SetStorageCrate(CStorageCrate* storageCrate) { m_cpStorageCrate = storageCrate; }
	CStorageCrate* GetStorageCrate() { return m_cpStorageCrate; }
	void SetDayNight(CDayNight* dayNight) { m_cpDayNight = dayNight; }

	float GetTotalPlayTime() { return m_fTotalPlayTime; };
	void SetTotalPlayTime(float fTotalPlayTime) { m_fTotalPlayTime = fTotalPlayTime; }

	void AddToRenderer();

	bool& GetKeyFound() { return m_bExitKeyFound; }

};

#endif