#pragma pack_matrix(row_major)

#define SHADOW_PASSES 6

struct VS_Input
{
	float4 pos : POSITIONS;
	float4 normals : NORMALS;
	float4 uvs : TEXCOORDS;
	float4 tagents : TANGENTS;

	uint4 BoneIndex : BONEINDICES;
	float4 BoneWeight : BONEWEIGHTS;
};

cbuffer bObject : register(b0)
{
	float4x4 fTransformMatrix;
}

cbuffer bScene : register(b1)
{
	float4x4 fViewMatrix;
	float4x4 fProjectionMatrix;
}

//StructuredBuffer<float4x4> invMats : register(t0);
//
//StructuredBuffer<float4x4> curMats : register(t1);

cbuffer BONES : register(b2)
{
	float4x4 CurBones[100];
	float4x4 InvBones[100];
};

struct TScene
{
	float4x4 fViewMatrix;
	float4x4 fProjectionMatrix;
};

cbuffer bLightScene : register(b9)
{
	TScene tScene[SHADOW_PASSES];
}

struct VS_Output
{
	float4 oldPos : OLD_POSITION;
	float4 pos : SV_POSITION;
	float4 normals : NORMALS;
	float4 uvs : TEXCOORD0;
	float4 tagents : TANGENTS;
};

VS_Output main(VS_Input input)
{
	VS_Output output;
	output.oldPos =
	output.pos =
	output.normals =
	output.uvs =
	output.tagents = float4(0, 0, 0, 0);

	output.uvs = float4(input.uvs);
	output.normals = float4(input.normals.xyz, 1.0f);
	output.pos = float4(input.pos);

	float4 NewVertex;
	float4 NewNormals;
	float4 TempPos;

	//Position
	float4x4 BindPose = InvBones[input.BoneIndex.x];
		float4x4 CurPose = CurBones[input.BoneIndex.x];

		TempPos = mul(output.pos, BindPose);
	TempPos = mul(TempPos, CurPose);

	TempPos = mul(TempPos, input.BoneWeight.x);
	NewVertex = float4(TempPos.xyz, 1.0f);


	BindPose = InvBones[input.BoneIndex.y];
	CurPose = CurBones[input.BoneIndex.y];

	TempPos = mul(output.pos, BindPose);
	TempPos = mul(TempPos, CurPose);

	TempPos = mul(TempPos, input.BoneWeight.y);
	NewVertex += float4(TempPos.xyz, 0.0f);



	BindPose = InvBones[input.BoneIndex.z];
	CurPose = CurBones[input.BoneIndex.z];

	TempPos = mul(output.pos, BindPose);
	TempPos = mul(TempPos, CurPose);

	TempPos = mul(TempPos, input.BoneWeight.z);
	NewVertex += float4(TempPos.xyz, 0.0f);



	BindPose = InvBones[input.BoneIndex.w];
	CurPose = CurBones[input.BoneIndex.w];

	TempPos = mul(output.pos, BindPose);
	TempPos = mul(TempPos, CurPose);

	TempPos = mul(TempPos, input.BoneWeight.w);
	NewVertex += float4(TempPos.xyz, 0.0f);

	/*NewVertex = mul(mul(output.pos, InvBones[input.BoneIndex.x]),
		mul(input.BoneWeight.x, CurBones[input.BoneIndex.x]));

		NewVertex = mul(mul(output.pos, InvBones[input.BoneIndex.y]),
		mul(input.BoneWeight.y, CurBones[input.BoneIndex.y]))
		+ NewVertex;

		NewVertex = mul(mul(output.pos, InvBones[input.BoneIndex.z]),
		mul(input.BoneWeight.z, CurBones[input.BoneIndex.z]))
		+ NewVertex;

		NewVertex = mul(mul(output.pos, InvBones[input.BoneIndex.w]),
		mul(input.BoneWeight.w, CurBones[input.BoneIndex.w]))
		+ NewVertex;*/

	//Normals
	//NewNormals = mul(mul(output.normals, InvBones[input.BoneIndex.x]),
	//	mul(input.BoneWeight.x, CurBones[input.BoneIndex.x]));
	//
	//NewNormals = mul(mul(NewNormals, InvBones[input.BoneIndex.y]),
	//	mul(input.BoneWeight.y, CurBones[input.BoneIndex.y]))
	//	+ NewNormals;
	//
	//NewNormals = mul(mul(NewNormals, InvBones[input.BoneIndex.z]),
	//	mul(input.BoneWeight.z, CurBones[input.BoneIndex.z]))
	//	+ NewNormals;
	//
	//NewNormals = mul(mul(NewNormals, InvBones[input.BoneIndex.w]),
	//	mul(input.BoneWeight.w, CurBones[input.BoneIndex.w]))
	//	+ NewNormals;

	float4x4 fNormalTransform = fTransformMatrix;
	fNormalTransform[3] = float4(0, 0, 0, 0); // Zero out position

	output.pos = float4(NewVertex.xyz, 1.0f);
	//output.normals = float4(NewNormals.xyz, 1.0f);

	output.oldPos = mul(float4(NewVertex.xyz, 1.0f), fTransformMatrix);
	output.normals = mul(output.normals, fNormalTransform);

	output.pos = mul(output.pos, fTransformMatrix);
	output.pos = mul(output.pos, fViewMatrix);
	output.pos = mul(output.pos, fProjectionMatrix);

	return output;
}