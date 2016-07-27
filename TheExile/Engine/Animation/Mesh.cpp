#include "Mesh.h"
#include <fstream>
CMesh::CMesh()
{
}

CMesh::~CMesh()
{
	vertices.clear();
	m_tVertices.clear();
	m_tAnimVertices.clear();
	indices.clear();
	joints.clear();
	influences.clear();
	texture_names.clear();

}

std::string& CMesh::GetName()
{
	return name;
}

std::vector< CMesh::UniqueVertex >& CMesh::GetUniqueVertices()
{
	return vertices;
}

std::vector< TVertex >& CMesh::GetVertices()
{
	return m_tVertices;
}

vector< TAnimVertex >& CMesh::GetAnimVertices()
{
	return m_tAnimVertices;
}

std::vector< unsigned int >& CMesh::GetIndices()
{
	return indices;
}

std::vector< Joint >& CMesh::GetJoints()
{
	return joints;
}

std::vector< std::vector< CMesh::JointInfluence >>& CMesh::GetInfluences()
{
	return influences;
}

std::vector< std::string >& CMesh::GetTextureNames()
{
	return texture_names;
}

XMFLOAT4X4& CMesh::GetLocalMatrix()
{
	return localMatrix;
}

XMFLOAT3 CMesh::GetLocalPosition()
{
	return{ localMatrix._41, localMatrix._42, localMatrix._43 };
}
void CMesh::BuildPlane(TPlane& plane)
{
	float f3Min[3] =
	{
		FLT_MAX,
		FLT_MAX,
		FLT_MAX
	};
	float f3Max[3] =
	{
		-FLT_MAX,
		-FLT_MAX,
		-FLT_MAX
	};
	float fDistance;
	XMFLOAT3 f3Normal;
	XMFLOAT3 f3PointOnPlane;


	if (GetAnimVertices().size() > 0)
	{
		f3Normal.x = GetAnimVertices()[0].m_fNormal[0];
		f3Normal.y = GetAnimVertices()[0].m_fNormal[1];
		f3Normal.z = GetAnimVertices()[0].m_fNormal[2];

		f3PointOnPlane = XMFLOAT3(GetAnimVertices()[0].m_fPosition[0], GetAnimVertices()[0].m_fPosition[1], GetAnimVertices()[0].m_fPosition[2]);

		fDistance = XMVector3Dot(XMLoadFloat3(&f3Normal), XMLoadFloat3(&f3PointOnPlane)).m128_f32[0];


		for (unsigned int VertIndex = 0; VertIndex < GetAnimVertices().size(); VertIndex++)
		{
			for (unsigned int xyz = 0; xyz < 3; xyz++)
			{
				if (GetAnimVertices()[VertIndex].m_fPosition[xyz] < f3Min[xyz])
					f3Min[xyz] = GetAnimVertices()[VertIndex].m_fPosition[xyz];
				if (GetAnimVertices()[VertIndex].m_fPosition[xyz] > f3Max[xyz])
					f3Max[xyz] = GetAnimVertices()[VertIndex].m_fPosition[xyz];
			}
		}
	}
	else
	{

		f3Normal.x = GetVertices()[0].m_fNormal[0];
		f3Normal.y = GetVertices()[0].m_fNormal[1];
		f3Normal.z = GetVertices()[0].m_fNormal[2];

		f3PointOnPlane = XMFLOAT3(GetVertices()[0].m_fPosition[0], GetVertices()[0].m_fPosition[1], GetVertices()[0].m_fPosition[2]);

		fDistance = XMVector3Dot(XMLoadFloat3(&f3Normal), XMLoadFloat3(&f3PointOnPlane)).m128_f32[0];


		for (unsigned int VertIndex = 0; VertIndex < GetVertices().size(); VertIndex++)
		{
			for (unsigned int xyz = 0; xyz < 3; xyz++)
			{
				if (GetVertices()[VertIndex].m_fPosition[xyz] < f3Min[xyz])
					f3Min[xyz] = GetVertices()[VertIndex].m_fPosition[xyz];
				if (GetVertices()[VertIndex].m_fPosition[xyz] > f3Max[xyz])
					f3Max[xyz] = GetVertices()[VertIndex].m_fPosition[xyz];
			}
		}

	}

	XMFLOAT3 f3Center;
	XMFLOAT3 f3Extents;

	f3Center.x = (f3Min[0] + f3Max[0]) / 2.0f;
	f3Center.y = (f3Min[1] + f3Max[1]) / 2.0f;
	f3Center.z = (f3Min[2] + f3Max[2]) / 2.0f;

	f3Extents.x = f3Max[0] - f3Center.x;
	f3Extents.y = f3Max[1] - f3Center.y;
	f3Extents.z = f3Max[2] - f3Center.z;



	plane.normal = f3Normal;
	plane.center = f3Center;
	plane.extents = f3Extents;
	plane.distance = fDistance;
}
void CMesh::BuildAABB(TAABB& aabb)
{

	XMFLOAT3 min = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
	XMFLOAT3 max = XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	if (GetAnimVertices().size() > 0)
	{
		for (unsigned int i = 0; i < GetAnimVertices().size(); i++)
		{
			if (GetAnimVertices()[i].m_fPosition[0] < min.x)
				min.x = GetAnimVertices()[i].m_fPosition[0];
			if (GetAnimVertices()[i].m_fPosition[1] < min.y)
				min.y = GetAnimVertices()[i].m_fPosition[1];
			if (GetAnimVertices()[i].m_fPosition[2] < min.z)
				min.z = GetAnimVertices()[i].m_fPosition[2];

			if (GetAnimVertices()[i].m_fPosition[0] > max.x)
				max.x = GetAnimVertices()[i].m_fPosition[0];
			if (GetAnimVertices()[i].m_fPosition[1] > max.y)
				max.y = GetAnimVertices()[i].m_fPosition[1];
			if (GetAnimVertices()[i].m_fPosition[2] > max.z)
				max.z = GetAnimVertices()[i].m_fPosition[2];

		}

	}
	else
	{
		for (unsigned int i = 0; i < GetVertices().size(); i++)
		{
			if (GetVertices()[i].m_fPosition[0] < min.x)
				min.x = GetVertices()[i].m_fPosition[0];
			if (GetVertices()[i].m_fPosition[1] < min.y)
				min.y = GetVertices()[i].m_fPosition[1];
			if (GetVertices()[i].m_fPosition[2] < min.z)
				min.z = GetVertices()[i].m_fPosition[2];

			if (GetVertices()[i].m_fPosition[0] > max.x)
				max.x = GetVertices()[i].m_fPosition[0];
			if (GetVertices()[i].m_fPosition[1] > max.y)
				max.y = GetVertices()[i].m_fPosition[1];
			if (GetVertices()[i].m_fPosition[2] > max.z)
				max.z = GetVertices()[i].m_fPosition[2];

		}
	}

	aabb.center.x = (min.x + max.x) / 2.0f;
	aabb.center.y = (min.y + max.y) / 2.0f;
	aabb.center.z = (min.z + max.z) / 2.0f;

	aabb.extents.x = max.x - aabb.center.x;
	aabb.extents.y = max.y - aabb.center.y;
	aabb.extents.z = max.z - aabb.center.z;


}

