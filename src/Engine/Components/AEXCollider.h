#pragma once

namespace AEX
{
	enum ECollisionShape
	{
		CSHAPE_AABB = 1,	// 001
		CSHAPE_CIRCLE = 2,	// 010
		CSHAPE_OBB = 4,	// 100
		CSHAPE_INDEX_MAX = (CSHAPE_OBB | CSHAPE_CIRCLE) + 1
	};

	enum EDynamicState
	{
		Dynamic,
		Static
	};

	struct Collider : public IComp
	{
		AEX_RTTI_DECL(Collider, IComp);

		Collider() {}
		~Collider() {}

		// Properties (Public members)
		bool IsGhost = false;
		ECollisionShape  mCollisionShape = CSHAPE_AABB;
		EDynamicState DynamicState = Dynamic;

		// member variables (not serialized)
		bool mbHasCollided;							// check if has collided
		std::vector<Collider *> mCollidedWith;	// if it did, what did it collide with

		void Update();

		// Serialization
		json& operator<< (json&j)  const;
		void operator>> (json&j);
		std::ostream& operator<< (std::ostream & o) const;

		// ImGui
		void OnGui();
	};
}