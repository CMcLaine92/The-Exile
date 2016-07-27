#include "AnimationManager.h"
#include "AnimationMesh.h"
#include "Mesh.h"

CAnimationManager::CAnimationManager()
{

}


CAnimationManager::~CAnimationManager()
{
	m_cAnimations.clear();
	m_tBlendingAnimations.clear();
}

void CAnimationManager::Switch(CAnimation * _cCurrAnim, CAnimation * _cToAnim)
{
	RemoveAnimation(_cCurrAnim);
	_cCurrAnim->SetTime(0.0f);
	AddAnimation(_cToAnim);
	_cToAnim->SetTime(0.0f);
}

void CAnimationManager::Blend(CAnimation * _cCurrAnim, CAnimation * _cToAnim, float _fTimeToBlend)
{
	TBlendingAnimation newBlend;
	newBlend.m_cBlendFrom = _cCurrAnim;
	newBlend.m_cBlendTo = _cToAnim;
	newBlend.m_fTotalTime = _fTimeToBlend;
	newBlend.m_fCurrentTime = 0.0f;

	m_tBlendingAnimations.push_back(newBlend);

	AddAnimation(_cToAnim);
	_cToAnim->SetTime(0.0f);
}

void CAnimationManager::AddAnimation(CAnimation * _cAnim)
{
	for (unsigned int i = 0; i < m_cAnimations.size(); i++)
	{
		if (_cAnim == m_cAnimations[i])
			return;
	}

	m_cAnimations.push_back(_cAnim);
}

void CAnimationManager::RemoveAnimation(CAnimation * _cAnim)
{
	if (_cAnim == nullptr)
	{
		return;
	}

	//Looping through all the object in the list, comparing them to the specified object
	auto iter = m_cAnimations.begin();
	while (iter != m_cAnimations.end())
	{

		if ((*iter) == _cAnim)
		{
			CAnimation * temp = (*iter);

			m_cAnimations.erase(iter);

			//SPENCER_TODO - Use the temp to uninitialize if necessary
			return;
		}

		iter++;
	}

	//Return false if the specified object was not in the specified list
	return;
}

void CAnimationManager::Update()
{
	for (unsigned int animation = 0; animation < m_cAnimations.size(); animation++)
	{
		// TODO: Fill out the "pose" vector.  You are responsible for sizing it appropriately.
		//  "channels" contains an array of animation information for each joint.
		//  "current_time" contains our current time.

		CAnimation * cCurrAnim = m_cAnimations[animation];
		cCurrAnim->GetPose().clear();
		vector<tChannel>& cCurrChannels = cCurrAnim->GetChannels();
		cCurrAnim->AddTime(DELTA_TIME() * cCurrAnim->GetScale());
		float current_time = cCurrAnim->GetCurrTime();

		for (unsigned int channelIndex = 0; channelIndex < cCurrChannels.size(); channelIndex++)
		{
			if (cCurrChannels.empty() == false)
			{
				int prevIndex = 0;
				int nextIndex = 0;

				if (cCurrChannels[channelIndex].keys[0].time > current_time)
				{
					prevIndex = cCurrChannels[channelIndex].keys.size() - 1;
					nextIndex = 0;
				}
				else
				{
					while (current_time - cCurrChannels[channelIndex].keys[nextIndex].time >= 0.0f)
					{
						nextIndex++;
					}
					prevIndex = nextIndex - 1;
				}

				float lamda = 0.0f;
				float tween = 0.0f;

				if (nextIndex == 0)
				{
					tween = cCurrChannels[channelIndex].keys[nextIndex].time;
					lamda = current_time / tween;
				}
				else
				{
					tween = cCurrChannels[channelIndex].keys[nextIndex].time - cCurrChannels[channelIndex].keys[prevIndex].time;
					lamda = (current_time - cCurrChannels[channelIndex].keys[prevIndex].time) / tween;
				}

				XMFLOAT4X4 interpValue = interpolate(cCurrChannels[channelIndex].keys[prevIndex].node, cCurrChannels[channelIndex].keys[nextIndex].node, (cCurrChannels[channelIndex].keys[prevIndex].time - cCurrChannels[channelIndex].keys[nextIndex].time), lamda);
				cCurrAnim->GetPose().push_back(interpValue);
			}
			else
				cCurrAnim->GetPose().push_back(cCurrAnim->GetAnimMesh()->GetBones().m_fBone[channelIndex]);
		}


		cCurrAnim->GetAnimMesh()->SetBones(cCurrAnim->GetPose(), cCurrAnim->m_vBindPose);

	}

	Blending();
}