void CMesh::ConvertVertices(bool animMesh)
{

	for (unsigned int vert = 0; vert < vertices.size(); vert++)
	{
		if (influences.size() > 0)
		{
			TAnimVertex tempAnim;
			tempAnim.m_fPosition[0] = vertices[vert].position.x;
			tempAnim.m_fPosition[1] = vertices[vert].position.y;
			tempAnim.m_fPosition[2] = vertices[vert].position.z;
			tempAnim.m_fPosition[3] = 1;
			tempAnim.m_fNormal[0] = vertices[vert].normal.x;
			tempAnim.m_fNormal[1] = vertices[vert].normal.y;
			tempAnim.m_fNormal[2] = vertices[vert].normal.z;
			tempAnim.m_fNormal[3] = 1;
			tempAnim.m_fTexCoord[0] = vertices[vert].tex_coord.u;
			tempAnim.m_fTexCoord[1] = vertices[vert].tex_coord.v;
			tempAnim.m_fTexCoord[2] = 0.0f;
			tempAnim.m_fTexCoord[3] = 0.0f;
			if (influences.size() > 0)
			{
				for (unsigned int i = 0; i < 4; i++)
				{
					tempAnim.m_fBoneIndex[i] = 0;
					tempAnim.m_fBoneWeight[i] = 0.0f;
				}

				for (unsigned int jointInfluence = 0; jointInfluence < influences[vert].size(); jointInfluence++)
				{
					tempAnim.m_fBoneIndex[jointInfluence] = influences[vert][jointInfluence].joint_index;
					tempAnim.m_fBoneWeight[jointInfluence] = influences[vert][jointInfluence].weight;
				}
			}

			m_tAnimVertices.push_back(tempAnim);
		}
		else
		{
			TVertex temp;

			//Positions
			temp.m_fPosition[0] = vertices[vert].position.x;
			temp.m_fPosition[1] = vertices[vert].position.y;
			temp.m_fPosition[2] = vertices[vert].position.z;
			temp.m_fPosition[3] = 1;

			//Normals
			temp.m_fNormal[0] = vertices[vert].normal.x;
			temp.m_fNormal[1] = vertices[vert].normal.y;
			temp.m_fNormal[2] = vertices[vert].normal.z;
			temp.m_fNormal[3] = 1;

			//Texture Coordinates
			temp.m_fTexCoord[0] = vertices[vert].tex_coord.u;
			temp.m_fTexCoord[1] = vertices[vert].tex_coord.v;
			temp.m_fTexCoord[2] = 0.0f;
			temp.m_fTexCoord[3] = 0.0f;

			m_tVertices.push_back(temp);
		}

	}
	influences.clear();
	vertices.clear();
	if (m_tAnimVertices.size() > 0)
		CalculateTangentVectors(m_tAnimVertices, indices);
	else
		CalculateTangentVectors(m_tVertices, indices);



}

