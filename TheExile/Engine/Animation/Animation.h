#pragma once
#include <unordered_map>
#include "Mesh.h"

class CAnimationMesh;


struct tKey
{
	float time;
	XMFLOAT4X4 node;
};

struct tKeyFrame
{
	std::vector< tKey> nodes;
};

struct tChannel
{
	std::vector< tKey > keys;
};

class CAnimation
{
	vector< tChannel > m_vtChannels;       //Only using one of these 2

	string name;

	vector<XMFLOAT4X4> m_vmatCurrentPose;


	float m_fCurrentTime = 0.0f;
	
	CAnimationMesh * m_cAnimMesh;

	float m_fSpeedScale = 1.0f;

	bool m_bLooping = false;

public:
	CAnimation();
	~CAnimation();

	std::string& GetName();
	const std::string& GetName() const;

	std::vector< tChannel >& GetChannels();
	const std::vector< tChannel >& GetChannels() const;

	vector<XMFLOAT4X4>& GetPose();
	const vector<XMFLOAT4X4>& GetPose() const;

	CAnimationMesh * GetAnimMesh();
	void CAnimation::SetAnimMesh(CAnimationMesh * _newMesh);

	float GetDuration() const;
	float GetCurrTime() const;

	void AddTime(float time);
	void SetTime(float time);

	void SetScale(float _scale) { m_fSpeedScale = _scale; }
	float GetScale() { return m_fSpeedScale; }

	void SetLooping(bool _loop) { m_bLooping = _loop; }

	void IntializeAnimation();
	XMFLOAT4X4 interpolate(const XMFLOAT4X4 &MatrixA, const XMFLOAT4X4 &MatrixB, float TotalTime, float lamda);


	void BinaryRead(const char* filePath);
	void BinaryWrite(const char* filePath);

	vector<Joint> m_vBindPose;
	

};