XMFLOAT4X4 CAnimationManager::interpolate(const XMFLOAT4X4 &MatrixA, const XMFLOAT4X4 &MatrixB, float TotalTime, float lamda)
{
	XMMATRIX Matrix1 = XMLoadFloat4x4(&MatrixA);
	XMMATRIX Matrix2 = XMLoadFloat4x4(&MatrixB);

	XMFLOAT4X4 retVal;
	XMStoreFloat4x4(&retVal, (Matrix1 + ((Matrix2 - Matrix1) / TotalTime * (lamda * TotalTime))));
	return retVal;
}

void ConvertToXMFLOAT4X4(matrix4 _mat, XMFLOAT4X4& _newMat)
{
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			_newMat.m[x][y] = _mat.m[x][y];
		}
	}
}

void CAnimationManager::Blending()
{
	vector<TBlendingAnimation *> removals;
	for (unsigned int animation = 0; animation < m_tBlendingAnimations.size(); animation++)
	{
		// TODO: Fill out the "pose" vector.  You are responsible for sizing it appropriately.
		//  "channels" contains an array of animation information for each joint.
		//  "current_time" contains our current time.

		m_tBlendingAnimations[animation].m_fCurrentTime += DELTA_TIME();
		if (m_tBlendingAnimations[animation].m_fCurrentTime > m_tBlendingAnimations[animation].m_fTotalTime)
		{
			RemoveAnimation(m_tBlendingAnimations[animation].m_cBlendFrom);
			m_tBlendingAnimations[animation].m_cBlendFrom->SetTime(0.0f);
			removals.push_back(&m_tBlendingAnimations[animation]);
			continue;
		}

		CAnimation * cCurrAnim = m_tBlendingAnimations[animation].m_cBlendFrom;
		CAnimation * cNextAnim = m_tBlendingAnimations[animation].m_cBlendTo;

		vector<XMFLOAT4X4> newPose;
		float lamda = m_tBlendingAnimations[animation].m_fCurrentTime / m_tBlendingAnimations[animation].m_fTotalTime;
		for (unsigned int bone = 0; bone < cCurrAnim->GetPose().size(); bone++)
		{
			XMFLOAT4X4 interpValue = interpolate(cCurrAnim->GetPose()[bone], cNextAnim->GetPose()[bone], m_tBlendingAnimations[animation].m_fTotalTime, lamda);
			newPose.push_back(interpValue);
		}

		cNextAnim->GetPose() = newPose;
		cNextAnim->GetAnimMesh()->SetBones(cNextAnim->GetPose(), cNextAnim->m_vBindPose);
	}

	for (unsigned int remove = 0; remove < removals.size(); remove++)
	{
		EndBlend(removals[remove]);
	}
}

void CAnimationManager::EndBlend(TBlendingAnimation * _blender)
{
	if (_blender == nullptr)
	{
		return;
	}

	//Looping through all the object in the list, comparing them to the specified object
	auto iter = m_tBlendingAnimations.begin();
	while (iter != m_tBlendingAnimations.end())
	{

		if (&(*iter) == _blender)
		{
			TBlendingAnimation * temp = &(*iter);

			m_tBlendingAnimations.erase(iter);

			//SPENCER_TODO - Use the temp to uninitialize if necessary
			return;
		}

		iter++;
	}

	//Return false if the specified object was not in the specified list
	return;
}
