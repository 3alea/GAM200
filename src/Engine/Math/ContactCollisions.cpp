// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:		ContactCollisions.cpp
//	Purpose:		This file stores the functions used for the collisions.
//	Project:		CS230_alejandro.balea_3
//	Author:			Alejandro Balea Moreno, alejandro.balea (540002118)
// ----------------------------------------------------------------------------
#include "Collisions.h"
#include "ContactCollisions.h"
#include <cfloat>	// FLT_MAX
// ---------------------------------------------------------------------------

#define AABB_VERTICES 4

namespace AEX
{
	// ------------------------------------- Function prototypes --------------------------------------------
	void GetMinAndMaxProj(std::vector<AEVec2> & polVertices, AEVec2 & normal, f32 & min, f32 & max);
	bool SAT(std::vector<AEVec2> & pol1Vertices, std::vector<AEVec2> & pol2Vertices, AEVec2 & vertexMin, AEVec2 & vertexMax,
			 Transform * tr1, Transform * tr2, Contact * pResult);

	/**************************************************************************/
	/*!
	  \fn    
		StaticCircleToStaticCircleEx

	  \brief 
		Checks for the collision between two circles. If the collision is true,
		the result of the collision is stored in pResult.

	  \param Center1
		A pointer to the center of the first circle.
    
	  \param Radius1
		The radius of the first circle.
    
	  \param Center2
		The total number of rows in the screen.

	  \param Radius2
		The radius of the second circle.
    
	  \param pResult
		The output from the result of the collision.
 
	  \return
		The checkup if both circles are colliding or not.
	*/
	/**************************************************************************/
	bool StaticCircleToStaticCircleEx(AEVec2 * Center1, float Radius1, AEVec2 * Center2, float Radius2, Contact * pResult)
	{
		// Check if both circles are intersecting
		if (StaticCircleToStaticCircle(Center1, Radius1, Center2, Radius2))
		{
			// Check if pResult is not NULL
			if (pResult)
			{ 
				// Get the distance between both circles
				AEVec2 circleDist = *Center2 - *Center1;
				// Normalize that vector to get the contact normal
				pResult->mNormal = circleDist.Normalize();
				// Compute the penetration between both circles
				pResult->mPenetration = Radius1 + Radius2 - circleDist.Length();
				// Compute the nearest point between both circles
				pResult->mPi = *Center1 + pResult->mNormal * Radius1;
			}
			return true; // There is collision; return true
		}
		return false; // There is no collision; return false
	}

	/**************************************************************************/
	/*!
	  \fn    
		StaticRectToStaticCircleEx

	  \brief 
		Checks for the collision between a circle and an AABB. If the collision is true,
		the result of the collision is stored in pResult.

	  \param Rect
		A pointer to the center of the circle.
    
	  \param Width
		The width of the AABB.
    
	  \param Height
		The height of the AABB.

	  \param Center
		The center of the circle.

	  \param Radius
		The radius of the circle.
    
	  \param pResult
		The output from the result of the collision.
 
	  \return
		The checkup if the circle and the AABB are colliding or not.
	*/
	/**************************************************************************/
	bool StaticRectToStaticCircleEx(AEVec2 * Rect, float Width, float Height, AEVec2 * Center, float Radius, Contact * pResult)
	{
		// Check if the AABB and the circle are intersecting
		if (StaticRectToStaticCirlce(Rect, Width, Height, Center, Radius))
		{
			// Check if pResult is not NULL
			if (pResult)
			{
				// Check if the center of the circle is inside the AABB
				if (StaticPointToStaticRect(Center, Rect, Width, Height))
				{
					// Get the distance between the AABB and the circle
					AEVec2 collDir = *Center - *Rect;
					// Compute the penetration vector
					AEVec2 penVec(Width / 2 - fabsf(collDir.x), Height / 2 - fabsf(collDir.y));
					// Set the penetration as the minimum parameter of penVec + the radius
					pResult->mPenetration = min(penVec.x, penVec.y) + Radius;
					// Check for the normal of collision as a ternaty operator
					pResult->mNormal = (penVec.x < penVec.y) ? AEVec2(1, 0) : AEVec2(0, 1);
					// Check for the position of the objects and set to negative if so
					pResult->mNormal = (collDir * pResult->mNormal < 0) ? -pResult->mNormal : pResult->mNormal;
					// Set the point of intersection as the center of the circle
					pResult->mPi = *Center;
				}
				else
				{
					// Get the nearest point between both RigidBodys by using AEClamp
					AEVec2 nearestPoint; 
					nearestPoint.x = Clamp(Center->x, Rect->x - Width / 2, Rect->x + Width / 2);
					nearestPoint.y = Clamp(Center->y, Rect->y - Height / 2, Rect->y + Height / 2);
					// Get the distance between the circle and the nearest point from the AABB
					AEVec2 distPoints = *Center - nearestPoint;
					// Normalize distPoints to get the normal
					pResult->mNormal = distPoints.Normalize();
					// Compute the penetration of the AABB and the circle
					pResult->mPenetration = Radius - distPoints.Length();
					// Set the point of intersection as the nearest point from the AABB
					pResult->mPi = nearestPoint;
				}
			}
			return true; // There is collision; return true
		}
		return false; // There is no collision; return false
	}