void CMesh::CalculateTangentVectors(std::vector<TVertex>& _vertsvec, std::vector<unsigned int> & _indexvec)
{
	//Loosely Based Off of Code available at http://www.terathon.com/code/tangent.html 
	//Credit goes to
	//Lengyel, Eric. “Computing Tangent Space Basis Vectors for an Arbitrary Mesh”. Terathon Software 3D Graphics Library, 2001. http://www.terathon.com/code/tangent.html


	std::vector<XMFLOAT4> tan1;
	std::vector<XMFLOAT4> tan2;
	XMFLOAT4 temp = { 0, 0, 0, 0 };
	for (unsigned int i = 0; i < _vertsvec.size(); i++)
	{
		tan1.push_back(temp);
		tan2.push_back(temp);

	}

	for (unsigned int index = 0; index < _indexvec.size(); index += 3)
	{
		float xyz1[3];
		float xyz2[3];
		float st1[2];
		float st2[2];

		TVertex& _v1 = _vertsvec[_indexvec[index]];
		TVertex& _v2 = _vertsvec[_indexvec[index + 1]];
		TVertex& _v3 = _vertsvec[_indexvec[index + 2]];

		for (unsigned int i = 0; i < 3; i++)
		{
			xyz1[i] = _v2.m_fPosition[i] - _v1.m_fPosition[i];
			xyz2[i] = _v3.m_fPosition[i] - _v1.m_fPosition[i];
		}
		for (unsigned int i = 0; i < 2; i++)
		{
			st1[i] = _v2.m_fTexCoord[i] - _v1.m_fTexCoord[i];
			st2[i] = _v3.m_fTexCoord[i] - _v1.m_fTexCoord[i];
		}

		float r = 1.0f / (float)((st1[0] * st2[1]) - (st2[0] * st1[1]));
		XMVECTOR sdir = { (st2[1] * xyz1[0] - st1[1] * xyz2[0]) * r,
			(st2[1] * xyz1[1] - st1[1] * xyz2[1]) * r,
			(st2[1] * xyz1[2] - st1[1] * xyz2[2]) * r };

		XMVECTOR tdir = { (st1[0] * xyz2[0] - st2[0] * xyz1[0]) * r,
			(st1[0] * xyz2[1] - st2[0] * xyz1[1]) * r,
			(st1[0] * xyz2[2] - st2[0] * xyz1[2]) * r };

		// sdir = XMVector3Normalize(sdir);
		// tdir = XMVector3Normalize(tdir);


		tan1[_indexvec[index]].x += sdir.m128_f32[0];
		tan1[_indexvec[index]].y += sdir.m128_f32[1];
		tan1[_indexvec[index]].z += sdir.m128_f32[2];

		tan2[_indexvec[index]].x += tdir.m128_f32[0];
		tan2[_indexvec[index]].y += tdir.m128_f32[1];
		tan2[_indexvec[index]].z += tdir.m128_f32[2];


		tan1[_indexvec[index + 1]].x += sdir.m128_f32[0];
		tan1[_indexvec[index + 1]].y += sdir.m128_f32[1];
		tan1[_indexvec[index + 1]].z += sdir.m128_f32[2];

		tan2[_indexvec[index + 1]].x += tdir.m128_f32[0];
		tan2[_indexvec[index + 1]].y += tdir.m128_f32[1];
		tan2[_indexvec[index + 1]].z += tdir.m128_f32[2];


		tan1[_indexvec[index + 2]].x += sdir.m128_f32[0];
		tan1[_indexvec[index + 2]].y += sdir.m128_f32[1];
		tan1[_indexvec[index + 2]].z += sdir.m128_f32[2];

		tan2[_indexvec[index + 2]].x += tdir.m128_f32[0];
		tan2[_indexvec[index + 2]].y += tdir.m128_f32[1];
		tan2[_indexvec[index + 2]].z += tdir.m128_f32[2];

	}
	for (unsigned int index = 0; index < _vertsvec.size(); index++)
	{
		XMVECTOR n;
		XMVECTOR t;
		t.m128_f32[0] = tan1[index].x;
		t.m128_f32[1] = tan1[index].y;
		t.m128_f32[2] = tan1[index].z;
		t.m128_f32[3] = tan1[index].w;

		XMVECTOR tang;
		for (unsigned int a = 0; a < 4; a++)
		{
			n.m128_f32[a] = _vertsvec[index].m_fNormal[a];
		}

		n = XMVector3Normalize(n);
		t = XMVector3Normalize(t);
		//Gram-Schmidt orthogonalize
		tang = (t - n * XMVector3Dot(n, t));
		tang = XMVector3Normalize(tang);


		XMVECTOR t2;
		t2.m128_f32[0] = tan2[index].x;
		t2.m128_f32[1] = tan2[index].y;
		t2.m128_f32[2] = tan2[index].z;


		//calc handedness
		tang.m128_f32[3] = (XMVector3Dot(XMVector3Cross(n, t), t2)).m128_f32[0] < 0.0f ? 1.0f : -1.0f;
		for (unsigned int i = 0; i < 4; i++)
		{
			_vertsvec[index].m_fTangent[i] = tang.m128_f32[i];

		}
	}
}

