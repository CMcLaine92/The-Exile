#pragma once
#include "Animation.h"

// This macro is used by the vec3 class to generate
// the code for overloaded operators. You will not be
// calling this directly yourself.
#define VEC3_BINARY_BROADCAST_OPERATOR(_oper_) \
	vec3 operator _oper_ (float fValue) const \
   { \
   vec3 a; \
   a.x = x _oper_ fValue; \
   a.y = y _oper_ fValue; \
   a.z = z _oper_ fValue; \
   return a; \
   }

// This macro is used by the vec3 class to generate
// the code for overloaded operators. You will not be
// calling this directly yourself.
#define VEC3_COMPONENT_OPERATOR(_oper_) \
	reference operator _oper_ (const_reference v) \
   { \
if (static_cast<void *>(this) != static_cast<const void *>(&v)) \
	  { \
	  x _oper_ v.x; \
	  y _oper_ v.y; \
	  z _oper_ v.z; \
	  } \
	  return *this; \
   }

// This macro is used by the vec3 class to generate
// the code for overloaded operators. You will not be
// calling this directly yourself.
#define VEC3_BROADCAST_OPERATOR(_oper_) \
	reference operator _oper_ (float fValue) \
   { \
   x _oper_ fValue; \
   y _oper_ fValue; \
   z _oper_ fValue; \
   return *this; \
   }

struct vec3
{
	//typedef to create an alias for vec3& named reference
	typedef vec3 &reference;

	//typedef to create an alias for const vec3& named const_reference
	typedef vec3 const &const_reference;

	// typedef to create an alias for float named element_type
	typedef float element_type;

	// This union allows the data of the vec3 to be accessed
	// in different ways with no additional overhead.
	//
	// Given the vector,
	// 
	//	vec3f myVector;
	//
	// You can access it's data the following ways...
	union
	{
		// Access the vector as an array of floating point values by using myVector.v
		element_type v[3];

		// Access the individuals components of the vector with myVector.x, myVector.y, or myVector.z
		struct
		{
			element_type x;
			element_type y;
			element_type z;
		};
	};

	// Constructor
	vec3(void) { }
	// Destructor
	~vec3(void) { }
	// Constructor given X,Y,Z values
	vec3(element_type vx, element_type vy, element_type vz) : x(vx), y(vy), z(vz) { }

	// Copy Constructor
	vec3(const vec3 &v) : x(v.x), y(v.y), z(v.z) { }

	// These are simply macros that generate the code for the
	// various overloaded operators. Simply use +,-,*,/,=,+=,
	// -=, *=, /= operators with vec3 objects to use them.
	// 
	// Ex:
	//
	//	vec3f vecA( 1.0f, 0.0f, 0.0f);
	//	vec3f vecB( 0.0f, 1.0f, 0.0f);
	//
	//	vecA += vecB;
	//

	VEC3_BINARY_BROADCAST_OPERATOR(+);
	VEC3_BINARY_BROADCAST_OPERATOR(-);
	VEC3_BINARY_BROADCAST_OPERATOR(*);
	VEC3_BINARY_BROADCAST_OPERATOR(/ );

	VEC3_COMPONENT_OPERATOR(= );
	VEC3_COMPONENT_OPERATOR(+= );
	VEC3_COMPONENT_OPERATOR(-= );
	VEC3_COMPONENT_OPERATOR(*= );
	VEC3_COMPONENT_OPERATOR(/= );

	VEC3_BROADCAST_OPERATOR(= );
	VEC3_BROADCAST_OPERATOR(+= );
	VEC3_BROADCAST_OPERATOR(-= );
	VEC3_BROADCAST_OPERATOR(*= );
	VEC3_BROADCAST_OPERATOR(/= );

	// Zero out the vector
	reference make_zero(void)
	{
		x = y = z = 0;
		return *this;
	}

	// Normalize the vector
	reference normalize(void)
	{
		element_type rsqrt = 1 / magnitude();
		x *= rsqrt;
		y *= rsqrt;
		z *= rsqrt;
		return *this;
	}

	// Negate the vector
	reference negate(void)
	{
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}

	// Calculate and return the magnitude of the vector
	element_type magnitude(void)
	{
		element_type fMagnitude;
		fMagnitude = (float)sqrt(x*x + y*y + z*z);
		return fMagnitude;
	}

	// dot_product
	//
	// Calculat the dot product of two vectors
	//
	// In:
	//	const_reference vectorA - constant reference to the first vector
	//	const_reference vectorB - constant reference to the second vector
	//
	// Return:
	//	element_type - the result of the dotproduct
	inline friend element_type dot_product(const_reference vectorA, const_reference vectorB)
	{
		return vectorA.x*vectorB.x + vectorA.y*vectorB.y + vectorA.z*vectorB.z;
	}

	// cross_product
	//
	// Calculate the cross product of two vectors
	//
	// In:
	//	const_reference vectorA - constant reference to the first vector
	//	const_reference vectorB - constant reference to the second vector
	//
	// Out:
	//	reference vectorO - Where to store the result of the cross product
	//	
	// Remarks:
	//	
	//	vectorO must not be a reference to the same object as vectorA or vectorB
	inline friend void cross_product(reference vectorO, const_reference vectorA, const_reference vectorB)
	{
		vectorO.x = vectorA.y*vectorB.z - vectorA.z*vectorB.y;
		vectorO.y = vectorA.z*vectorB.x - vectorA.x*vectorB.z;
		vectorO.z = vectorA.x*vectorB.y - vectorA.y*vectorB.x;
	}

