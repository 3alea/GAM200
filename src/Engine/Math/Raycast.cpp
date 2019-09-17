// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:		Raycast.cpp
//	Purpose:		This file contains the implementation of some of 
//					the following functions used for the Raycast assignment.
//	Project:		CS230_alejandro.balea_4_2
//	Author:			Alejandro Balea Moreno - alejandro.balea (540002118)
// ----------------------------------------------------------------------------
#include "Raycast.h"

namespace AEX
{
	// ---------------------------- HELPER FUNCTIONS ---------------------------- //
	/**************************************************************************/
	/*!
	  \fn    
		quadratic

	  \brief 
		Computes the quadratic formula depending on an a, b and c parameter.

	  \param a
		The a parameter of the quadratic function.
    
	  \param b
		The b parameter of the quadratic function.
    
	  \param c
		The c parameter of the quadratic function.

	  \param min
		The lowest result of the function.

	  \param max
		The highest result of the function.
 
	  \return
		The checkup if there is an existing result.
	*/
	/**************************************************************************/
	bool quadratic(f32 a, f32 b, f32 c, f32 & min, f32 & max)
	{
		// Calculate the result inside the square root
		f32 insideSqrt = (b * b) - (4 * a * c);
		// If insideSqrt is lower than 0, there is no solution; return false.
		if (insideSqrt < 0)
			return false;

		// Compute the min and max results of the quadratic formula
		min = (-b - sqrt(insideSqrt)) / (2 * a);
		max = (-b + sqrt(insideSqrt)) / (2 * a);

		// return true, meaning that there is an existing result
		return true;
	}

	/**************************************************************************/
	/*!
	  \fn    
		RayCastLine

	  \brief 
		Performs a raycast test against a line.

	  \param origin
		The origin of the ray.
    
	  \param dir
		The direction of the ray.
    
	  \param line
		The Line segment to raycast against.

	  \param outPi
		The point of intersection between the ray and the Line segment.
 
	  \return
		The time of intersection as a factor of dir.
	*/
	/**************************************************************************/
	f32 RayCastLine(const AEVec2 & origin, const AEVec2 & dir, const LineSegment2D & line, AEVec2 * outPi)
	{
		// Compute the time of intersection
		f32 time = (line.mNdotP0 - line.mN * origin) / (line.mN * dir);
		// Check if outPos is not NULL
		if (outPi)
		{
			// Compute the resultant position of the ray
			*outPi = origin + dir * time;
			// Compute the distance between the ray and the two segments of the line
			AEVec2 vec0 = *outPi - line.mP0;
			AEVec2 vec1 = *outPi - line.mP1;
			// If the dot product between these vectors are less than 0.0f, return the computed time
			if (vec0 * vec1 < 0.0f)
				return time;
		}

		// If not, return a negative number (error)
		return -1.0f;
	}
	
	/**************************************************************************/
	/*!
	  \fn    
		RayCastLine

	  \brief 
		Performs a raycast test against a line.

	  \param origin
		The origin of the ray.
    
	  \param dir
		The direction of the ray.
    
	  \param rect
		The transform of the OBB to raycast against.

	  \param outPi
		The point of intersection between the ray and the Line segment.
 
	  \return
		The time of intersection as a factor of dir.
	*/
	/**************************************************************************/
	f32 RayCastRect(const AEVec2 & origin, const AEVec2 & dir, const Transform & rect, AEVec2 * outPi)
	{
		// Set time as its maximum value (infinite)
		f32 time = (f32)INFINITE;

		// Compute both half extents
		AEVec2 halfExtentX, halfExtentY;
		halfExtentX.FromAngle(rect.mOrientation);
		halfExtentY = halfExtentX.Perp() * rect.mScale.y / 2.0f;
		halfExtentX *= rect.mScale.x / 2.0f;

		// Compute the vertices of the OBB with its half extents
		AEVec2 OBBVertices[4];
		OBBVertices[0] = rect.mTranslation - halfExtentX + halfExtentY;
		OBBVertices[1] = rect.mTranslation - halfExtentX - halfExtentY;
		OBBVertices[2] = rect.mTranslation + halfExtentX - halfExtentY;
		OBBVertices[3] = rect.mTranslation + halfExtentX + halfExtentY;

		// Create a for loop to compute the Raycast onto all the lines formed by the OBB
		for (int i = 0; i < 4; i++)
		{
			// Create a Line segment formed by the OBB vertices in order
			LineSegment2D currLine(OBBVertices[i], OBBVertices[(i + 1) % 4]);
			// Get the current time by calling RayCastLine
			f32 currTime = RayCastLine(origin, dir, currLine, outPi);
			//  if the current time is positive and its lower than time, set time as currTime
			if (currTime > 0.0f && currTime < time)
				time = currTime;
		}

		// If outPi is not NULL, compute its final position
		if (outPi)
			*outPi = origin + dir * time;

		// Return the resultant time
		return time;
	}

	/**************************************************************************/
	/*!
	  \fn    
		RayCastCircle

	  \brief 
		Performs a raycast test against a circle.

	  \param origin
		The origin of the ray.
    
	  \param dir
		The direction of the ray.
    
	  \param circle
		the center of the circle.

	  \param radius
		the radius of the circle.

	  \param outPi
		The point of intersection between the ray and the circle.
 
	  \return
		The time of intersection as a factor of dir.
	*/
	/**************************************************************************/
	f32 RayCastCircle(const AEVec2 & origin, const AEVec2 & dir, const AEVec2 & circle, f32 radius, AEVec2 * outPi)
	{
		// Compute the distance between the circle and the origin of the ray
		AEVec2 dist = circle - origin;

		// Compute the a, b and c parameter for the quadratic f
		f32 a = dir * dir;
		f32 b = -2 * (dir * dist);
		f32 c = (dist * dist) - (radius * radius);

		// Define the minimum and maximum parameters for the quadratic function,
		// apart from the resultant time
		f32 min, max, time;

		// Call quadratic and check if there sis a possible value in the function.
		// If not, return -1 (error)
		if (!quadratic(a, b, c, min, max))
			return -1;

		// If the minimum is bigger than 0.0f, set it as the time
		if (min > 0.0f)
			time = min;
		// If the minimum is bigger than 0.0f, set it as the time
		else if (max > 0.0f)
			time = max;
		// If there are no positive results, return -1 (error)
		else return -1;

		// If outPi is not NULL, set it as the final position
		if (outPi)
			*outPi = origin + dir * time;

		// Return the resultant time
		return time;
	}

	/*!
	 \brief	Performs a raycast test against a circle. Simple wrapper around
			the function above
	*/
	f32 RayCastCircle(const Ray & ray, const AEVec2 & circle, f32 radius, AEVec2 * outPi)
	{
		return RayCastCircle(ray.mOrigin, ray.mDirection, circle, radius, outPi);
	}

	/*!
	 \brief	Performs a raycast test against an obb. Simple wrapper around
			the function above
	*/
	f32 RayCastRect(const Ray & ray, const Transform &rect, AEVec2 * outPi)
	{
		return RayCastRect(ray.mOrigin, ray.mDirection, rect, outPi);
	}

	/*!
	 \brief	Performs a raycast test against a line. Simple wrapper around
			the function above
	*/
	f32 RayCastLine(const Ray & ray, const LineSegment2D & line, AEVec2 * outPi)
	{
		return RayCastLine(ray.mOrigin, ray.mDirection, line, outPi);
	}
}