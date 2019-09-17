#pragma once
// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:		CollisionSystem.cpp
//	Purpose:		This file stores the functions used for the collision system.
//	Project:		CS230_alejandro.balea_3
//	Author:			Alejandro Balea Moreno, alejandro.balea (540002118)
// ----------------------------------------------------------------------------
#include "AEXCollisionSystem.h"
#include "src\Engine\Components\AEXComponents.h"

namespace AEX
{
	void PushIfNotDuplicate(std::vector<Collider *> & vec, Collider * RB);

	// @PROVIDED
	//!----------------------------------------------------------------------------
	// \fn		Init
	// \brief	Called at beginning of game. Sets default and adds the collision 
	//			test to the function pointer map. 
	// ----------------------------------------------------------------------------
	bool CollisionSystem::Initialize()
	{
		// default
		mCollisionIterations = 5;

		for (u32 i = 0; i < CSHAPE_INDEX_MAX; ++i)
			mCollisionTests[i] = NULL;

		mCollisionTests[CSHAPE_CIRCLE | CSHAPE_CIRCLE] = CollideCircles;
		mCollisionTests[CSHAPE_AABB | CSHAPE_AABB] = CollideAABBs;
		mCollisionTests[CSHAPE_OBB | CSHAPE_OBB] = CollideOBBs;
		mCollisionTests[CSHAPE_CIRCLE | CSHAPE_AABB] = CollideAABBToCircle;
		mCollisionTests[CSHAPE_OBB | CSHAPE_AABB] = CollideOBBs;
		mCollisionTests[CSHAPE_CIRCLE | CSHAPE_OBB] = CollideOBBToCircle;

		return true;
	}
	// @PROVIDED
	//!----------------------------------------------------------------------------
	// \fn		Update
	// \brief	Frame update. For now, only call ClearBodies.
	// ----------------------------------------------------------------------------
	void CollisionSystem::Update()
	{
		CollideAllBodies();
	}
	// @PROVIDED
	//!----------------------------------------------------------------------------
	// \fn		Shutdown
	// \brief	Shutdown operation at the end of a game. 
	//			For now, only call ClearBodies.
	// ----------------------------------------------------------------------------
	void CollisionSystem::Shutdown()
	{
		ClearBodies();
	}

	// @PROVIDED
	//!----------------------------------------------------------------------------
	// \fn		AddRigidBody
	// \brief	Adds the rigidbody to the appropriate container (based on is_dynamic). 
	// ----------------------------------------------------------------------------
	void CollisionSystem::AddRigidBody(Collider* obj, bool Collider)
	{
		/*if (obj->mOwner->)
			mDynamicBodies.push_back(obj);
		else
			mStaticBodies.push_back(obj);*/
	}
	// @PROVIDED
	//!----------------------------------------------------------------------------
	// \fn		RemoveRigidBody
	// \brief	Removes the rigidbody from the containers (brute force). 
	// ----------------------------------------------------------------------------
	void CollisionSystem::RemoveRigidBody(Collider *obj)
	{
		mDynamicBodies.remove(obj);
		mStaticBodies.remove(obj);
	}
	// @PROVIDED
	//!----------------------------------------------------------------------------
	// \fn		ClearBodies
	// \brief	Clears the static and dynamic body containers.
	// ----------------------------------------------------------------------------
	void CollisionSystem::ClearBodies()
	{
		mDynamicBodies.clear();
		mStaticBodies.clear();
	}

	// @PROVIDED
	//!----------------------------------------------------------------------------
	// \fn		GetCollisionFn
	// \brief	returns the appropriate collision function based on the shape of the 
	//			passed rigid bodies, b1 and b2.
	// ----------------------------------------------------------------------------
	CollisionFn CollisionSystem::GetCollisionFn(Collider * b1, Collider * b2)
	{
		u32 collision_index = b1->mCollisionShape | b2->mCollisionShape;
		return mCollisionTests[collision_index];
	}

