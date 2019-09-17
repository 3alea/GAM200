#ifndef COLLISION_H_
#define COLLISION_H_
// ---------------------------------------------------------------------------
#include "../Math/Collisions.h"
#include "../Math/ContactCollisions.h"
#include "src\Engine\Components\AEXCollider.h"

// Collision restitution for velocity resolution
#define DFLT_RESTITUTION 0.908f;

namespace AEX
{
	bool CollideCircles(Collider* body1, Collider* body2, Contact * c);
	bool CollideAABBs(Collider* body1, Collider* body2, Contact * c);
	bool CollideOBBs(Collider* body1, Collider* body2, Contact * c);
	bool CollideAABBToCircle(Collider* body1, Collider* body2, Contact * c);
	bool CollideOBBToCircle(Collider* body1, Collider* body2, Contact * c);

	// typedef for function pointer CollisionFn
	typedef bool(*CollisionFn)(Collider*, Collider*, Contact *);

	struct CollisionSystem : public ISystem
	{
		AEX_RTTI_DECL(CollisionSystem, ISystem);
		AEX_SINGLETON(CollisionSystem);
		// ------------------------------------------------------------------------
		// Member Variables
		std::list<Collider*> mStaticBodies;
		std::list<Collider*> mDynamicBodies;

		// Collision iterations
		u32 mCollisionIterations;
		u32 mCollisionsThisFrame; // collisions this frame

								  // Collision Tests -They are added to the collision system at initialize. 
								  // (see CollisionSystem::Init) for more details.
		CollisionFn mCollisionTests[CSHAPE_INDEX_MAX];

		// ------------------------------------------------------------------------
		// Member Functions

		// System Functions
		bool Initialize();
		void Update();
		void Shutdown();

		// Rigid Body Management
		void AddRigidBody(Collider* obj, bool is_dynamic);
		void RemoveRigidBody(Collider *obj);
		void ClearBodies();

		// findeing the collision tests
		CollisionFn GetCollisionFn(Collider * b1, Collider * b2);

		// Collides and resolve all rigidbodies 
		void CollideAllBodies();

		// Exposed solver
		void ResolveContactPenetration(Collider * obj1, Collider * obj2, Contact * contact);
		void ResolveContactVelocity(Collider * obj1, Collider * obj2, Contact * contact);
	};

	CollisionSystem::CollisionSystem() : ISystem() {}
	// ---------------------------------------------------------------------------
}

#endif