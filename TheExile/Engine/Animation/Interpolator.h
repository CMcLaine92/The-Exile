
#ifndef _INTERP_H_
#define _INTERP_H_



class CAnimation;

class CInterpolator
{

private:

	const CAnimation* m_pCurrAnim;
	int m_nCurrFrame;
	float m_fCurrBlendTime;
	float m_fTotalBlendTime;
	vector<XMFLOAT4X4> m_vBones;


public:

	CInterpolator();
	~CInterpolator();
	vector<XMFLOAT4X4>& GetPose();
	const CAnimation* GetAnimation() const;
	XMFLOAT4X4* GetCurrentBone(int index);
	float GetTime() const; 
	void SetAnimation(const CAnimation* newAnim);
	void SetTime(float time);
	void Update(float dt);

	static XMFLOAT4X4 Interpolate(const XMFLOAT4X4 &MatrixA, const XMFLOAT4X4 &MatrixB, float lamda);

};


#endif