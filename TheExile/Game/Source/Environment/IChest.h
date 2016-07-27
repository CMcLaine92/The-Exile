#pragma once
#include "../../../Engine/Object Manager/IObject.h"
class IChest :
	public IObject
{
public:
	enum EDirection { eNorth, eEast, eSouth, eWest };
	enum EChestType { eWeaponChest, eShieldChest, eHealthChest, eItemChest };

	IChest(EChestType _eType, EDirection _direction = eNorth, XMFLOAT3 _pos = { 0.0f, 0.0f, 0.0f }, string _szTag = "Chest");
	~IChest();

	void Update();
};

