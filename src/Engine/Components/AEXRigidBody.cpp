#include <aexmath\aexmath.h>
#include "..\Composition\AEXSerialization.h"
#include "..\Composition\AEXComponent.h"
#include "..\Composition\AEXGameObject.h"
#include "..\Components\AEXComponents.h"
#include "AEXRigidBody.h"
#include "../Imgui/imgui.h"

namespace AEX
{
	/**************************************************************************/
	/*!
	  \fn    
	    Integrate
	
	  \brief 
	    Integrates using symplectic Euler and clears the forces afterward. 
	
	  \param timeStep
	    The parameter used to regulate the RigidBody integration by the time.
	*/
	/**************************************************************************/
	void RigidBody::Integrate(f32 timeStep)
	{
		// check if the inverse mass is not equal to zero
		if (mInvMass)
		{
			// Multiply the acceleration by the inverse mass
			mAcceleration *= mInvMass;
			// Increment the RigidBody's velocity by the acceleration and timeStep
			Velocity += mAcceleration * timeStep;
			// Decrement the RigidBody's velocity depending on its drag
			Velocity *= LinearDrag;
			GetTransformComp->mLocal.mTranslation += Velocity * timeStep; // Increment the RigidBody position by the velocity and timeStep
			// Reset the acceleration to (0, 0)
			mAcceleration = AEVec2();
		}
	}

	/**************************************************************************/
	/*!
	  \fn    
	    AddForce
	
	  \brief 
	    Adds a force to the object
	
	  \param force
	    The direction of the force
	*/
	/**************************************************************************/
	void RigidBody::AddForce(AEVec2 force)
	{
		mAcceleration += force;
	}

	/**************************************************************************/
	/*!
	  \fn    
	    Update
	
	  \brief 
	    Updates function for the RigidBody
	*/
	/**************************************************************************/
	void RigidBody::Update()
	{
		// add gravity
		if (mInvMass)
			AddForce(Gravity / mInvMass);

		// Integrate physics
		Integrate(0.16667f);
	}

	json & RigidBody::operator<<(json & j) const
	{
		j["gravity"] << Gravity;
		j["velocity"] << Velocity;
		j["angularVelocity"] << AngularVelocity;
		j["mass"] << Mass;
		j["linearDrag"] << LinearDrag;
		return j;
	}

	void RigidBody::operator>>(json & j)
	{
		if (j.find("gravity") != j.end())
			j["gravity"] >> Gravity;
		if (j.find("velocity") != j.end())
			j["velocity"] >> Velocity;
		if (j.find("angularVelocity") != j.end())
			j["angularVelocity"] >> AngularVelocity;
		if (j.find("mass") != j.end())
			j["mass"] >> Mass;
		if (j.find("linearDrag") != j.end())
			j["linearDrag"] >> LinearDrag;
	}

	std::ostream & RigidBody::operator<<(std::ostream & o) const
	{
		json j;
		this->operator<<(j);
		return o << j;
	}

	void RigidBody::OnGui()
	{
		if (ImGui::CollapsingHeader("RigidBody"))
		{
			ImGui::Text("Mass");
			ImGui::InputFloat("Mass", &Mass);

			f32* gravityArray[2]{ &Gravity.x, &Gravity.y };
			ImGui::Text("Gravity");
			ImGui::InputFloat2("Gravity", *gravityArray);
			
			f32* velocityArray[2]{ &Velocity.x, &Velocity.y };
			ImGui::Text("Velocity");
			ImGui::InputFloat2("Velocity", *velocityArray);

			ImGui::Text("(WIP) Angular Velocity");
			ImGui::InputFloat("Angular Velocity", &AngularVelocity);
			
			ImGui::Text("Linear Drag");
			ImGui::InputFloat("Linear Drag", &LinearDrag);
		}
	}
}
