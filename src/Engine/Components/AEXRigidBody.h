#pragma once
#include "src\Engine\Composition\AEXSerialization.h"

namespace AEX
{
	/****************************************************************
		- Component : RigidBody

		- Dependencies :	Transform
							Collider ?
		- @TODO :
			1) substitute on Update() 0.16667f to gAEFrameTime
			2) (Optional) Implement AngularVelocity
	****************************************************************/
	struct RigidBody : public IComp
	{
		AEX_RTTI_DECL(RigidBody, IComp);

		friend class CollisionSystem;

		// Properties (Public members)
		AEVec2			 Gravity;
		AEVec2			 Velocity;
		f32				 AngularVelocity = 0.0f;
		f32				 Mass = 1.0f;
		f32				 LinearDrag = 0.990f;

		RigidBody() : IComp() {}

		// adding force
		void AddForce(AEVec2 force);
		// Integrate Euler
		void Integrate(f32 timeStep);
		// Update
		void Update();

		// Serialization
		json& operator<<(json&j)  const;
		void operator>>(json&j);
		std::ostream& operator<<(std::ostream & o) const;

		// ImGui
		void OnGui();

	private:
		AEVec2	mAcceleration;
		f32		mInvMass;
	};
}