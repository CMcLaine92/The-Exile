#include "Interpolator.h"
#include "Animation.h"

CInterpolator::CInterpolator()
{
	m_fTotalBlendTime = 0.0f;
	m_fCurrBlendTime = 0.0f;
}

CInterpolator::~CInterpolator()
{
	m_vBones.clear();
	m_pCurrAnim = nullptr;
}
const CAnimation* CInterpolator::GetAnimation() const
{
	return m_pCurrAnim;
}
vector<XMFLOAT4X4>& CInterpolator::GetPose()
{
	return m_vBones;

}

XMFLOAT4X4* CInterpolator::GetCurrentBone(int index)
{
	return &m_vBones[index];
}
float CInterpolator::GetTime() const 
{ 
	return m_fCurrBlendTime;
}
void CInterpolator::SetAnimation(const CAnimation* newAnim)
{ 
	m_pCurrAnim = newAnim;
}
void CInterpolator::SetTime(float time)
{ 
	m_fTotalBlendTime = time; 
}
void CInterpolator::Update(float dt)
{
	float duration = m_pCurrAnim->GetDuration();
	const std::vector<tChannel>& tChannels = m_pCurrAnim->GetChannels();

	m_vBones.clear();
	m_fCurrBlendTime += dt;
	if (m_fCurrBlendTime >= duration)
		m_fCurrBlendTime -= duration;
	else if (m_fCurrBlendTime < 0)
		m_fCurrBlendTime += duration;

	unsigned int prevFrame = 0, nextFrame = 0;
	float lerpRatio = 0.0f, tweenTime = 0.0f;

	for (size_t channel = 0; channel < tChannels.size(); channel++)
	{
		for (size_t frame = 0; frame < tChannels[channel].keys.size(); frame++)
		{

			if (tChannels[channel].keys[frame].time > m_fCurrBlendTime)
			{
				nextFrame = frame;
				break;
			}
		}

		if (nextFrame == 0)
		{
			prevFrame = tChannels[channel].keys.size() - 1;
			lerpRatio = m_fCurrBlendTime / tChannels[channel].keys[nextFrame].time;
		}
		else
		{
			prevFrame = nextFrame - 1;
			tweenTime = tChannels[channel].keys[nextFrame].time - tChannels[channel].keys[prevFrame].time;
			lerpRatio = (m_fCurrBlendTime - tChannels[channel].keys[prevFrame].time) / tweenTime;
		}

		m_vBones.push_back(Interpolate(tChannels[channel].keys[prevFrame].node, tChannels[channel].keys[nextFrame].node, lerpRatio));
	}
}

