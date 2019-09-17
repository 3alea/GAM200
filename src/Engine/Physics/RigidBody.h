#ifndef PHYSICS_OBJ_H_
#define PHYSICS_OBJ_H_
enum ECollisionShape
{
	CSHAPE_AABB		= 1,	// 001
	CSHAPE_CIRCLE	= 2,	// 010
	CSHAPE_OBB		= 4,	// 100
	CSHAPE_INDEX_MAX = (CSHAPE_OBB | CSHAPE_CIRCLE) + 1
};
struct RigidBody : public GameObject
{
	// physics data
	Vector2	mPrevPosition;
	Vector2	mVelocity;
	Vector2	mAcceleration;
	Vector2	mGravity;
	f32		mInvMass;
	f32		mDrag;

	ECollisionShape mCollisionShape = CSHAPE_AABB;

	// NEW for ASSIGNMENT 4 demo!
	
		bool mbHasCollided;						// check if has collided
		std::vector<RigidBody *> mCollidedWith;	// if it did, what did it collide with

		bool mbIsGhost; 						// if true, it will detect collisions, but not resolve them.

	//  ------------------------

	// constructor
	RigidBody();

	// integrate
	void Integrate(float timeStep);

	// adding force
	void AddForce(Vector2 force);

	// compute AABB (center + size)
	void ComputeAABB(Vector2 * outPos, Vector2 * outSize);

	// update
	virtual void Update();

	// render (Provided)
	virtual void Render();
	virtual void Render(AEMtx33 * pCamMtx);
};



#endif