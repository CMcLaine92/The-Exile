#include "Animation.h"
#include "AnimationMesh.h"
#include <fstream>

CAnimation::CAnimation()
{

}

//CAnimation::CAnimation(const CAnimation& other)
//{
//	if (this != &other)
//	{
//		*this = other;
//	}
//}
//const CAnimation&  CAnimation::operator=(const CAnimation& other)
//{
//	unsigned int newSize = (unsigned int)other.GetChannels().size();
//	unsigned int numKeys = 0;
//
//	this->m_vtChannels.resize(newSize);
//	this->name = other.GetName();
//
//	
//	for (unsigned int currChan = 0; currChan < newSize; currChan++)
//	{
//		numKeys = (unsigned int)other.GetChannels()[currChan].keys.size();
//		this->m_vtChannels[currChan].keys.resize(numKeys);
//
//		for (unsigned int currKey = 0; currKey < numKeys; currKey++)
//		{
//			this->m_vtChannels[currChan].keys[currKey].node = other.GetChannels()[currChan].keys[currKey].node;
//			this->m_vtChannels[currChan].keys[currKey].time = other.GetChannels()[currChan].keys[currKey].time;
//		}
//
//	}
//
//	return *this;
//
//}

CAnimation::~CAnimation()
{
}

std::string& CAnimation::GetName()
{
	return name;
}

const std::string& CAnimation::GetName() const
{
	return name;
}

std::vector< tChannel >& CAnimation::GetChannels()
{
	return m_vtChannels;
}

const std::vector< tChannel >& CAnimation::GetChannels() const
{
	return m_vtChannels;
}

vector<XMFLOAT4X4>& CAnimation::GetPose()
{
	return m_vmatCurrentPose;
}

const vector<XMFLOAT4X4>& CAnimation::GetPose() const
{
	return m_vmatCurrentPose;
}

float CAnimation::GetDuration() const
{

	return m_vtChannels.back().keys.back().time;
}

float CAnimation::GetCurrTime() const
{
	return m_fCurrentTime;
}

void CAnimation::AddTime(float time)
{
	SetTime(m_fCurrentTime + time);
}

void CAnimation::SetTime(float time)
{
	float duration = GetDuration();

	m_fCurrentTime = time;
	if (duration == 0.0f)
		return;
	while (m_fCurrentTime < 0.0f)
	{
		if (m_bLooping)
			m_fCurrentTime += duration;
		else
			m_fCurrentTime = duration;
	}

	while (m_fCurrentTime > duration)
	{
		if (m_bLooping)
			m_fCurrentTime -= duration;
		else
			m_fCurrentTime = 0.0f;
	}
}

CAnimationMesh * CAnimation::GetAnimMesh()
{
	return m_cAnimMesh;
}

void CAnimation::SetAnimMesh(CAnimationMesh * _newMesh)
{
	m_cAnimMesh = _newMesh;
}

void CAnimation::IntializeAnimation()
{
	GetPose().clear();
	for (unsigned int channelIndex = 0; channelIndex < GetChannels().size(); channelIndex++)
	{
		if (GetChannels()[channelIndex].keys.size())
			GetPose().push_back(GetChannels()[channelIndex].keys[1].node);
	}
	m_cAnimMesh->SetBones(GetPose(), m_vBindPose);
}

XMFLOAT4X4 CAnimation::interpolate(const XMFLOAT4X4 &MatrixA, const XMFLOAT4X4 &MatrixB, float TotalTime, float lamda)
{
	XMMATRIX Matrix1 = XMLoadFloat4x4(&MatrixA);
	XMMATRIX Matrix2 = XMLoadFloat4x4(&MatrixB);

	XMFLOAT4X4 retVal;
	XMStoreFloat4x4(&retVal, (Matrix1 + ((Matrix2 - Matrix1) / TotalTime * (lamda * TotalTime))));
	return retVal;
}

