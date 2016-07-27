#ifndef _ANIMATOR_H_
#define _ANIMATOR_H_
#include "Interpolator.h"

class CAnimationMesh;
class CRig;
class CAnimSet;

enum EBlendType {CUR_GO_NEXT_GO, CUR_GO_NEXT_STOP, CURR_STOP_NEXT_GO, CURR_STOP_NEXT_STOP, DEFAULT, POP_NEXT};

class CAnimator
{
public:
	CAnimator();
	~CAnimator();

	void SetAnimSet(string animKey);
	void TransitionTo(EBlendType blendType, int AnimKey, float transitionTime = 0.0f);
	void SetAnimMesh(CAnimationMesh* _animMesh);
	void Update(float dt);
	bool InTransition(void);


private:
	
	CInterpolator m_cInterpolators[2];
	CAnimSet* m_pAnimSet;
	CRig* m_pRig;
	CAnimationMesh* m_cpMesh;
	float m_fCurrBlendTime;
	float m_fTotalBlendTime;
	int m_nAnimSetKey;
	int m_nCurrInterp;
	EBlendType m_eBlendType;




};




#endif