// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:		LineSegment2D.cpp
//	Purpose:		This file contains the implementation of the following 
//					functions for the LineSegment creation part
//					on the Raycast assignment.
//	Project:		CS230_alejandro.balea_4_2
//	Author:			Alejandro Balea Moreno - alejandro.balea (540002118)
// ----------------------------------------------------------------------------
#include "LineSegment2D.h"
// ---------------------------------------------------------------------------

namespace AEX
{
	/**************************************************************************/
	/*!
	  \fn    
		LineSegment2D (Default constructor)

	  \brief 
		Initializes all values to zero.
	*/
	/**************************************************************************/
	LineSegment2D::LineSegment2D()
	{
		// Set all variables as the default ones:
		// mP0 = (0.0f, 0.0f)
		mP0 = AEVec2();
		// mP1 = (0.0f, 0.0f)
		mP1 = AEVec2();
		// mN = (0.0f, 0.0f)
		mN	= AEVec2();
		// mNdotP0 = 0.0f
		mNdotP0 = 0.0f;
	}

	/**************************************************************************/
	/*!
	  \fn    
		LineSegment2D (Non-default constructor)

	  \brief 
		Constructs a 2D line segment's data using 2 points.

	  \param p0
		The first segment of the Line segment.
    
	  \param p1
		The second segment of the Line segment.
	*/
	/**************************************************************************/
	LineSegment2D::LineSegment2D(const AEVec2 & p0, const AEVec2 &p1)
	{
		// Set mP0 as the inputted p0
		mP0 = p0;
		// Set mP1 as the inputted p1
		mP1 = p1;
		// Compute the edgeVector by substracting the last point of the segment with
		// the first one
		AEVec2 edgeVec = mP1 - mP0;
		// Compute the normal as the negation of the normalized perpendicular vector
		// from edgeVec
		mN = -edgeVec.Perp().Normalize();
		// compute the dot product between N and P0 to avoid computing it 
		// every time it's needed
		mNdotP0 = mN * p0;
	}

	/**************************************************************************/
	/*!
	  \fn    
		StaticPointToStaticLineSegment

	  \brief 
		Determines the distance separating a point from a line.

	  \param P
		A pointer to the point we want to compute with.
    
	  \param LS
		A pointer to the Line segment we want to compute with.

	  \return
		The resultant distance as a float.
	*/
	/**************************************************************************/
	float StaticPointToStaticLineSegment(AEVec2 *P, LineSegment2D *LS)
	{
		// Compute the result of the substraction of each projection (with the point
		// and the other one with one of the points of the Line segment)
		float result = *P * LS->mN - LS->mN * LS->mP0;
		// Return the result
		return result;
	}

	/**************************************************************************/
	/*!
	  \fn    
		PointInLineSegments

	  \brief 
		Determines if a point is contained by all line segments.

	  \param P
		A pointer to the point we want to compute with.
    
	  \param LS
		An array of Line segments we want to compute with.
    
	  \param count
		The number of lines we want to check if they are inside the line segments.
 
	  \return
		The checkup if the point is inside all Line segments.
	*/
	/**************************************************************************/
	bool PointInLineSegments(AEVec2 *P, LineSegment2D *LS, u32 count)
	{
		// Make a for loop, calling StaticPointToStaticLineSegment for each 
		// Line segment. If one of the resultant is begger than 0.0f, return false.
		for (u32 i = 0; i < count; i++)
			if (StaticPointToStaticLineSegment(P, &LS[i]) > 0.0f)
				return false;

		// If everything went ok, the point is inside the array of lines; return true.
		return true;
	}
}