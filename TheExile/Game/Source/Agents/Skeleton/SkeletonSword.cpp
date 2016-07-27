#include "SkeletonSword.h"
#include "../../../../Engine/Asset Manager/AssetManager.h"


CSkeletonSword::CSkeletonSword() : CWeapon(CAssetManager::GetInstance()->GetPrefabMesh("OrcDagger"), L"../Game/Assets/Art/2D/Textures/OrcDaggerDiffuse.dds", nullptr, 10, 1.0f, "SkeletonSword")
{

}