	/**************************************************************************/
	/*!
	  \fn    
		StaticOBBToStaticCircleEx

	  \brief 
		Checks for the collision between a circle and an OBB. If the collision is true,
		the result of the collision is stored in pResult.
	    
	  \param OBB
		A transform where the parameters of the OBB are stored.

	  \param Center
		A pointer to the center of the circle.
    
	  \param Radius
		The radius of the circle.
    
	  \param pResult
		The output from the result of the collision.
 
	  \return
		The checkup if the circle and the OBB are colliding or not.
	*/
	/**************************************************************************/
	bool StaticOBBToStaticCircleEx(Transform * OBB, AEVec2 * Center, float Radius, Contact * pResult)
	{
		// Check if the OBB and the circle are intersecting
		if (OrientedRectToStaticCirlce(&OBB->mTranslation, OBB->mScale.x, OBB->mScale.y, OBB->mOrientation, Center, Radius))
		{
			// Check if pResult is not NULL
			if (pResult)
			{
				// Get the transform and inverse matrix from the OBB
				AEMtx33 transfMtx = AEMtx33::Translate(OBB->mTranslation.x, OBB->mTranslation.y) * AEMtx33::RotRad(OBB->mOrientation);
				AEMtx33 inverseMtx = AEMtx33::RotRad(-OBB->mOrientation) * AEMtx33::Translate(-OBB->mTranslation.x, -OBB->mTranslation.y);

				// Get the parameters from AABB vs circle
				StaticRectToStaticCircleEx(&AEVec2(0, 0), OBB->mScale.x, OBB->mScale.y, &(inverseMtx * *Center), Radius, pResult);

				// Get the normal and point of intersection by multiplying by the normal matrix
				pResult->mNormal = transfMtx.MultVec(pResult->mNormal);
				pResult->mPi = transfMtx * pResult->mPi;
			}
			return true; // There is collision; return true
		}
		return false; // There is no collision; return false
	}