	bool CollideCircles(Collider* body1, Collider* body2, Contact * c)
	{
		AEVec2 pos1 = GetTransLocalByComp(body1).mTranslation;
		AEVec2 pos2 = GetTransLocalByComp(body2).mTranslation;

		return StaticCircleToStaticCircleEx(&pos1, GetTransLocalByComp(body1).mScale.x, &pos2, GetTransLocalByComp(body2).mScale.x, c);
	}
	bool CollideAABBs(Collider* body1, Collider* body2, Contact * c)
	{
		AEVec2 p1 = GetTransLocalByComp(body1).mTranslation, s1 = GetTransLocalByComp(body1).mScale,
			   p2 = GetTransLocalByComp(body2).mTranslation, s2 = GetTransLocalByComp(body2).mScale;
		return StaticRectToStaticRectEx(&p1, &s1, &p2, &s2, c);
	}
	bool CollideOBBs(Collider* body1, Collider* body2, Contact * c)
	{
		return OrientedRectToOrientedRectEx(&GetTransLocalByComp(body1), &GetTransLocalByComp(body2), c);
	}
	bool CollideAABBToCircle(Collider* body1, Collider* body2, Contact * c)
	{
		// which is which
		Collider * rect		= body1->mCollisionShape == CSHAPE_AABB ? body1 : body2;
		Collider * circle	= body1->mCollisionShape == CSHAPE_CIRCLE ? body1 : body2;

		if (StaticRectToStaticCircleEx(&AEVec2(GetTransLocalByComp(rect).mTranslation), GetTransLocalByComp(rect).mScale.x, GetTransLocalByComp(rect).mScale.y, &AEVec2(GetTransLocalByComp(circle).mTranslation), GetTransLocalByComp(circle).mScale.x, c))
		{
			if (circle == body1) // flip normal to match our convention
				c->mNormal = -c->mNormal;
			return true;
		}
		return false;
	}
	bool CollideOBBToCircle(Collider* body1, Collider* body2, Contact * c)
	{

		// which is which
		Transform obb = body1->mCollisionShape == CSHAPE_OBB
			? Transform(GetTransLocalByComp(body1))
			: Transform(GetTransLocalByComp(body2));

		Collider * circle = body1->mCollisionShape == CSHAPE_CIRCLE ? body1 : body2;
		if (StaticOBBToStaticCircleEx(&obb, &AEVec2(GetTransLocalByComp(circle).mTranslation), GetTransLocalByComp(circle).mScale.x, c))
		{
			if (circle == body1) // flip normal to match our convention
				c->mNormal = -c->mNormal;
			return true;
		}
		return false;
	}

	#pragma endregion
	// ----------------------------------------------------------------------------

	// ----------------------------------------------------------------------------
	#pragma region // @TODO

	/**************************************************************************/
	/*!
	  \fn    
		ResolveContactPenetration

	  \brief 
		Given the contact data, resolves the penetration of the two given bodies.

	  \param body1
		The first RigidBody to be tampered with.
    
	  \param body2
		The second RigidBody to be tampered with.
    
	  \param contact
		The contact data as an input.
	*/
	/**************************************************************************/
	void CollisionSystem::ResolveContactPenetration(Collider * body1, Collider * body2, Contact * contact)
	{
		// Compute the contact penetration as a vector
		AEVec2 contactPen = contact->mNormal * contact->mPenetration;

		// Get the massInfluence of each RigidBody
		float invMass1 = GetRigidBodyByComp(body1)->mInvMass / (GetRigidBodyByComp(body1)->mInvMass + GetRigidBodyByComp(body2)->mInvMass);
		float invMass2 = GetRigidBodyByComp(body2)->mInvMass / (GetRigidBodyByComp(body1)->mInvMass + GetRigidBodyByComp(body2)->mInvMass);

		// Calculate the position of each RigidBody depending on contactPen and each massInfluence
		GetTransLocalByComp(body1).mTranslation -= contactPen * invMass1;
		GetTransLocalByComp(body2).mTranslation += contactPen * invMass2;

		// Reset the acceleration of each RigidBody
		GetRigidBodyByComp(body1)->mAcceleration = AEVec2();
		GetRigidBodyByComp(body2)->mAcceleration = AEVec2();
	}

	/**************************************************************************/
	/*!
	  \fn    
		ResolveContactVelocity

	  \brief 
		Given the contact data, resolves the velocity of the two given bodies.

	  \param body1
		The first RigidBody to be tampered with.
    
	  \param body2
		The second RigidBody to be tampered with.
    
	  \param contact
		The contact data as an input.
	*/
	/**************************************************************************/
	void CollisionSystem::ResolveContactVelocity(Collider * body1, Collider * body2, Contact * contact)
	{
		// Get the velocity differential (Relative velocity)
		AEVec2 relVel = GetRigidBodyByComp(body2)->Velocity - GetRigidBodyByComp(body1)->Velocity;
		// Get the separating velocity
		float sep = relVel * contact->mNormal;

		// If the separating velocity is higher than 0, they are alreading separating, so return
		if (sep > 0)
			return;

		// Compute the mass influence pf each RigidBody
		float massInfl1 = GetRigidBodyByComp(body1)->mInvMass / (GetRigidBodyByComp(body1)->mInvMass + GetRigidBodyByComp(body2)->mInvMass);
		float massInfl2 = GetRigidBodyByComp(body2)->mInvMass / (GetRigidBodyByComp(body1)->mInvMass + GetRigidBodyByComp(body2)->mInvMass);

		float primeSep = -sep * DFLT_RESTITUTION;
		float deltaSep = primeSep - sep;

		// Compute the new velocity of each RigidBody
		GetRigidBodyByComp(body1)->Velocity -= contact->mNormal * deltaSep * (massInfl1 * massInfl1);
		GetRigidBodyByComp(body2)->Velocity += contact->mNormal * deltaSep * (massInfl2 * massInfl2);
	}

