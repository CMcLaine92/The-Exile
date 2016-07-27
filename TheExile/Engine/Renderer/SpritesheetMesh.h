#ifndef SPRITESHEETMESH_H_
#define SPRITESHEETMESH_H_
#include "RenderMesh.h"
/***********************************************
* Filename:  		SpritesheetMesh.h
* Date:      		09/03/2015
* Mod. Date: 		----------
* Mod. Initials:	--
* Author:    		Matthew Zanini
* Purpose:   		Used to get Particles to the Renderer
*
************************************************/
struct TSpritesheetData
{
	//Frame width
	float m_fWidth;
	//Frame height
	float m_fHeight;
	float m_fTextureWidth;
	float m_fTextureHeight;
	unsigned int m_unRows;
	unsigned int m_unColumns;
	float m_fFrameTime;
};

class CSpritesheetMesh : public CRenderMesh
{
private:	
	TSpritesheetData m_tSpritesheetData;
	unsigned int m_unCurrentFrame = 0;
	float m_fCurrentTime =0.0f;
	//Scale for a single update of frames
	float m_fUVScale[2];
	//Hold Onto Original TexCoords for scaling purposes
	std::vector<float> m_vOriginalTexCoordsU;
	std::vector<float> m_vOriginalTexCoordsV;
	vector < unsigned int > m_nIndexArr;
	vector < TVertex3D >m_tVertexArr;


	//Helper Functions
	void MapVertexBuffer(ID3D11DeviceContext* d3dContext);
	void InitializeUVs();
	void MoveOverFrame();
public:
	CSpritesheetMesh(std::vector<unsigned int> tIndexArr, std::vector<TVertex3D> tVertexArr,TSpritesheetData tSpritesheetData,const wchar_t* chTexturePath);
	~CSpritesheetMesh() = default;
	void Draw() override;
	void Draw(ID3D11DeviceContext* d3dContext);
	void Update();
};


#endif