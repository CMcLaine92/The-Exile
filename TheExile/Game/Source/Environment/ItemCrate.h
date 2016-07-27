#ifndef _ITEM_CRATE_H_
#define _ITEM_CRATE_H_

#include "IChest.h"

class CDayNight;
class CItemCrate : public IChest
{
public:
	CItemCrate(EDirection _direction = eNorth, XMFLOAT3 _pos = { 0.0f, 0.0f, 0.0f }, CDayNight* dayNight = nullptr, string _szTag = "ItemChest");

	~CItemCrate();

	void SetActive(bool active);

	virtual void Update() override;



private:

	CDayNight* m_pDayNight;
	float m_fInactiveTimer;
	bool m_bActive;

};




#endif