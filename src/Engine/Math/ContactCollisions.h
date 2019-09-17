// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:		ContactCollisions.h
//	Purpose:		Declares static containment and overlap tests between 
//					different primitives.
//	Project:		cs230_tkomair_3
//	Author:			Thomas Komair (tkomair@digipen.edu)
// ----------------------------------------------------------------------------
#ifndef CONTACT_COLLISION_H
#define CONTACT_COLLISION_H
// ----------------------------------------------------------------------------

#include "src\Engine\AEX.h"
#include "src\Engine\Math\Polygon2D.h"

namespace AEX
{
	struct Contact
	{
		AEVec2	mPi;
		AEVec2	mNormal;
		float	mPenetration;
	};
	
	//! ---------------------------------------------------------------------------
	// \fn		StaticCircleToStaticCircleEx
	// \brief	Checks overlap between two circles. If an overla exists, it stores
	//			the contact data into pResult.
	//
	// \details	
	//			- You must compute ALL the contact data.
	//			- You should check for pResult not being equal to NULL.
	//			- Normal must be from Circle1 to Circle2.
	//			- Point of intersection must be on the edge of Circle1. It represents
	//			  The point where both circles touch (after collision is resolved).
	// 
	//  \return	true if the shapes overlap, false otherwise
	// ---------------------------------------------------------------------------
	bool StaticCircleToStaticCircleEx(AEVec2 * Center1, float Radius1, AEVec2 * Center2, float Radius2, Contact * pResult);
	
	//! ---------------------------------------------------------------------------
	// \fn		StaticRectToStaticCircleEx
	// \brief	Checks overlap between a rectangle and a circle.If an overlap exists, 
	// 			it stores the contact data into pResult.
	//
	// \details	
	//			- You must handle the case when the circle is inside the rectangle
	//			  as described in class. 
	//			- You should check for pResult not being equal to NULL.
	//			- You must compute ALL the contact data.
	//			- You must assume that the contact viewer is the rectangle. 
	//			  (i.e. normal goes from Box to Circle).
	// 
	//  \return	true if the shapes overlap, false otherwise
	// ---------------------------------------------------------------------------
	bool StaticRectToStaticCircleEx(AEVec2 * rectPos, float rectWidth, float rectHeight, AEVec2 * circlePos, float radius, Contact * pResult);
	
	//! ---------------------------------------------------------------------------
	// \fn		StaticOBBToStaticCircleEx
	// \brief	Checks overlap between and oriented box and a circle.If an overlap exists, 
	// 			it stores the contact data into pResult.
	//
	// \details	
	//			- You should use StaticrectToStaticCircleEx.
	//			- You must compute ALL the contact data.
	//			- You should check for pResult not being equal to NULL.
	//			- You must assume that the contact viewer is the OBB. (i.e. normal goes from 
	//			  Box to Circle).
	// 
	//  \return	true if the shapes overlap, false otherwise
	// ---------------------------------------------------------------------------
	bool StaticOBBToStaticCircleEx(Transform * OBB, AEVec2 * Center, float Radius, Contact * pResult);
	
	//! ---------------------------------------------------------------------------
	// \fn		StaticRectToStaticRectEx
	// \brief	Checks overlap between two AABB , using the simplified Separating Axis
	// 			Theorem described in class. If an overlap exists, it stores the 
	// 			contact data into pResult.
	//
	// \details	
	//			- You must compute ALL the contact data.
	//			- You should check for pResult not being equal to NULL.
	//			- To determine the point of collision, use the following logic:
	//				* check if any corner of box2 is inside of box1, if so use 
	//				  it as the point of interesection. 
	//				* otherwise, check if any corner of box1 is inside of box2, use
	//				  it as the point of intersection.
	// 
	//  \return	true if the shapes overlap, false otherwise
	// ---------------------------------------------------------------------------
	bool StaticRectToStaticRectEx(AEVec2 *pos1, AEVec2 *size1, AEVec2 *pos2, AEVec2 *size2, Contact * pResult);
	
	//! ---------------------------------------------------------------------------
	// \fn		OrientedRectToOrientedRectEx
	// \brief	Checks overlap between two oriented box, using the Separating Axis
	// 			Theorem described in class. If an overlap exists, it stores the 
	// 			contact data into pResult.
	//
	// \details	
	//			- You must compute ALL the contact data.
	//			- You should check for pResult not being equal to NULL
	// 
	//  \return	true if the shapes overlap, false otherwise
	// ---------------------------------------------------------------------------
	bool OrientedRectToOrientedRectEx(Transform * OBB1, Transform	 * OBB2, Contact * pResult);

	//! ---------------------------------------------------------------------------
	// \fn		PolygonToPolygon
	// \brief	Checks overlap between two oriented box, using the Separating Axis
	// 			Theorem described in class. If an overlap exists, it stores the 
	// 			contact data into pResult.
	//
	// \details	
	//			- You should check for pResult not being equal to NULL
	// 
	//  \return	true if the shapes overlap, false otherwise
	// ---------------------------------------------------------------------------
	bool PolygonToPolygon(Polygon2D * p1, Transform * tr1, Polygon2D * p2, Transform * tr2, Contact * pResult);
}


#endif