	/**************************************************************************/
	/*!
	  \fn    
		StaticRectToStaticRectEx

	  \brief 
		Checks for the collision between two AABBs. If the collision is true,
		the result of the collision is stored in pResult.

	  \param pos1
		The position of the first AABB.
    
	  \param size1
		The scale of the first AABB.
    
	  \param pos2
		The position of the second AABB.

	  \param size2
		The scale of the second AABB.
    
	  \param pResult
		The output from the result of the collision.
 
	  \return
		The checkup if both AABBs are colliding or not.
	*/
	/**************************************************************************/
	bool StaticRectToStaticRectEx(AEVec2 *pos1, AEVec2 *size1, AEVec2 *pos2, AEVec2 *size2, Contact * pResult)
	{
		// Check if both AABBs are intersecting
		if (StaticRectToStaticRect(pos1, size1->x, size1->y, pos2, size2->x, size2->y))
		{
			// Check if pResult is not NULL
			if (pResult)
			{
				// Get the distance from the AABBs
				AEVec2 distAABB = *pos2 - *pos1;
				// Compute the penetration vector of the AABBs
				AEVec2 penVec;
				penVec.x = (size1->x + size2->x) / 2 - fabsf(distAABB.x);
				penVec.y = (size1->y + size2->y) / 2 - fabsf(distAABB.y);

				// Get the corners of the second AABB
				AEVec2 AABBCorners[AABB_VERTICES];
				AABBCorners[0] = AEVec2(pos2->x - size2->x / 2, pos2->y + size2->y / 2);
				AABBCorners[1] = AEVec2(pos2->x - size2->x / 2, pos2->y - size2->y / 2);
				AABBCorners[2] = AEVec2(pos2->x + size2->x / 2, pos2->y - size2->y / 2);
				AABBCorners[3] = AEVec2(pos2->x + size2->x / 2, pos2->y + size2->y / 2);

				// Create a for loop for the vertices
				for (unsigned i = 0; i < AABB_VERTICES; i++)
				{
					// Find the vertex which is colliding with the other AABB
					if (StaticPointToStaticRect(&AABBCorners[i], pos1, size1->x, size1->y))
					{
						pResult->mPi = AABBCorners[i];
						break;
					}
				}

				// Compute the normal depending of the position of the AABBs and the minimum parameter from penVec
				pResult->mNormal = (fabsf(penVec.x) <= fabsf(penVec.y)) ? AEVec2(1, 0) : AEVec2(0, 1);
				pResult->mNormal = (distAABB * pResult->mNormal < 0) ? -pResult->mNormal : pResult->mNormal;
			
				// Set the penetration as the minimum parameter between	both penVec parameters	
				pResult->mPenetration = min(penVec.x, penVec.y);
			}
			return true; // There is collision; return true
		}
		return false; // There is no collision; return false    
	}


