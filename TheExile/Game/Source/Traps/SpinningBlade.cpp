/***********************************************
* Filename:  		SpinningBlade.xpp
* Date:      		09/03/2015
* Mod. Date:
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:			Spinning blade that rotates around it's self and dealing damage to anything hit.
************************************************/
#include "SpinningBlade.h"
#include "../../../Engine/Asset Manager/AssetManager.h"
#include "../../../Engine/Core/Messages/AddObjectMessage.h"
#include "../../../Engine/Core/Messages/RemoveObjectMessage.h"
#include "../../../Engine/Core/Collision/Capsule.h"
#include "../../../Engine/Core/Collision/AABB.h"
#include "../../../Game/Source/Player/Player.h"
#include "../../../Game/Source/Agents/Skeleton/Skeleton.h"

#include "../../../Engine/Audio/Source/AudioSystemWwise.h"
#include "../../../Engine/Audio/Source/Wwise_IDs.h"

#include "../../../Engine/Animation/Mesh.h"
#include "../../../Engine/Animation/AnimationMesh.h"


#define TRAP_HEIGHT 180
#define TRAP_DMG 25
#define BLADE_RADIUS 10
#define TRAP_SPEED 250.0f
#define RETURN_RANGE 100
#define TRACK_WIDTH 80.0f

