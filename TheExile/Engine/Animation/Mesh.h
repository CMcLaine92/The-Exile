#pragma once
#include "../Renderer/Renderer.h"

struct Joint
{
	// The name of the joint
	std::string name;

	// The joints parent
	unsigned int parent;

	// The joints children
	std::vector< unsigned int > children;

	// Bind pose transform in world space
	XMFLOAT4X4 bind_pose_transform;
};

struct TAnimVertex
{
	float m_fPosition[4];
	float m_fNormal[4];
	float m_fTexCoord[4];
	float m_fTangent[4];

	unsigned int m_fBoneIndex[4];
	float m_fBoneWeight[4];
};

struct TVertex3D
{
	float m_fPosition[4];
	float m_fNormal[4];
	float m_fTexCoord[4];
	float m_fTangent[4];
};

typedef TVertex3D TVertex; // Legacy


class CMesh
{
public:

	enum eTextureName {eDiffuse, eNormal,eNumTextures};

	struct TAABB
	{
		float3 center;
		float3 extents;
	};
	struct TPlane
	{
		float3 extents;
		float3 normal;
		float3 center;
		float distance;
	};

	struct TexCoord
	{
		float u, v;
	};

	struct MinMax
	{
		float3 min;
		float3 max;
	};

	

	struct JointInfluence
	{
		unsigned int joint_index;
		float weight;
	};

	struct UniqueVertex
	{
		XMFLOAT3 position;
		XMFLOAT3 normal;
		TexCoord tex_coord;

		int control_point_index;

		bool operator==(const UniqueVertex& v) const
		{
			return position.x == v.position.x &&
				position.y == v.position.y &&
				position.z == v.position.z &&
				normal.x == v.normal.x &&
				normal.y == v.normal.y &&
				normal.z == v.normal.z &&
				tex_coord.u == v.tex_coord.u &&
				tex_coord.v == v.tex_coord.v &&
				control_point_index == v.control_point_index;
		}
	};

public:

	CMesh();
	~CMesh();

	//string textures[eNumTextures];

	bool LoadTexture(const char* file_name);

	std::string& GetName();
	void BuildAABB(TAABB& aabb);
	void BuildPlane(TPlane& plane);

	std::vector< UniqueVertex >& GetUniqueVertices();

	std::vector< TVertex >& GetVertices();
	vector< TAnimVertex >& GetAnimVertices();

	vector<TAABB>& GetAABBs() { return m_tAABBs; }
	vector<TPlane>& GetPlanes() { return m_tPlanes; }
	void SetName(const char* _name) { name = _name; }

	std::vector< unsigned int >& GetIndices();

	bool IsPlane() { return isPlane; }
	std::vector< Joint >& GetJoints();
	std::vector< std::vector< JointInfluence >>& GetInfluences();

	XMFLOAT4X4& GetLocalMatrix();
	XMFLOAT3 GetLocalPosition();

	std::vector< std::string >& GetTextureNames();
	void ConvertVertices(bool animMesh = false);

	void CalculateTangentVectors(std::vector<TVertex>& _vertsvec, std::vector<unsigned int> & _indexvec);
	void CalculateTangentVectors(std::vector<TAnimVertex>& _vertsvec, std::vector<unsigned int> & _indexvec);

	void BinaryWrite(const char* path);
	void BinaryRead(const char* path);


private:

	std::string name;

	std::vector< UniqueVertex > vertices;

	vector< TAnimVertex > m_tAnimVertices;
	vector<TVertex> m_tVertices;

	vector<TAABB> m_tAABBs;
	vector<TPlane> m_tPlanes;


	std::vector< unsigned int > indices;

	std::vector< Joint > joints;
	std::vector< std::vector< JointInfluence >> influences;

	std::vector< std::string > texture_names;
	std::vector< ID3D11ShaderResourceView * > textures;

	XMFLOAT4X4 localMatrix;
	bool isPlane = false;
};


