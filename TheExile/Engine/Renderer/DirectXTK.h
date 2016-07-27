#pragma once

#define MAX_FONTS 25 // Doesn't take up much memory, go wild

// Forward Declarations //
struct ID3D11Device;
struct ID3D11DeviceContext;
namespace DirectX { struct XMFLOAT2; }
namespace DirectX { struct XMFLOAT3; }
namespace DirectX { struct XMFLOAT4; }

namespace DirectX { class SpriteFont; }
namespace DirectX { class SpriteBatch; }

typedef DirectX::XMFLOAT2 float2;
typedef DirectX::XMFLOAT3 float3;
typedef DirectX::XMFLOAT4 float4;

class CTextRenderer
{
	float m_fWidth;
	float m_fHeight;
	unsigned int m_nFontCount = 0;

	DirectX::SpriteFont* m_d3dFont[MAX_FONTS];
	DirectX::SpriteBatch* m_d3dBatch = nullptr;

	void Convert(float2& fPosition, float2& Scale);

public:
	CTextRenderer(ID3D11DeviceContext* d3dContext, float2 fWindowRes);
	~CTextRenderer();

	void Resize(float2 fWindowRes);
	void RenderText(const wchar_t* chText, float2 fPosition, float4 fColor, float2 fScale, int nAlign = 0, unsigned int nFontIndex = 0);
	unsigned int LoadFont(ID3D11Device* d3dDevice, const wchar_t* chFontPath);
};