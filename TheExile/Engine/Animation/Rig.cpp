#include "Rig.h"
#include <fstream>
CRig::CRig()
{
	

}
CRig::CRig(const char* path)
{
	LoadRig(path);
}

void LoadRig(const char* path);

CRig::~CRig()
{
	joints.clear();
}

const vector<Joint>& CRig::GetBindPose() const
{
	return joints;
}
const int CRig::GetNumBones() const
{
	return (int)joints.size();
}
void CRig::LoadRig(const char* path)
{
	ifstream file(path, ios_base::binary);
	if (file.is_open())
	{
		int m_nNumBones = 0;
		file.read((char*)&m_nNumBones, sizeof(int));
		joints.resize(m_nNumBones);
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
			joints[i].name = jointName;
			delete[] jointName;
			file.read((char*)&numChildren, sizeof(unsigned int));
			

			joints[i].children.resize(numChildren);
			for (size_t child = 0; child < numChildren; child++)
				file.read((char*)&joints[i].children[child], sizeof(unsigned int));

			file.read((char*)&joints[i].parent, sizeof(unsigned int));

			file.read((char*)&joints[i].bind_pose_transform, sizeof(float) * 16);
		}


		file.close();
	}
}