	/**************************************************************************/
	/*!
	  \fn    
		OrientedRectToOrientedRectEx

	  \brief 
		Checks for the collision between two OBBs. If the collision is true,
		the result of the collision is stored in pResult.

	  \param OBB1
		The transform where the parameters of the first OBB are stored.
    
	  \param OBB2
		The transform where the parameters of the second OBB are stored.

	  \param pResult
		The output from the result of the collision.
 
	  \return
		The checkup if both OBBs are colliding or not.
	*/
	/**************************************************************************/
	bool OrientedRectToOrientedRectEx(Transform * OBB1, Transform * OBB2, Contact * pResult)
	{
		// Compute the distance between both OBBs
		AEVec2 dist = OBB2->mTranslation - OBB1->mTranslation;
	
		// Get the normals from each OBB
		AEVec2 normals[4];
		// OBB 1
		normals[0].FromAngle(OBB1->mOrientation);
		normals[1].FromAngle(OBB1->mOrientation + PI/2);
		// OBB 2
		normals[2].FromAngle(OBB2->mOrientation);
		normals[3].FromAngle(OBB2->mOrientation + PI/2);

		// Get the half extents from each OBB
		AEVec2 halfExtents[4];
		// OBB1
		halfExtents[0] = normals[0] * OBB1->mScale.x / 2;
		halfExtents[1] = normals[1] * OBB1->mScale.y / 2;
		// OBB 2
		halfExtents[2] = normals[2] * OBB2->mScale.x / 2;
		halfExtents[3] = normals[3] * OBB2->mScale.y / 2;

		// Get the corners from the first OBB
		AEVec2 AABB1Corners[AABB_VERTICES];
		AABB1Corners[0] = OBB1->mTranslation - halfExtents[0] + halfExtents[1];
		AABB1Corners[1] = OBB1->mTranslation - halfExtents[0] - halfExtents[1];
		AABB1Corners[2] = OBB1->mTranslation + halfExtents[0] - halfExtents[1];
		AABB1Corners[3] = OBB1->mTranslation + halfExtents[0] + halfExtents[1];

		// Get the corners from the second OBB
		AEVec2 AABB2Corners[AABB_VERTICES];
		AABB2Corners[0] = OBB2->mTranslation - halfExtents[2] + halfExtents[3];
		AABB2Corners[1] = OBB2->mTranslation - halfExtents[2] - halfExtents[3];
		AABB2Corners[2] = OBB2->mTranslation + halfExtents[2] - halfExtents[3];
		AABB2Corners[3] = OBB2->mTranslation + halfExtents[2] + halfExtents[3];

		// If pResult is not NULL, initialize mPenetration
		if (pResult)
			pResult->mPenetration = FLT_MAX;
	
		// For each normal
		for (int i = 0; i < AABB_VERTICES; i++)
		{
			// Get the projected length of the normal
			float distLength = (dist.Project(normals[i])).Length();
			// Initailize the length of the projected half extents
			float extentLength = 0.0f;

			// Increment extentLength by the projection of the half extents by the normal
			for (int j = 0; j < AABB_VERTICES; j++)
				extentLength += (halfExtents[j].Project(normals[i])).Length();
		
			// Check if the projected distance of the OBB is larger than the extents' length
			if (distLength > extentLength)
				return false; // There is a separating axis; return false

			// Check if pResult is not NULL and if the penetrion is the minimum one or not
			if (pResult && pResult->mPenetration > extentLength - distLength)
			{
				// Set the normal and penetration as the minimum ones
				pResult->mPenetration = extentLength - distLength;
				pResult->mNormal = normals[i];
				pResult->mNormal = (dist * pResult->mNormal < 0) ? -pResult->mNormal : pResult->mNormal;
			}
		}
	
		for (unsigned i = 0; i < AABB_VERTICES; i++)
		{
			// Check if pResult is not NULL amd if the corners from the second AABB are intersecting with the first AABB
			if (pResult && StaticPointToOrientedRect(&AABB2Corners[i], &OBB1->mTranslation, OBB1->mScale.x, OBB1->mScale.y, OBB1->mOrientation))
			{
				pResult->mPi = AABB2Corners[i];
				return true; // There is collision; return true
			}
		}

		for (unsigned i = 0; i < AABB_VERTICES; i++)
		{
			// Check if pResult is not NULL amd if the corners from the first AABB are intersecting with the second AABB
			if (pResult && StaticPointToOrientedRect(&AABB1Corners[i], &OBB2->mTranslation, OBB2->mScale.x, OBB2->mScale.y, OBB2->mOrientation))
			{
				pResult->mPi = AABB1Corners[i];
				return true; // There is collision; return true
			}
		}
	}


	/**************************************************************************/
	/*!
	  \fn    
		PolygonToPolygon

	  \brief 
		Checks for the collision between two 2D polygons. If the collision is true,
		the result of the collision is stored in pResult.

	  \param p1
		A pointer to the first polygon used to check for collision.
    
	  \param tr1
		The transform of the first polygon.
    
	  \param p2
		A pointer to the second polygon used to check for collision.

	  \param tr2
		The transform of the second polygon.
    
	  \param pResult
		The output from the result of the collision.
 
	  \return
		The checkup if both circles are colliding or not.
	*/
	/**************************************************************************/
	bool PolygonToPolygon(Polygon2D * p1, Transform * tr1, Polygon2D * p2, Transform * tr2, Contact * pResult)
	{
		// Get the polygon
		std::vector<AEVec2> pol1Vertices = p1->GetTransformedVertices(tr1->GetMatrix());
		std::vector<AEVec2> pol2Vertices = p2->GetTransformedVertices(tr2->GetMatrix());

		// Check if pResult is not NULL
		if (pResult)
		{
			// Initialize mPi and mPenetration
			pResult->mPi = AEVec2();
			pResult->mPenetration = FLT_MAX;
		}
		
		// Check if there is a separating axis in one of the normals of the first polygon by calling SAT() 
		for (u32 i = 1; i < pol1Vertices.size(); i++)
			if (!SAT(pol1Vertices, pol2Vertices, pol1Vertices[i - 1], pol1Vertices[i], tr1, tr2, pResult))
				return false; // There is no collision; return false

		// Check if there is a separating axis in one of the normals of the second polygon by calling SAT() 
		for (u32 i = 1; i < pol2Vertices.size(); i++)
			if (!SAT(pol2Vertices, pol1Vertices, pol2Vertices[i - 1], pol2Vertices[i], tr1, tr2, pResult))
				return false; // There is no collision; return false
	
		// Check if there is a separating axis in the last normal of the first polygon by calling SAT() 
		if (!SAT(pol1Vertices, pol2Vertices, pol1Vertices[0], pol1Vertices[pol1Vertices.size() - 1], tr1, tr2, pResult))
			return false; // There is no collision; return false

						  // Check if there is a separating axis in the last normal of the second polygon by calling SAT() 
		if (!SAT(pol2Vertices, pol1Vertices, pol2Vertices[0], pol2Vertices[pol2Vertices.size() - 1], tr1, tr2, pResult))
			return false; // There is no collision; return false

		return true; // There is collision; return true
	}

