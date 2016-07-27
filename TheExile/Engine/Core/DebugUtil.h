

#ifndef _DEBUG_UTIL_H_
#define _DEBUG_UTIL_H_


#define CIRCLE_SIDES 361
enum eCubeVerts { TLF, TLB, TRF, TRB, BLF, BLB, BRF, BRB };
class CNavGraph;


namespace DEBUG_UTIL
{

	void DrawAABBCollider(XMFLOAT3 min, XMFLOAT3 max, XMFLOAT3 color = { 0.0f, 1.0f, 0.0f });

	void DrawWorldAxis(XMFLOAT3 position);
	//{
	//	XMFLOAT3 verts[8] =
	//	{
	//		{ min.x, max.y, min.z }, { min.x, max.y, max.z }, { max.x, max.y, min.z }, { max.x, max.y, max.z },
	//		{ min.x, min.y, min.z }, { min.x, min.y, max.z }, { max.x, min.y, min.z }, { max.x, min.y, max.z }
	//	};

	//	/*verts[TLF] = XMFLOAT3(min.x, max.y, min.z);
	//	verts[TLB] = XMFLOAT3(min.x, max.y, max.z);
	//	verts[TRF] = XMFLOAT3(max.x, max.y, min.z);
	//	verts[TRB] = XMFLOAT3(max.x, max.y, max.z);
	//	verts[BLF] = XMFLOAT3(min.x, min.y, min.z);
	//	verts[BLB] = XMFLOAT3(min.x, min.y, max.z);
	//	verts[BRF] = XMFLOAT3(max.x, min.y, min.z);
	//	verts[BRB] = XMFLOAT3(max.x, min.y, max.z);*/

	//	//Top Face

	//	GRAPHICS->DrawLine(verts[TLF], verts[TLB], color);
	//	GRAPHICS->DrawLine(verts[TLB], verts[TRB], color);
	//	GRAPHICS->DrawLine(verts[TRB], verts[TRF], color);
	//	GRAPHICS->DrawLine(verts[TRF], verts[TLF], color);

	//	//Bottom Face
	//	GRAPHICS->DrawLine(verts[BLF], verts[BLB], color);
	//	GRAPHICS->DrawLine(verts[BLB], verts[BRB], color);
	//	GRAPHICS->DrawLine(verts[BRB], verts[BRF], color);
	//	GRAPHICS->DrawLine(verts[BRF], verts[BLF], color);

	//	//Connecting Lines
	//	GRAPHICS->DrawLine(verts[BLF], verts[TLF], color);
	//	GRAPHICS->DrawLine(verts[BLB], verts[TLB], color);
	//	GRAPHICS->DrawLine(verts[BRB], verts[TRB], color);
	//	GRAPHICS->DrawLine(verts[BRF], verts[TRF], color);


	//}

	void DrawCapsuleCollider(XMFLOAT3 start, XMFLOAT3 end, float radius, XMFLOAT3 color = { 0.0f, 1.0f, 0.0f });
	/*{
		XMFLOAT3 verts[CIRCLE_SIDES];
		
		for (unsigned short i = 0; i < CIRCLE_SIDES; ++i)
		{
			verts[i].x = sin(i * (XM_PI / 180.0f)) * radius + start.x;
			verts[i].z = cos(i * (XM_PI / 180.0f)) * radius + start.z;
			verts[i].y = 0.0f;
		}
		

		for (unsigned short i = 0; i + 1 < CIRCLE_SIDES; ++i)
		{
			XMFLOAT3 top = verts[i];
			XMFLOAT3 bottom = top;
			XMFLOAT3 topnext = verts[i + 1];
			XMFLOAT3 bottomnext = topnext;

			top.y = end.y + radius;
			topnext.y = top.y;
			bottom.y = start.y - radius;
			bottomnext.y = bottom.y;

			GRAPHICS->DrawLine(top, topnext, color);
			GRAPHICS->DrawLine(bottom, bottomnext, color);

			if (i % 36 == 0)
				GRAPHICS->DrawLine(top, bottom, color);
		}
	}*/
	void DrawNavGraph(CNavGraph* navGraph, XMFLOAT3 color = { 1.0f, 1.0f, 0.0f });
	void DrawPath(vector<XMFLOAT3>& path, XMFLOAT3 color = { 1.0f, 1.0f, 0.0f });
	/*{
		if (path.size())
		{
			for (size_t i = 0; i < path.size() - 1; i++)
			{
				XMFLOAT3 thisPos = path[i];
				XMFLOAT3 nextPos = path[i + 1];
				GRAPHICS->DrawLine(thisPos, nextPos, color);
			}
		}
	}*/




}





#endif