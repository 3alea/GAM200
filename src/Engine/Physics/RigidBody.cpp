// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:		RigidBody.cpp
//	Purpose:		This file stores the functions used for the RigidBody class.
//	Project:		CS230_alejandro.balea_3
//	Author:			Alejandro Balea Moreno, alejandro.balea (540002118)
// ----------------------------------------------------------------------------
#include <AEEngine.h>
#include "..\Engine.h"
#include "RigidBody.h"

std::vector<Vector2> GetOBBCorners(Transform2D & OBB);

// ----------------------------------------------------------------------------
#pragma region // @PROVIDED

// @PROVIDED
//!----------------------------------------------------------------------------
// \fn		Constructor
// \brief	Sets default values
// ----------------------------------------------------------------------------
RigidBody::RigidBody()
	: GameObject()
	, mInvMass(1.0f)
	, mDrag(0.990f)
{}

// @PROVIDED
//!----------------------------------------------------------------------------
// \fn		Update
// \brief	Updates the game object and its physics
// ----------------------------------------------------------------------------
void RigidBody::Update()
{
	// Do the default update first. 
	GameObject::Update();

	// add gravity
	if (mInvMass)
		AddForce(mGravity / mInvMass);

	// Integrate physics
	Integrate((f32)gAEFrameTime);
}

// ----------------------------------------------------------------------------
#pragma region // @TODO

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
void RigidBody::Integrate(float timeStep)
{
	// check if the inverse mass is not equal to zero
	if (mInvMass)
	{
		// Multiply the acceleration by the inverse mass
		mAcceleration *= mInvMass;
		// Increment the RigidBody's velocity by the acceleration and timeStep
		mVelocity += mAcceleration * timeStep;
		// Decrement the RigidBody's velocity depending on its drag
		mVelocity *= mDrag;
		// Increment the RigidBody position by the velocity and timeStep
		mPosition += mVelocity * timeStep;

		// Reset the acceleration to (0, 0)
		mAcceleration = Vector2();
	}
}

/**************************************************************************/
/*!
  \fn    
    ComputeAABB

  \brief 
    Computes the AABB of the rigidbody. 
	Stores the result in the provided parameters (outPos and outSize).

  \param outPos
    The resultant position of the RigidBody.
    
  \param outSize
    The resultant scale of the RigidBody.
*/
/**************************************************************************/
void RigidBody::ComputeAABB(Vector2 * outPos, Vector2 * outSize)
{
	// Create a switch to check for the collision shape of the RigidBody
	switch (mCollisionShape)
	{
	case CSHAPE_AABB:	// If the RigidBody is an AABB
		// Set the outPos as the current position of the RigidBody
		*outPos = mPosition;
		// Set the outSize as the scale of the RigidBody
		*outSize = mScale;
		break;
	case CSHAPE_CIRCLE:	// If the RigidBody is a circle
		// Set the outPos as the current position of the RigidBody
		*outPos = mPosition;
		// Set the outSize as the diameter of the circle on each side
		*outSize = Vector2(mScale.x * 2, mScale.x * 2);
		break;
	case CSHAPE_OBB:	// If the RigidBody is an OBB
		// Call GetOBBCorners to get the corners of the OBB
		std::vector<Vector2> OBBCorners = GetOBBCorners(Transform2D::Transform2D(mPosition,mScale,mRotation));
		// Get the min and max values of the x and y parameters of the corners
		float minX = min(OBBCorners[0].x, min(OBBCorners[1].x, min(OBBCorners[2].x, OBBCorners[3].x)));
		float maxX = max(OBBCorners[0].x, max(OBBCorners[1].x, max(OBBCorners[2].x, OBBCorners[3].x)));
		float minY = min(OBBCorners[0].y, min(OBBCorners[1].y, min(OBBCorners[2].y, OBBCorners[3].y)));
		float maxY = max(OBBCorners[0].y, max(OBBCorners[1].y, max(OBBCorners[2].y, OBBCorners[3].y)));
		// Set the outPos as the current position of the RigidBody
		*outPos = mPosition;
		// Set the outSize as the 'max - min' of each parameter
		*outSize = Vector2(maxX - minX, maxY - minY);
		break;

	}
}

/**************************************************************************/
/*!
  \fn    
    GetOBBCorners

  \brief 
    Gets all the corners from the inputted OBB.

  \param OBB
    The transform data of the OBB.
 
  \return
    An array of the OBB vertices.
*/
/**************************************************************************/
std::vector<Vector2> GetOBBCorners(Transform2D & OBB)
{
	// Get each normal from the OBB
	Vector2 normals[2];
	normals[0].FromAngle(OBB.mOrientation);
	normals[1].FromAngle(OBB.mOrientation + PI / 2);

	// With its normals, compute its half extents
	Vector2 halfExtents[2];
	halfExtents[0] = normals[0] * OBB.mScale.x / 2;
	halfExtents[1] = normals[1] * OBB.mScale.y / 2;

	// Define an arrat of OBB corners and push each corner into the array
	std::vector<Vector2> OBBCorners;
	OBBCorners.push_back(OBB.mPosition - halfExtents[0] + halfExtents[1]);
	OBBCorners.push_back(OBB.mPosition - halfExtents[0] - halfExtents[1]);
	OBBCorners.push_back(OBB.mPosition + halfExtents[0] - halfExtents[1]);
	OBBCorners.push_back(OBB.mPosition + halfExtents[0] + halfExtents[1]);

	// Return the array of vertices
	return OBBCorners;
}

#pragma endregion
// ----------------------------------------------------------------------------