CSpinningBlade::CSpinningBlade( XMFLOAT3 _location, float _rotationSpeed, XMFLOAT3 _start, XMFLOAT3 _end ) : IObject( "SpinningBlade" )
{
	m_fRotationSpeed = _rotationSpeed;
	m_fRotationAngle = 0.0f;
	m_nTrapDamage = TRAP_DMG;
	m_vStart = _start;
	m_vEnd = _end;
	m_vTarget = m_vEnd;
	m_fOffset = _location;

	if( m_vEnd.x == 0.0f && m_vEnd.y == 0.0f && m_vEnd.z == 0.0f )
		m_bMoving = false;
	else
		m_bMoving = true;

	AudioSystemWwise::Get()->RegisterEntity( this, "SpinningBlade" );

	// Add this to the obj Manager
	//CAddObjectMessage* addObj = new CAddObjectMessage( this, CObjectManager::Dynamic );
	//if( !addObj->Send() )
	//	return;

	// Set the Location
	SetPosition( _location );

	// Load/Set the mesh
	vSpinMesh = CAssetManager::GetInstance()->GetPrefabMesh("SpinningBlade");
	SetRenderMesh(new CRenderMesh(vSpinMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetStandardPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Failbot_Metal.dds"));

	// Find the size of the blades
	float max = 0, min = 0;
	FindBladeSize( max, min );
	m_fRadius = max;

	// Quad treee collider (Not detecting collision)
	CMesh::TAABB& aabb = vSpinMesh->GetAABBs()[0];
	
	XMFLOAT3 f3Extents = aabb.extents;
	f3Extents.x += 700.0f;
	f3Extents.z += 700.0f;

	AddCollider(new CCollider(false, new CAABB(aabb.center, f3Extents), false));

	// Create the X-Axis blade collider
	XMFLOAT3 XStart = { _location.x + max, _location.y + TRAP_HEIGHT, _location.z };
	XMFLOAT3 XEnd = { _location.x + min, _location.y + TRAP_HEIGHT, _location.z };
	AddCollider( new CCollider( false, new CCapsule( XStart, XEnd, BLADE_RADIUS ), true ) );
	m_bDealDamageToPlayer.push_back( true );
	m_bDealDamageToSkeleton.push_back( true );

	// Create the Z-Axis blade collider
	XMFLOAT3 YStart = { _location.x, _location.y + TRAP_HEIGHT, _location.z + max };
	XMFLOAT3 YEnd = { _location.x, _location.y + TRAP_HEIGHT, _location.z - max };
	AddCollider( new CCollider( false, new CCapsule( YStart, YEnd, BLADE_RADIUS ), true ,false) );
	m_bDealDamageToPlayer.push_back( true );
	m_bDealDamageToSkeleton.push_back( true );

	GRAPHICS->AddRenderMesh( m_cpRenderMesh );
}

CSpinningBlade::~CSpinningBlade()
{
	SAFE_DELETE(m_cTrack);

	AudioSystemWwise::Get()->UnRegisterEntity( this );

	CRemoveObjectMessage* removeObj = new CRemoveObjectMessage( this, CObjectManager::Static, false );
	removeObj->Send();
}

void CSpinningBlade::Update()
{
	if( !m_bAudioPlaying )
	{
		AudioSystemWwise::Get()->PostEvent( AK::EVENTS::SPINNINGBLADE, this );
		m_bAudioPlaying = true;
	}

	if( m_bMoving )
	{
		// calculate the velocity from current to target.
		XMVECTOR toTarget = XMLoadFloat3( &m_vTarget ) - XMLoadFloat3( GetPosition() );

		if( XMVector3Length( toTarget ).m128_f32[0] < RETURN_RANGE )
		{
			XMVECTOR start = XMLoadFloat3(&m_vStart);
			
			if(  XMVector3Equal(XMLoadFloat3(&m_vTarget), start)  ) // If we've reached the start
				m_vTarget = m_vEnd; // New target is the end
			else // If we've reached the end
				m_vTarget = m_vStart; // New target is the start
		}


		toTarget = XMVector3Normalize( toTarget );
		toTarget *= TRAP_SPEED;
		XMFLOAT3 newVel; XMStoreFloat3( &newVel, toTarget );
		//SetWorldVelocity( newVel );

		// Manual Position Update //
		m_mWorld.m[3][0] += newVel.x * DELTA_TIME();
		m_mWorld.m[3][1] += newVel.y * DELTA_TIME();
		m_mWorld.m[3][2] += newVel.z * DELTA_TIME();
	}

	RotateLocalY( m_mWorld, -m_fRotationSpeed ); // Rotate the model based on a rotation speed

	// Keep track of the angle we are at
	m_fRotationAngle += m_fRotationSpeed;

	if( m_fRotationAngle >= 360.0f )
		m_fRotationAngle -= 360.0f;

	// Convert To radiansd
	float rotationRadians = RADIANS( m_fRotationAngle );

	// Get the X-Axis Blade collider bounds
	CCapsule* theXCapsule = reinterpret_cast<CCapsule*> ( m_pvColliders[1]->GetBounds() );

	// Calculate their new positions relative to worlds origin (new rotation)
	XMFLOAT3 Xstart = { cos( rotationRadians ) * m_fRadius, TRAP_HEIGHT, sin( rotationRadians ) * m_fRadius };
	XMFLOAT3 Xend = { cos( rotationRadians + XM_PI ) * m_fRadius, TRAP_HEIGHT, sin( rotationRadians + XM_PI ) * m_fRadius };

	// Adjust for the traps position
	Xstart.x += GetPosition()->x;
	Xstart.z += GetPosition()->z;
	Xend.x += GetPosition()->x;
	Xend.z += GetPosition()->z;

	// Set the new values
	theXCapsule->SetStart( Xstart );
	theXCapsule->SetEnd( Xend );

	// Repeat for Z-Axis Blade
	CCapsule* theZCapsule = reinterpret_cast<CCapsule*>( m_pvColliders[2]->GetBounds() );

	XMFLOAT3 Zstart = { cos( rotationRadians + XM_PI / 2 ) * m_fRadius, TRAP_HEIGHT, sin( rotationRadians + XM_PI / 2 ) * m_fRadius };
	XMFLOAT3 Zend = { cos( rotationRadians + ( 3 * XM_PI / 2 ) ) * m_fRadius, TRAP_HEIGHT, sin( rotationRadians + 3 * XM_PI / 2 ) * m_fRadius };

	Zstart.x += GetPosition()->x;
	Zstart.z += GetPosition()->z;
	Zend.x += GetPosition()->x;
	Zend.z += GetPosition()->z;

	theZCapsule->SetStart( Zstart );
	theZCapsule->SetEnd( Zend );

}

int l = 0;
void CSpinningBlade::FindBladeSize( float& _outMax, float& _outMin )
{

	// Loop through all the verts and find the highest X
	_outMax = -FLT_MAX;
	_outMin = FLT_MAX;
	for( size_t i = 0; i < vSpinMesh->GetVertices().size(); i++ )
	{
		TVertex curVert = vSpinMesh->GetVertices()[i];

		if( curVert.m_fPosition[0] > _outMax )
		{
			_outMax = curVert.m_fPosition[0];
		}
		else if( curVert.m_fPosition[0] < _outMin )
		{
			_outMin = curVert.m_fPosition[0];
		}
	}

}

void CSpinningBlade::CollisionResponse( IObject * _cCollidedObject )
{
	DBG_UNREFERENCED_PARAMETER( _cCollidedObject );

	/*if (_cCollidedObject->GetTag() == "Player" || _cCollidedObject->GetTag() == "Skeleton")
	{

	for (size_t i = 1; i < m_pvColliders.size(); i++)
	{
	CCollider* curCollider = m_pvColliders[i];

	for (size_t obj = 0; obj < curCollider->GetCollidedObjects().size(); obj++)
	{
	IObject* curObj = curCollider->GetCollidedObjects()[obj];

	if (curObj->GetTag() == "Player")
	{

	if (m_bDealDamageToPlayer[i - 1])
	{
	reinterpret_cast<CPlayer*>(_cCollidedObject)->TakeDamage(TRAP_DMG);
	m_bDealDamageToPlayer[i - 1] = false;
	curCollider->SetCollided(false);
	}
	}
	else if (curObj->GetTag() == "Skeleton")
	{
	if (m_bDealDamageToSkeleton[i - 1])
	{
	reinterpret_cast<CSkeleton*>(_cCollidedObject)->TakeDamage(TRAP_DMG);
	}


	if (obj == (curCollider->GetCollidedObjects().size() - 1))
	{
	m_bDealDamageToSkeleton[i - 1] = false;
	}

	}
	}

	if (!curCollider->GetCollided())
	{
	m_bDealDamageToPlayer[i - 1] = true;
	m_bDealDamageToSkeleton[i - 1] = true;

	}
	else
	{
	}

	}

	}*/
}

void CSpinningBlade::CreateTrack()
{
	// Track Creation //
	if (m_bMoving == true)
	{
		float fTrackLength = 0.0f;		

		float fStartX = m_vStart.x - m_fOffset.x;
		float fStartZ = m_vStart.z - m_fOffset.z;

		float fEndX = m_vEnd.x - m_fOffset.x;
		float fEndZ = m_vEnd.z - m_fOffset.z;

		CMesh cTrack;

		cTrack.GetIndices().push_back(0);
		cTrack.GetIndices().push_back(1);
		cTrack.GetIndices().push_back(2);

		cTrack.GetIndices().push_back(0);
		cTrack.GetIndices().push_back(2);
		cTrack.GetIndices().push_back(3);

		if (m_fOffset.z > 0)
		{
			fTrackLength = m_vStart.z - m_vEnd.z;
			OUTPUT("Long\n");

			cTrack.GetVertices().push_back
				({
					{ fStartX - TRACK_WIDTH, 3.0f, fStartZ, 1.0f },
					{ 0.0f, 1.0f, 0.0f, 1.0f },
					{ 0, -(fTrackLength / 200.0f) }
			});

			cTrack.GetVertices().push_back
				({
					{ fStartX + TRACK_WIDTH, 3.0f, fStartZ, 1.0f },
					{ 0.0f, 1.0f, 0.0f, 1.0f },
					{ 1, -(fTrackLength / 200.0f) }
			});

			cTrack.GetVertices().push_back
				({
					{ fEndX + TRACK_WIDTH, 3.0f, fEndZ, 1.0f },
					{ 0.0f, 1.0f, 0.0f, 1.0f },
					{ 1, (fTrackLength / 200.0f) }
			});

			cTrack.GetVertices().push_back
				({
					{ fEndX - TRACK_WIDTH, 3.0f, fEndZ, 1.0f },
					{ 0.0f, 1.0f, 0.0f, 1.0f },
					{ 0, (fTrackLength / 200.0f) }
			});
		}
		else
		{
			fTrackLength = m_vStart.x - m_vEnd.x;
			OUTPUT("Wide\n");

			cTrack.GetVertices().push_back
				({
					{ fStartX, 3.0f, fStartZ + TRACK_WIDTH, 1.0f },
					{ 0.0f, 1.0f, 0.0f, 1.0f },
					{ -(fTrackLength / 200.0f), 0 }
			});

			cTrack.GetVertices().push_back
				({
					{ fEndX, 3.0f, fEndZ + TRACK_WIDTH, 1.0f },
					{ 0.0f, 1.0f, 0.0f, 1.0f },
					{ (fTrackLength / 200.0f), 0 }
			});

			cTrack.GetVertices().push_back
				({
					{ fEndX, 3.0f, fEndZ - TRACK_WIDTH, 1.0f },
					{ 0.0f, 1.0f, 0.0f, 1.0f },
					{ (fTrackLength / 200.0f), 1 }
			});

			cTrack.GetVertices().push_back
				({
					{ fStartX, 3.0f, fStartZ - TRACK_WIDTH, 1.0f },
					{ 0.0f, 1.0f, 0.0f, 1.0f },
					{ -(fTrackLength / 200.0f), 1 }
			});
		}

		m_cTrack = new CRenderMesh(&cTrack, GRAPHICS->GetStandardVS(), GRAPHICS->GetStandardPS());
		m_cTrack->SetTexture(L"../Game/Assets/Art/2D/Textures/Track.dds");
		SnapPosition(m_cTrack->GetTransformMatrix(), m_mWorld);
		m_cTrack->GetTransformMatrix().m[3][1] += 5.0f;

		GRAPHICS->AddRenderMesh(m_cTrack);
	}
}