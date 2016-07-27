#ifndef _RIG_H_
#define _RIG_H_
#include "Mesh.h"

class CRig
{
	vector<Joint> joints;

public:

	CRig();
	CRig(const char* path);

	~CRig();

	const vector<Joint>& GetBindPose() const;
	const int GetNumBones() const;
	void LoadRig(const char* path);

};


#endif