	/**************************************************************************/
	/*!
	  \fn    
		GetMinAndMaxProj

	  \brief 
		Check for the minimum and maximum values from the projected vertex array.

	  \param polVertices
		The array of vertices which will be projected.
    
	  \param normal
		The normal where the vertices will be projected on.
    
	  \param min
		The minimum value from the proyected vertices.

	  \param max
		The maximum value from the proyected vertices.
	*/
	/**************************************************************************/
	void GetMinAndMaxProj(std::vector<AEVec2> & polVertices, AEVec2 & normal, f32 & min, f32 & max)
	{
		// Create a for loop to access all the vertices from the polygon
		for (u32 i = 0; i < polVertices.size(); i++)
		{
			// Get the vertices and project them to the normal
			f32 projectedVertex = polVertices[i] * normal;

			// Check for minimum and maximum collisions and set when if is true
			if (projectedVertex < min)
				min = projectedVertex;
			if (projectedVertex > max)
				max = projectedVertex;
		}
	}

	/**************************************************************************/
	/*!
	  \fn    
		SAT

	  \brief 
		Computes the Separating Axis Theorem on the vertex arrays.

	  \param pol1Vertices
		The array of vertices of the first polygon.
    
	  \param pol2Vertices
		The array of vertices of the second polygon.
    
	  \param vertexMin
		The first point to compute the normal of the current segment.

	  \param vertexMax
		The second point to compute the normal of the current segment.
    
	  \param tr1
		The transform of the first polygon.

	  \param tr2
		The transform of the second polygon.
    
	  \param pResult
		The output from the result of the collision.
 
	  \return
		The checkup if the polygons are colliding or not on the projected axis.
	*/
	/**************************************************************************/
	bool SAT(std::vector<AEVec2> & pol1Vertices, std::vector<AEVec2> & pol2Vertices, AEVec2 & vertexMin, AEVec2 & vertexMax,
			 Transform * tr1, Transform * tr2, Contact * pResult)
	{
		// Compute the edge vector
		AEVec2 currVec = vertexMax - vertexMin;
		// Compute the normal of that edge vector
		AEVec2 currNormal = currVec.Perp().NormalizeThis();
		// Initialize the min and max values
		f32 pol1Min = FLT_MAX, pol1Max = -FLT_MAX, pol2Min = FLT_MAX, pol2Max = -FLT_MAX;

		// Call GetMinAndMaxProj to get the min and maximum values of the polygons
		GetMinAndMaxProj(pol1Vertices, currNormal, pol1Min, pol1Max);
		GetMinAndMaxProj(pol2Vertices, currNormal, pol2Min, pol2Max);

		// Check if there is a separating axis
		if (pol1Max < pol2Min || pol2Max < pol1Min)
			return false; // There is a separating axis; return false
		
		// Get the penetration of the polygon depending on the position of the polygons
		f32 polPen = (pol1Max >= pol2Min) ? pol1Max - pol2Min : pol2Max - pol1Min;

		// Check if pResult is not NULL and its penetration is bigger than the current penetration
		if (pResult && pResult->mPenetration > polPen)
		{
			// set mPenetration and mNormal to its current parameters
			pResult->mPenetration = polPen;
			pResult->mNormal = (currNormal * (tr2->mTranslation - tr1->mTranslation) >= 0) ? currNormal : -currNormal;
		}

		return true; // Continue checking!
	}
}
