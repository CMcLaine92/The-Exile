/***********************************************
* Filename:  		PressurePlate.cpp
* Date:      		9/1/2015
* Mod. Date:
* Mod. Initials:	NS
* Author:    		Numi Stefansson
* Purpose:			Pressure plate object, for each trap that needs it.
************************************************/
#include "PressurePlate.h"

#include "../../Game/Source/Traps/SpikeTrap.h"
#include "../../Game/Source/Traps/FireTrap.h"
#include "../../Engine/Core/Messages/AddObjectMessage.h"
#include "../../Engine/Core/Messages/RemoveObjectMessage.h"
#include "../../Engine/Asset Manager/AssetManager.h"
#include "../../Engine/Core/Collision/AABB.h"
#include "../../Engine/Audio/Source/AudioSystemWwise.h"
#include "../../Engine/Audio/Source/Wwise_IDs.h"

CPressurePlate::CPressurePlate( XMFLOAT3 _location, bool _used ) : IObject( "PressurePlate" )
{
	AudioSystemWwise::Get()->RegisterEntity( this, "PressurePlate" );
	XMFLOAT3 newLocation = _location;
	newLocation.y = 1;
	SetPosition( newLocation );

	CMesh* vPressureMesh = CAssetManager::GetInstance()->GetPrefabMesh("PressurePlate");

	SetRenderMesh( new CRenderMesh( vPressureMesh, GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Bricks.dds" ) );
	m_cpRenderMesh->SetNormals(L"../Game/Assets/Art/2D/Normal Maps/Bricks.dds");
	m_cpRenderMesh->GetSpecular() = 1.5f;
	GRAPHICS->AddRenderMesh( m_cpRenderMesh );

	//tempFloor->SetRenderMesh( new CRenderMesh( tempMesh->GetIndices(), tempMesh->GetVertices(), GRAPHICS->GetVertexShader(), GRAPHICS->GetNormalMapPS(), nullptr, nullptr, nullptr, L"../Game/Assets/Art/2D/Textures/Maze_Ground.dds" ) );
	//tempFloor->GetRenderMesh()->SetNormals( L"../Game/Assets/Art/2D/Normal Maps/Rocky.dds" );
	//tempFloor->GetRenderMesh()->GetSpecular() = 1.5f;

	//if (_used)
	{
		AddCollider( new CCollider( false, Bounds::AABB, vPressureMesh->GetVertices(), _used,false ) );

		for( size_t i = 0; i < m_pvColliders.size(); i++ )
		{
			( (CAABB*)m_pvColliders[i]->GetBounds() )->SetCenter( { m_mWorld._41, m_mWorld._42, m_mWorld._43 } );
		}

	}

	CAddObjectMessage* addObj = new CAddObjectMessage( this, CObjectManager::Static );
	addObj->Send();
}

CPressurePlate::~CPressurePlate()
{
	m_ipParent = nullptr;

	AudioSystemWwise::Get()->UnRegisterEntity( this );

	//CRemoveObjectMessage* removeObj = new CRemoveObjectMessage( this, CObjectManager::Static, false );
	//removeObj->Send();
}

void CPressurePlate::Update()
{

}

void CPressurePlate::CollisionResponse( IObject * _cCollidedObject )
{
	if( _cCollidedObject->GetTag() == "Player" || _cCollidedObject->GetTag() == "Skeleton" )
	{

		if( m_ipParent->GetTag() == "FireTrap" )
		{
			CFireTrap* parentTrap = reinterpret_cast<CFireTrap*>( m_ipParent );

			if( !parentTrap->GetActive() )
			{
				parentTrap->MakeActive();

				if( !m_bFireTrapSoundPlayed )
					AudioSystemWwise::Get()->PostEvent( AK::EVENTS::PRESSUREPLATE, this );

				m_bFireTrapSoundPlayed = true;
			}
			else
				m_bFireTrapSoundPlayed = false;
		}
		else if( m_ipParent->GetTag() == "SpikeTrap" )
		{
			CSpikeTrap* parentTrap = reinterpret_cast<CSpikeTrap*>( m_ipParent );

			// Activate the parent trap if not activated
			if( !parentTrap->GetActive() && !parentTrap->GetReset() && parentTrap->GetPressurePlateActivated() )
			{
				AudioSystemWwise::Get()->PostEvent( AK::EVENTS::PRESSUREPLATE, this );
				parentTrap->MakeActive();
			}
		}
	}
}