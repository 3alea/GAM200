#pragma once

// Includes
#include "src\Engine\Components\AEXTransformComp.h"
#include "src\Engine\Components\AEXRigidBody.h"
#include "src\Engine\Components\AEXCollider.h"

// Find De-Finds (sorry for the bad pun)
#define GetTransformComp	dynamic_cast<TransformComp*>(GetOwner()->GetComp("TransformComp"))
#define GetRigidBody		dynamic_cast<RigidBody*>(GetOwner()->GetComp("RigidBody"))	
#define GetCollider			dynamic_cast<Collider*>(GetOwner()->GetComp("Collider"))

#define GetTransByComp(comp) dynamic_cast<TransformComp*>(comp->GetOwner()->GetComp("TransformComp"))
#define GetRigidBodyByComp(comp) dynamic_cast<RigidBody*>(comp->GetOwner()->GetComp("RigidBody"))
#define GetColliderByComp(comp) dynamic_cast<Collider*>(comp->GetOwner()->GetComp("Collider"))

#define GetTransLocalByComp(comp) GetTransByComp(comp)->mLocal