void CAnimation::BinaryRead(const char* filePath)
{
std:ifstream file(filePath, std::ios_base::binary);


	if (file.is_open())
	{
		unsigned int chNameSize = 0;
		char * nameOut = nullptr;
		unsigned int numChannels = 0;
		unsigned int numKeys = 0;

		file.read((char*)&chNameSize, sizeof(unsigned int));

		nameOut = new char[chNameSize + 1];

		file.read(nameOut, chNameSize);
		nameOut[chNameSize] = '\0';
		name = nameOut;

		file.read((char*)&numChannels, sizeof(unsigned int));
		m_vtChannels.resize(numChannels);
		for (unsigned int currChan = 0; currChan < numChannels; currChan++)
		{
			file.read((char*)&numKeys, sizeof(unsigned int));
			m_vtChannels[currChan].keys.resize(numKeys);
			for (unsigned int currKey = 0; currKey < numKeys; currKey++)
			{
				XMFLOAT4X4 nodeIn;
				XMStoreFloat4x4(&nodeIn, XMMatrixIdentity());
				float timeIn = 0;

				file.read((char*)&nodeIn, sizeof(XMFLOAT4X4));
				file.read((char*)&timeIn, sizeof(float));
				m_vtChannels[currChan].keys[currKey].node = nodeIn;
				m_vtChannels[currChan].keys[currKey].time = timeIn;
			}
		}

		int m_nNumBones = 0;
		file.read((char*)&m_nNumBones, sizeof(int));
		m_vBindPose.resize(m_nNumBones);
		for (int i = 0; i < m_nNumBones; i++)
		{
			char * jointName;
			unsigned int numChildren;
			unsigned int parent;
			XMFLOAT4X4 bindPose;
			unsigned int jointNameSize;

			file.read((char*)&jointNameSize, sizeof(unsigned int));

			jointName = new char[jointNameSize + 1];
			file.read((char*)jointName, jointNameSize);
			jointName[jointNameSize] = '\0';
			m_vBindPose[i].name = jointName;
			delete[] jointName;
			file.read((char*)&numChildren, sizeof(unsigned int));


			m_vBindPose[i].children.resize(numChildren);
			for (size_t child = 0; child < numChildren; child++)
				file.read((char*)&m_vBindPose[i].children[child], sizeof(unsigned int));

			file.read((char*)&m_vBindPose[i].parent, sizeof(unsigned int));

			file.read((char*)&m_vBindPose[i].bind_pose_transform, sizeof(float) * 16);
		}


		delete[] nameOut;
		nameOut = nullptr;
		file.close();
	}
}
void CAnimation::BinaryWrite(const char* filePath)
{


std:ofstream file(filePath, std::ios_base::binary);


	if (file.is_open())
	{
		unsigned int chNameSize = name.size();
		const char * nameOut = name.c_str();
		unsigned int numChannels = m_vtChannels.size();
		unsigned int numKeys = 0;

		file.write((char*)&chNameSize, sizeof(unsigned int));

		file.write(nameOut, chNameSize);

		file.write((char*)&numChannels, sizeof(unsigned int));

		for (unsigned int currChan = 0; currChan < numChannels; currChan++)
		{
			numKeys = m_vtChannels[currChan].keys.size();
			file.write((char*)&numKeys, sizeof(unsigned int));

			for (unsigned int currKey = 0; currKey < numKeys; currKey++)
			{
				XMFLOAT4X4 node = m_vtChannels[currChan].keys[currKey].node;
				float time = m_vtChannels[currChan].keys[currKey].time;

				file.write((char*)&node, sizeof(XMFLOAT4X4));
				file.write((char*)&time, sizeof(float));
			}
		}

		unsigned int numJoints = m_vBindPose.size();
		file.write((char*)&numJoints, sizeof(unsigned int));

		//10: Joints
		for (unsigned int joint = 0; joint < numJoints; joint++)
		{
			const char * jointName = m_vBindPose[joint].name.c_str();
			unsigned int numChildren = m_vBindPose[joint].children.size();
			unsigned int parent = m_vBindPose[joint].parent;
			XMFLOAT4X4 bindPose = m_vBindPose[joint].bind_pose_transform;
			unsigned int jointNameSize = m_vBindPose[joint].name.size();

			//10 A: Save out size of name - unsigned int
			file.write((char*)&jointNameSize, sizeof(unsigned int));

			//10 B: Save out joint name - string
			file.write((char*)jointName, m_vBindPose[joint].name.size());

			//10 C: Save out number of children - unsigned int
			file.write((char*)&numChildren, sizeof(unsigned int));

			//10 D: Save out joint children indices - vector<unsigned int>
			file.write((char*)& m_vBindPose[joint].children._Myfirst, sizeof(unsigned int)* m_vBindPose[joint].children.size());

			//10 E: Save out joint parent - unsigned int
			file.write((char*)& m_vBindPose[joint].parent, sizeof(unsigned int));

			//10 F: Save out bind pose - float[16]
			file.write((char*)& m_vBindPose[joint].bind_pose_transform, sizeof(float) * 16);
		}

			file.close();
	}
	//unsigned int numChannels = _saveAnim->GetChannels().size();

	////12 A: Number of Channels - unsigned int
	//fout.write((char*)&numChannels, sizeof(unsigned int));

	//for (unsigned int channel = 0; channel < numChannels; channel++)
	//{
	//	unsigned int numKeys = _saveAnim->GetChannels()[channel].keys.size();

	//	//12 B: Number of keys - unsigned int
	//	fout.write((char*)&numKeys, sizeof(unsigned int));
	//	for (unsigned int key = 0; key < numKeys; key++)
	//	{
	//		XMFLOAT4X4 node = _saveAnim->GetChannels()[channel].keys[key].node;
	//		float time = _saveAnim->GetChannels()[channel].keys[key].time;

	//		//12 C: Key matrix - XMFLOAT4X4
	//		fout.write((char*)&node, sizeof(XMFLOAT4X4));

	//		//12 D: Key time - float
	//		fout.write((char*)&time, sizeof(float));
	//	}
	//}

	


}