void CMesh::CalculateTangentVectors(std::vector<TAnimVertex>& _vertsvec, std::vector<unsigned int> & _indexvec)
{
	//Loosely Based Off of Code available at http://www.terathon.com/code/tangent.html 
	//Credit goes to
	//Lengyel, Eric. “Computing Tangent Space Basis Vectors for an Arbitrary Mesh”. Terathon Software 3D Graphics Library, 2001. http://www.terathon.com/code/tangent.html


	std::vector<XMFLOAT4> tan1;
	std::vector<XMFLOAT4> tan2;
	XMFLOAT4 temp = { 0, 0, 0, 0 };
	for (unsigned int i = 0; i < _vertsvec.size(); i++)
	{
		tan1.push_back(temp);
		tan2.push_back(temp);

	}

	for (unsigned int index = 0; index < _indexvec.size(); index += 3)
	{
		float xyz1[3];
		float xyz2[3];
		float st1[2];
		float st2[2];

		TAnimVertex& _v1 = _vertsvec[_indexvec[index]];
		TAnimVertex& _v2 = _vertsvec[_indexvec[index + 1]];
		TAnimVertex& _v3 = _vertsvec[_indexvec[index + 2]];

		for (unsigned int i = 0; i < 3; i++)
		{
			xyz1[i] = _v2.m_fPosition[i] - _v1.m_fPosition[i];
			xyz2[i] = _v3.m_fPosition[i] - _v1.m_fPosition[i];
		}
		for (unsigned int i = 0; i < 2; i++)
		{
			st1[i] = _v2.m_fTexCoord[i] - _v1.m_fTexCoord[i];
			st2[i] = _v3.m_fTexCoord[i] - _v1.m_fTexCoord[i];
		}

		float r = 1.0f / (float)((st1[0] * st2[1]) - (st2[0] * st1[1]));
		XMVECTOR sdir = { (st2[1] * xyz1[0] - st1[1] * xyz2[0]) * r,
			(st2[1] * xyz1[1] - st1[1] * xyz2[1]) * r,
			(st2[1] * xyz1[2] - st1[1] * xyz2[2]) * r };

		XMVECTOR tdir = { (st1[0] * xyz2[0] - st2[0] * xyz1[0]) * r,
			(st1[0] * xyz2[1] - st2[0] * xyz1[1]) * r,
			(st1[0] * xyz2[2] - st2[0] * xyz1[2]) * r };

		// sdir = XMVector3Normalize(sdir);
		// tdir = XMVector3Normalize(tdir);


		tan1[_indexvec[index]].x += sdir.m128_f32[0];
		tan1[_indexvec[index]].y += sdir.m128_f32[1];
		tan1[_indexvec[index]].z += sdir.m128_f32[2];

		tan2[_indexvec[index]].x += tdir.m128_f32[0];
		tan2[_indexvec[index]].y += tdir.m128_f32[1];
		tan2[_indexvec[index]].z += tdir.m128_f32[2];


		tan1[_indexvec[index + 1]].x += sdir.m128_f32[0];
		tan1[_indexvec[index + 1]].y += sdir.m128_f32[1];
		tan1[_indexvec[index + 1]].z += sdir.m128_f32[2];

		tan2[_indexvec[index + 1]].x += tdir.m128_f32[0];
		tan2[_indexvec[index + 1]].y += tdir.m128_f32[1];
		tan2[_indexvec[index + 1]].z += tdir.m128_f32[2];


		tan1[_indexvec[index + 2]].x += sdir.m128_f32[0];
		tan1[_indexvec[index + 2]].y += sdir.m128_f32[1];
		tan1[_indexvec[index + 2]].z += sdir.m128_f32[2];

		tan2[_indexvec[index + 2]].x += tdir.m128_f32[0];
		tan2[_indexvec[index + 2]].y += tdir.m128_f32[1];
		tan2[_indexvec[index + 2]].z += tdir.m128_f32[2];

	}
	for (unsigned int index = 0; index < _vertsvec.size(); index++)
	{
		XMVECTOR n;
		XMVECTOR t;
		t.m128_f32[0] = tan1[index].x;
		t.m128_f32[1] = tan1[index].y;
		t.m128_f32[2] = tan1[index].z;
		t.m128_f32[3] = tan1[index].w;

		XMVECTOR tang;
		for (unsigned int a = 0; a < 4; a++)
		{
			n.m128_f32[a] = _vertsvec[index].m_fNormal[a];
		}
		n = XMVector3Normalize(n);
		t = XMVector3Normalize(t);
		//Gram-Schmidt orthogonalize
		tang = (t - n * XMVector3Dot(n, t));
		tang = XMVector3Normalize(tang);


		XMVECTOR t2;
		t2.m128_f32[0] = tan2[index].x;
		t2.m128_f32[1] = tan2[index].y;
		t2.m128_f32[2] = tan2[index].z;


		//calc handedness
		tang.m128_f32[3] = (XMVector3Dot(XMVector3Cross(n, t), t2)).m128_f32[0] < 0.0f ? 1.0f : -1.0f;
		for (unsigned int i = 0; i < 4; i++)
		{
			_vertsvec[index].m_fTangent[i] = tang.m128_f32[i];

		}
	}


}