	/**************************************************************************/
	/*!
	  \fn    
		CollideAllBodies

	  \brief 
		Performs brute force collision detection/resolution between dynamic and static bodies.
	*/
	/**************************************************************************/
	void CollisionSystem::CollideAllBodies()
	{
		// Initialize mCollisionsThisFrame to 0
		mCollisionsThisFrame = 0;

		//------------------------------------------------
		// @NEW - Assignment 4_4
		// Define an iterator for the mDynamicBodies list which will go through
		std::list<Collider *>::iterator itA;

		// Clear the mCollidedWith vectors (For both mDynamicBodies and mStaticBodies)
		for (itA = mDynamicBodies.begin(); itA != mDynamicBodies.end(); itA++)
			(*itA)->mCollidedWith.clear();
		for (itA = mStaticBodies.begin(); itA != mStaticBodies.end(); itA++)
			(*itA)->mCollidedWith.clear();
		//------------------------------------------------

		// Compute the collisions by n iterations
		for (u32 n = 0; n < mCollisionIterations; n++)
		{
			// Use itA to go through the mDynamicBodies list
			for (itA = mDynamicBodies.begin(); itA != mDynamicBodies.end(); itA++)
			{
				// Define another iterator; this time for the mDynamicBodies list
				std::list<Collider *>::iterator itB = itA;
				// increment by 1 itB
				itB++;
				// Use itB to go through the mDynamicBodies list
				for (; itB != mDynamicBodies.end(); itB++)
				{
					// Get the collision function calling GetCollisionFn
					CollisionFn colFn = GetCollisionFn(*itA, *itB);
					// Initialize contactResolution
					Contact contactResolution;

					// Check if both RigidBodies are colliding
					if (colFn(*itA, *itB, &contactResolution))
					{
						//------------------------------------------------
						// @NEW - Assignment 4_4
						(*itA)->mbHasCollided = true;
						(*itB)->mbHasCollided = true;
						PushIfNotDuplicate((*itA)->mCollidedWith, *itB);
						PushIfNotDuplicate((*itB)->mCollidedWith, *itA);

						if ((*itA)->IsGhost || (*itB)->IsGhost)
							continue;
						//------------------------------------------------

						// Resolve the contact penetation between each body
						ResolveContactPenetration(*itA, *itB, &contactResolution);

						// If it's the first iteration, resolve the contact velocity between each body and increment mCollisionsThisFrame
						if (n == 0)
						{
							ResolveContactVelocity(*itA, *itB, &contactResolution);
							mCollisionsThisFrame++;
						}
					}
				}

				// Use itB to go through the mStaticBodies list
				for (itB = mStaticBodies.begin(); itB != mStaticBodies.end(); itB++)
				{
					// Get the collision function calling GetCollisionFn
					CollisionFn colFn = GetCollisionFn(*itA, *itB);
					// Initialize contactResolution
					Contact contactResolution;

					// Check if both RigidBodies are colliding
					if (colFn(*itA, *itB, &contactResolution))
					{
						//------------------------------------------------
						// @NEW - Assignment 4_4
						(*itA)->mbHasCollided = true;
						(*itB)->mbHasCollided = true;
						(*itA)->mCollidedWith.push_back(*itB);
						(*itB)->mCollidedWith.push_back(*itA);

						if ((*itA)->IsGhost || (*itB)->IsGhost)
							continue;
						//------------------------------------------------

						// Resolve the contact penetation between each body
						ResolveContactPenetration(*itA, *itB, &contactResolution);

						// If it's the first iteration, resolve the contact velocity between each body and increment mCollisionsThisFrame
						if (n == 0)
						{
							ResolveContactVelocity(*itA, *itB, &contactResolution);
							mCollisionsThisFrame++;
						}
					}
				}
			}
		}
	}
	//------------------------------------------------
	// @NEW - Assignment 4_4
	/**************************************************************************/
	/*!
	  \fn    
		PushIfNotDuplicate

	  \brief 
		Given the inputted parameters, check if the RigidBody* is included in
		the vector. If not, pushes back the RigidBody* to the list.

	  \param vec
		The RigidBody pointer vector we want to check in.
    
	  \param RB
		The pointer to a RigidBody we want to compute with.
	*/
	/**************************************************************************/
	void PushIfNotDuplicate(std::vector<Collider *> & vec, Collider * RB)
	{
		for (u32 i = 0; i < vec.size(); i++)
			if (vec[i] == RB)
				return;

		vec.push_back(RB);
	}
	//------------------------------------------------

	#pragma endregion
	// ----------------------------------------------------------------------------
}