XMFLOAT4X4 CInterpolator::Interpolate(const XMFLOAT4X4 &MatrixA, const XMFLOAT4X4 &MatrixB, float lamda)
{
	XMFLOAT4X4 r;
	XMMATRIX tempA = XMLoadFloat4x4(&MatrixA);
	XMMATRIX tempB = XMLoadFloat4x4(&MatrixB);
	XMMATRIX result = XMLoadFloat4x4(&MatrixB);
	XMMATRIX iMat;
	XMVECTOR det = XMMatrixDeterminant(tempA);
	iMat = XMMatrixInverse(&det, tempA);

	XMMatrixMultiply(result, iMat);

	float trace = 1.0f + iMat.r[0].m128_f32[0] + iMat.r[1].m128_f32[1] + iMat.r[2].m128_f32[2];

	float quatResult[4];

	if (trace > 0.00000001)
	{
		float s = sqrtf(trace) * 2.0f;
		quatResult[0] = (iMat.r[2].m128_f32[1] - iMat.r[1].m128_f32[2]) / s;
		quatResult[1] = (iMat.r[0].m128_f32[2] - iMat.r[2].m128_f32[0]) / s;
		quatResult[2] = (iMat.r[1].m128_f32[0] - iMat.r[0].m128_f32[1]) / s;
		quatResult[3] = 0.25f * s;
	}
	else
	{

		if (iMat.r[0].m128_f32[0] > iMat.r[1].m128_f32[1] && iMat.r[0].m128_f32[0] > iMat.r[2].m128_f32[2])
		{
			float s = sqrtf(1.0f + iMat.r[0].m128_f32[0] - iMat.r[1].m128_f32[1] - iMat.r[2].m128_f32[2]) * 2;
			quatResult[0] = 0.25f * s;
			quatResult[1] = (iMat.r[1].m128_f32[0] + iMat.r[0].m128_f32[1]) / s;
			quatResult[2] = (iMat.r[0].m128_f32[2] + iMat.r[2].m128_f32[0]) / s;
			quatResult[3] = (iMat.r[2].m128_f32[1] - iMat.r[1].m128_f32[2]) / s;
		}
		else if (iMat.r[1].m128_f32[1] > iMat.r[2].m128_f32[2])
		{
			float s = sqrtf(1.0f + iMat.r[1].m128_f32[1] - iMat.r[0].m128_f32[0] - iMat.r[2].m128_f32[2]) * 2;
			quatResult[1] = (iMat.r[1].m128_f32[0] + iMat.r[0].m128_f32[1]) / s;
			quatResult[1] = 0.25f * s;
			quatResult[2] = (iMat.r[2].m128_f32[1] - iMat.r[1].m128_f32[2]) / s;
			quatResult[3] = (iMat.r[0].m128_f32[2] + iMat.r[2].m128_f32[0]) / s;
		}
		else
		{
			float s = sqrtf(1.0f + iMat.r[2].m128_f32[2] - iMat.r[0].m128_f32[0] - iMat.r[1].m128_f32[1]) * 2;
			quatResult[0] = (iMat.r[0].m128_f32[2] + iMat.r[2].m128_f32[0]) / s;
			quatResult[1] = (iMat.r[2].m128_f32[1] - iMat.r[1].m128_f32[2]) / s;
			quatResult[2] = 0.25f * s;
			quatResult[3] = (iMat.r[1].m128_f32[0] + iMat.r[0].m128_f32[1]) / s;
		}
	}

	float quatMagnitude = sqrtf(quatResult[0] * quatResult[0] + quatResult[1] * quatResult[1] + quatResult[2] * quatResult[2] + quatResult[3] * quatResult[3]);
	float quatNormalized[4] = { quatResult[0] / quatMagnitude, quatResult[1] / quatMagnitude, quatResult[2] / quatMagnitude, quatResult[3] / quatMagnitude };

	float cos_a = quatNormalized[3];
	float angle = acos(cos_a) * 2;
	float sin_a = sqrtf(1.0f - cos_a * cos_a);




	if (angle == 0.0)
	{
		result = XMLoadFloat4x4(&MatrixA);


		result.r[3].m128_f32[0] = tempA.r[3].m128_f32[0] + ((tempB.r[3].m128_f32[0] - tempA.r[3].m128_f32[0])*lamda);
		result.r[3].m128_f32[1] = tempA.r[3].m128_f32[1] + ((tempB.r[3].m128_f32[1] - tempA.r[3].m128_f32[1])*lamda);
		result.r[3].m128_f32[2] = tempA.r[3].m128_f32[2] + ((tempB.r[3].m128_f32[2] - tempA.r[3].m128_f32[2])*lamda);

		XMStoreFloat4x4(&r, result);
		return r;
	}
	XMVECTOR axis;

	if (fabs(sin_a) < 0.0005f)
		sin_a = 1.0f;

	axis.m128_f32[0] = quatNormalized[0] / sin_a;
	axis.m128_f32[1] = quatNormalized[1] / sin_a;
	axis.m128_f32[2] = quatNormalized[2] / sin_a;

	angle *= lamda;

	sin_a = sin(angle * 0.5f);
	cos_a = cos(angle * 0.5f);

	quatNormalized[0] = axis.m128_f32[0] * sin_a;
	quatNormalized[1] = axis.m128_f32[1] * sin_a;
	quatNormalized[2] = axis.m128_f32[2] * sin_a;
	quatNormalized[3] = cos_a;

	quatMagnitude = sqrt(quatNormalized[0] * quatNormalized[0] + quatNormalized[1] * quatNormalized[1] + quatNormalized[2] * quatNormalized[2] + quatNormalized[3] * quatNormalized[3]);
	quatNormalized[0] /= quatMagnitude;
	quatNormalized[1] /= quatMagnitude;
	quatNormalized[2] /= quatMagnitude;
	quatNormalized[3] /= quatMagnitude;

	float xx = quatNormalized[0] * quatNormalized[0];
	float xy = quatNormalized[0] * quatNormalized[1];
	float xz = quatNormalized[0] * quatNormalized[2];
	float xw = quatNormalized[0] * quatNormalized[3];
	float yy = quatNormalized[1] * quatNormalized[1];
	float yz = quatNormalized[1] * quatNormalized[2];
	float yw = quatNormalized[1] * quatNormalized[3];
	float zz = quatNormalized[2] * quatNormalized[2];
	float zw = quatNormalized[2] * quatNormalized[3];

	result.r[0].m128_f32[0] = 1 - 2 * (yy + zz);
	result.r[0].m128_f32[1] = 2 * (xy - zw);
	result.r[0].m128_f32[2] = 2 * (xz + yw);
	result.r[1].m128_f32[0] = 2 * (xy + zw);
	result.r[1].m128_f32[1] = 1 - 2 * (xx + zz);
	result.r[1].m128_f32[2] = 2 * (yz - xw);
	result.r[2].m128_f32[0] = 2 * (xz - yw);
	result.r[2].m128_f32[1] = 2 * (yz + xw);
	result.r[2].m128_f32[2] = 1 - 2 * (xx + yy);
	result.r[0].m128_f32[3] = result.r[1].m128_f32[3] = result.r[2].m128_f32[3] = result.r[3].m128_f32[0] = result.r[3].m128_f32[1] = result.r[3].m128_f32[2] = 0;
	result.r[3].m128_f32[3] = 1.0f;

	result = XMMatrixMultiply(result, tempA);

	result.r[3].m128_f32[0] = tempA.r[3].m128_f32[0] + ((tempB.r[3].m128_f32[0] - tempA.r[3].m128_f32[0])*lamda);
	result.r[3].m128_f32[1] = tempA.r[3].m128_f32[1] + ((tempB.r[3].m128_f32[1] - tempA.r[3].m128_f32[1])*lamda);
	result.r[3].m128_f32[2] = tempA.r[3].m128_f32[2] + ((tempB.r[3].m128_f32[2] - tempA.r[3].m128_f32[2])*lamda);


	XMStoreFloat4x4(&r, result);
	return r;
}