void CMesh::BinaryWrite(const char * filePath)
{

	ofstream fout;
	fout.open(filePath, std::ios_base::binary);

	if (fout.is_open())
	{
		const char * name = GetName().c_str();
		unsigned int numVerts = GetVertices().size();
		unsigned int numAnimVerts = GetAnimVertices().size();

		unsigned int numIndices = GetIndices().size();
		unsigned int numTextures = GetTextureNames().size();

		unsigned int nameSize = GetName().size();
		unsigned int animMesh = (numAnimVerts > 0) ? 1 : 0;

		fout.write((char*)&animMesh, sizeof(unsigned int));

		//2 A: Size of name - unsigned int
		fout.write((char*)&nameSize, sizeof(unsigned int));

		//2 B: Name of mesh - string
		fout.write((char*)name, nameSize);

		if (animMesh)
			fout.write((char*)&numAnimVerts, sizeof(unsigned int));
		else
			fout.write((char*)&numVerts, sizeof(unsigned int));

		//3: Number of vertices - unsigned int

		//4: Number of indices - unsigned int
		fout.write((char*)&numIndices, sizeof(unsigned int));



		//6: Number of Textures - unsigned int
		fout.write((char*)&numTextures, sizeof(unsigned int));



		if (animMesh)
			fout.write((char*)GetAnimVertices()._Myfirst, sizeof(TAnimVertex)* numAnimVerts);
		else
			fout.write((char*)GetVertices()._Myfirst, sizeof(TVertex)* numVerts);


		//9: Indices - vector<unsigned int>
		fout.write((char*)GetIndices()._Myfirst, sizeof(unsigned int)* numIndices);

		TAABB aabb;
		BuildAABB(aabb);
		if (aabb.extents.y > 0.00001f)
		{
			char p = 0;
			fout.write(&p, sizeof(char));
			fout.write((char*)&aabb, sizeof(TAABB));
		}

		else
		{
			TPlane plane;
			BuildPlane(plane);
			char p = 1;
			fout.write(&p, sizeof(char));
			fout.write((char*)&plane, sizeof(TPlane));
		}

		unsigned int numAABBs = (unsigned int)m_tAABBs.size();
		fout.write((char*)&numAABBs, sizeof(unsigned int));
		for (unsigned int i = 0; i < numAABBs; i++)
		{
			TAABB aabb = m_tAABBs[i];
			fout.write((char*)&aabb, sizeof(TAABB));
		}

		unsigned int numPlanes = (unsigned int)m_tPlanes.size();
		fout.write((char*)&numPlanes, sizeof(unsigned int));
		for (unsigned int i = 0; i < numPlanes; i++)
		{
			TPlane aabb = m_tPlanes[i];
			fout.write((char*)&aabb, sizeof(TPlane));
		}


		if (animMesh)
		{

			//unsigned int numJoints = GetJoints().size();
			//fout.write((char*)&numJoints, sizeof(unsigned int));

			////10: Joints
			//for (unsigned int joint = 0; joint < numJoints; joint++)
			//{
			//	const char * jointName = GetJoints()[joint].name.c_str();
			//	unsigned int numChildren = GetJoints()[joint].children.size();
			//	unsigned int parent = GetJoints()[joint].parent;
			//	XMFLOAT4X4 bindPose = GetJoints()[joint].bind_pose_transform;
			//	unsigned int jointNameSize = GetJoints()[joint].name.size();

			//	//10 A: Save out size of name - unsigned int
			//	fout.write((char*)&jointNameSize, sizeof(unsigned int));

			//	//10 B: Save out joint name - string
			//	fout.write((char*)jointName, GetJoints()[joint].name.size());

			//	//10 C: Save out number of children - unsigned int
			//	fout.write((char*)&numChildren, sizeof(unsigned int));

			//	//10 D: Save out joint children indices - vector<unsigned int>
			//	fout.write((char*)&GetJoints()[joint].children._Myfirst, sizeof(unsigned int)*GetJoints()[joint].children.size());

			//	//10 E: Save out joint parent - unsigned int
			//	fout.write((char*)&GetJoints()[joint].parent, sizeof(unsigned int));

			//	//10 F: Save out bind pose - float[16]
			//	fout.write((char*)&GetJoints()[joint].bind_pose_transform, sizeof(float) * 16);
			//}

		}
		fout.close();
	}



}





