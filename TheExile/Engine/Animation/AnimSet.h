#ifndef _ANIM_SET_H_
#define _ANIM_SET_H_
#include "Animation.h"

class CRig;

class CAnimSet
{

public:
	CAnimSet();
	~CAnimSet();
	const CAnimation* GetAnimation(int index);
	const CAnimation* GetDefaultAnim();
	void AddRig(CRig* _rig);
	bool LoadFromXML(const char* filePath);

private:
	
	vector<CAnimation> m_cAnimations;
	CRig* m_pRig;


};


#endif