	// interpolate
	//
	// Linear interpolate two vectors based on a lamda value and return the resulting vector.
	//
	// In:
	//	const_reference vectorA - constant reference to the first vector
	//	const_reference vectorB - constant reference to the second vector
	//
	// Return:
	//
	//	vec3 - The resulting vector from the interpolation	

	element_type *array(void) { return this->v; }
	element_type const *array(void) const { return this->v; }
};

struct matrix4
{
	typedef float element_type;
	typedef matrix4 &reference;
	typedef matrix4 const &const_reference;

	union
	{
		element_type m[4][4];
		element_type ma[16];
		struct
		{
			element_type xx, xy, xz, xw;
			element_type yx, yy, yz, yw;
			element_type zx, zy, zz, zw;
			element_type wx, wy, wz, ww;
		};
		struct
		{
			vec3 axis_x;	element_type pad_xw;
			vec3 axis_y;	element_type pad_yw;
			vec3 axis_z;	element_type pad_zw;
			union
			{
				struct
				{
					vec3 axis_w;	element_type pad_ww;
				};
				struct
				{
					vec3 axis_pos;
				};
			};
		};
	};

	matrix4::matrix4(element_type m00, element_type m01, element_type m02, element_type m03,
		element_type m10, element_type m11, element_type m12, element_type m13,
		element_type m20, element_type m21, element_type m22, element_type m23,
		element_type m30, element_type m31, element_type m32, element_type m33)
		: 
		xx(m00), xy(m01), xz(m02), xw(m03),
		yx(m10), yy(m11), yz(m12), yw(m13),
		zx(m20), zy(m21), zz(m22), zw(m23),
		wx(m30), wy(m31), wz(m32), ww(m33)
	{
	}

	matrix4::matrix4(element_type n)
		: xx(n), xy(n), xz(n), xw(n),
		yx(n), yy(n), yz(n), yw(n),
		zx(n), zy(n), zz(n), zw(n),
		wx(n), wy(n), wz(n), ww(n)
	{
	}

	matrix4 matrix4::operator *(const matrix4 &rhs)
	{
		matrix4 temp(rhs.xx * xx + rhs.yx * xy + rhs.zx * xz + rhs.wx * xw,
			rhs.xy * xx + rhs.yy * xy + rhs.zy * xz + rhs.wy * xw,
			rhs.xz * xx + rhs.yz * xy + rhs.zz * xz + rhs.wz * xw,
			rhs.xw * xx + rhs.yw * xy + rhs.zw * xz + rhs.ww * xw,

			rhs.xx * yx + rhs.yx * yy + rhs.zx * yz + rhs.wx * yw,
			rhs.xy * yx + rhs.yy * yy + rhs.zy * yz + rhs.wy * yw,
			rhs.xz * yx + rhs.yz * yy + rhs.zz * yz + rhs.wz * yw,
			rhs.xw * yx + rhs.yw * yy + rhs.zw * yz + rhs.ww * yw,

			rhs.xx * zx + rhs.yx * zy + rhs.zx * zz + rhs.wx * zw,
			rhs.xy * zx + rhs.yy * zy + rhs.zy * zz + rhs.wy * zw,
			rhs.xz * zx + rhs.yz * zy + rhs.zz * zz + rhs.wz * zw,
			rhs.xw * zx + rhs.yw * zy + rhs.zw * zz + rhs.ww * zw,

			rhs.xx * wx + rhs.yx * wy + rhs.zx * wz + rhs.wx * ww,
			rhs.xy * wx + rhs.yy * wy + rhs.zy * wz + rhs.wy * ww,
			rhs.xz * wx + rhs.yz * wy + rhs.zz * wz + rhs.wz * ww,
			rhs.xw * wx + rhs.yw * wy + rhs.zw * wz + rhs.ww * ww);
		return temp;
	}
};

class CAnimationManager
{
	struct TBlendingAnimation
	{
		vector<XMFLOAT4X4> m_cOriginalPose;
		CAnimation * m_cBlendFrom;
		CAnimation * m_cBlendTo;
		float m_fCurrentTime = 0.0f;
		float m_fTotalTime = 0.0f;
	};

	vector<CAnimation *> m_cAnimations;
	vector<TBlendingAnimation> m_tBlendingAnimations;

	XMFLOAT4X4 interpolate(const XMFLOAT4X4 &MatrixA, const XMFLOAT4X4 &MatrixB, float TotalTime, float lamda);
	ID3D11Buffer * m_BoneBuffer;
	ID3D11Buffer * m_InvBoneBuffer;
	ID3D11VertexShader * m_AnimVertShader;
	ID3D11PixelShader * m_AnimPixShader;
	ID3D11InputLayout * m_Layout;

public:
	CAnimationManager();
	~CAnimationManager();

	void Blend(CAnimation * _cCurrAnim, CAnimation * _cToAnim, float _fTimeToBlend);
	void AddAnimation(CAnimation * _cAnim);
	void RemoveAnimation(CAnimation * _cAnim);
	void Update();
	void Blending();
	void EndBlend(TBlendingAnimation * _blender);
	void Switch(CAnimation * _cCurrAnim, CAnimation * _cToAnim);

	unsigned int keyToRender = 0;
};

void ConvertXMFLOAT4X4(XMFLOAT4X4 _mat, matrix4& _newMat);
void ConvertToXMFLOAT4X4(matrix4 _mat, XMFLOAT4X4& _newMat);