//////////////////////////////////////////////////////
//////////////WRITING OUT ANIMATIONS//////////////////
//////////////////////////////////////////////////////

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

//unsigned int chNameSize = _saveAnim->GetName().size();
//const char * name = _saveAnim->GetName().c_str();

////13 A: Animation Name Size
//fout.write((char*)&chNameSize, sizeof(unsigned int));

////13 B: Name of animation - string
//fout.write((char*)name, _saveAnim->GetName().size());




void CMesh::BinaryRead(const char* path)
{
	
	ifstream file;
	file.open(path, std::ios_base::binary);

	if (file.is_open())
	{
		char * _name;
		unsigned int numVerts;
		unsigned int animMesh;

		unsigned int numIndices;
		unsigned int numTextures;

		unsigned int nameSize;

		file.read((char*)&animMesh, sizeof(unsigned int));

		//2 A: Size of name - unsigned int
		file.read((char*)&nameSize, sizeof(unsigned int));

		_name = new char[nameSize + 1];
		//2 B: Name of mesh - string
		file.read((char*)_name, nameSize);
		_name[nameSize] = '\0';

		GetName() = _name;
		delete[]_name;
		//3: Number of vertices - unsigned int
		file.read((char*)&numVerts, sizeof(unsigned int));

		//4: Number of indices - unsigned int
		file.read((char*)&numIndices, sizeof(unsigned int));


		//6: Number of Textures - unsigned int
		file.read((char*)&numTextures, sizeof(unsigned int));



		if (animMesh == 1)
		{
			GetAnimVertices().resize(numVerts);
			for (size_t i = 0; i < numVerts; i++)
			{
				TAnimVertex vert;
				ZeroMemory(&vert, sizeof(TAnimVertex));

				file.read((char*)&vert, sizeof(TAnimVertex));
				GetAnimVertices()[i] = vert;

			}


			//8: TAnimVertex information - vector<TAnimVertex>
		}
		else
		{
			//7: TVertex information - vector<TVertex>
			GetVertices().resize(numVerts);
			for (size_t i = 0; i < numVerts; i++)
			{
				TVertex vert;
				ZeroMemory(&vert, sizeof(TVertex));
				file.read((char*)&vert, sizeof(TVertex));
				GetVertices()[i] = vert;

			}


		}


		GetIndices().resize(numIndices);
		for (size_t i = 0; i < numIndices; i++)
		{
			unsigned int index;
			file.read((char*)&index, sizeof(unsigned int));
			GetIndices()[i] = index;

		}

		char p = 0;
		file.read(&p, sizeof(char));
		isPlane = (bool)p;
		if (isPlane == false)
		{
			TAABB aabb;
			file.read((char*)&aabb, sizeof(TAABB));
			m_tAABBs.push_back(aabb);

		}
		else
		{
			TPlane plane;
			file.read((char*)&plane, sizeof(TPlane));
			m_tPlanes.push_back(plane);
		}



		unsigned int numAABBs;
		file.read((char*)&numAABBs, sizeof(unsigned int));
		for (unsigned int i = 0; i < numAABBs; i++)
		{
			TAABB aabb;
			file.read((char*)&aabb, sizeof(TAABB));
			m_tAABBs.push_back(aabb);
		}

		unsigned int numPlanes = (unsigned int)m_tPlanes.size();
		file.read((char*)&numPlanes, sizeof(unsigned int));
		for (unsigned int i = 0; i < numPlanes; i++)
		{
			TPlane plane;
			file.read((char*)&plane, sizeof(TPlane));
			m_tPlanes.push_back(plane);

		}

		if (animMesh == 1)
		{
			/*int m_nNumBones = 0;
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
*/
		}
		

		file.close();
	}


}