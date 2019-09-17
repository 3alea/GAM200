/******************************************************************************/
/*!
\file   Collisions.cpp
\author Alejandro Balea
\par    email: alejandro.balea@digipen.edu
\par    DigiPen login: alejandro.balea
\par    Course: CS230
\par    Assignment #2_2
\date   02/20/2019 - 02/25/2019
\brief
This file contains the implementation of some collision-computing functions.
*/
/******************************************************************************/
#include "Collisions.h" 

namespace AEX
{
	/****************************** HELPER FUNCTIONS ******************************/
	float MyClamp(float min, float center, float max);
	/******************************************************************************/
	
	/**************************************************************************/
	/*!
	  \fn    
	    StaticPointToStaticCircle
	
	  \brief 
	    Checks if a point is colliding with a circle.
	
	  \param P
	    A pointer to the position of the point.
	    
	  \param Center
	    A pointer to the center of the circle.
	    
	  \param Radius
	    The radius of the circle.
	 
	  \return
	    The checkup if the objects are colliding or not.
	*/
	/**************************************************************************/
	bool StaticPointToStaticCircle(AEVec2 * P, AEVec2 * Center, float Radius)
	{
		// Check if the squared distance between P and the center of the circle
		// is lower than the squared distance of the radius. 
		if (P->DistanceSq(*Center) <= Radius * Radius)
			return true; // Colliding!
		else
			return false; // Not colliding!
	}
	
	/**************************************************************************/
	/*!
	  \fn    
	    StaticPointToStaticRect
	
	  \brief 
	    Checks if a point is colliding with an AABB.
	
	  \param P
	    A pointer to the position of the point.
	    
	  \param Rect
	    A pointer to the center of the AABB.
	    
	  \param Width
	    The width of the AABB.
	
	  \param Height
		The height of the AABB.
	 
	  \return
	    The checkup if the objects are colliding or not.
	*/
	/**************************************************************************/
	bool StaticPointToStaticRect(AEVec2 * Pos, AEVec2 * Rect, float Width, float Height)
	{
		float left = Rect->x - Width / 2; // The leftside limit
		float right = Rect->x + Width / 2; // The rightside limit
		float bottom = Rect->y - Height / 2; // The bottomside limit
		float top = Rect->y + Height / 2; // The topside limit
	
		// Check if the point is between the x and y limits 
		if (left <= Pos->x 
			&& Pos->x <= right 
			&& bottom <= Pos->y 
			&& Pos->y <= top)
			return true; // Colliding!
		else
			return false; // Not colliding!
	}
	
	/**************************************************************************/
	/*!
	  \fn    
	    StaticPointToOrientedRect
	
	  \brief 
	    Checks if a point is colliding with an OBB.
	
	  \param P
	    A pointer to the position of the point.
	    
	  \param Rect
	    A pointer to the center of the OBB.
	    
	  \param Width
	    The width of the OBB.
	
	  \param Height
		The height of the OBB.
	
	  \param AngleRad
		The angle formed between the OBB and the x-axis in radians.
	 
	  \return
	    The checkup if the objects are colliding or not.
	*/
	/**************************************************************************/
	bool StaticPointToOrientedRect(AEVec2 * Pos, AEVec2 * Rect, float Width, float Height, float AngleRad)
	{
		// Compute the rotation matrix
		AEMtx33 rotMtx = rotMtx.RotRad(-AngleRad);
		// Compute the translation matrix
		AEMtx33 transMtx = transMtx.Translate(-Rect->x, -Rect->y);
		// Concatenate the matrices
		AEMtx33 MtxResult = rotMtx * transMtx;
		// Get the linear transformation of the point with the resultant matrix
		AEVec2 vecResult = MtxResult * *Pos;
	
		// Call StaticPointToStaticRect to check if the point is colliding with the OBB 
		if (StaticPointToStaticRect(&vecResult, &AEVec2(0,0), Width, Height))
			return true; // Colliding!
		else
			return false; // Not colliding!
	}
	
	/**************************************************************************/
	/*!
	  \fn    
	    StaticPointToStaticRect
	
	  \brief 
	    Checks if a circle is colliding with another circle.
	
	  \param Center0
	    A pointer to the center of the first circle.
	
	  \param Radius0
		The radius of the first circle.
	    
	  \param Center1
	     A pointer to the center of the second circle.
	    
	  \param Radius1
	    The radius of the second circle.
	 
	  \return
	    The checkup if the circles are colliding or not.
	*/
	/**************************************************************************/
	bool StaticCircleToStaticCircle(AEVec2 * Center0, float Radius0, AEVec2 * Center1, float Radius1)
	{
		// Compute the sum of the radius
		float RadiusSum = Radius0 + Radius1;
	
		// Check if the squared value of the sum of the radius is bigger than
		// the  squared distance between both circles.
		if (RadiusSum * RadiusSum >= Center0->DistanceSq(*Center1))
			return true; // Colliding!
		else
			return false; // Not colliding!
	}
	
	/**************************************************************************/
	/*!
	  \fn    
	    StaticRectToStaticRect
	
	  \brief 
	    Checks if a circle is colliding with another circle.
	
	  \param Rect0
	    A pointer to the center of the first AABB.
	
	  \param Width0
		The width of the first AABB.
	    
	  \param Height0
	     The height of the first AABB.
	    
	  \param Rect1
	    A pointer to the center of the second AABB.
	
	  \param Width1
		The width of the second AABB.
	    
	  \param Height1
	    The height of the second AABB.
	 
	  \return
	    The checkup if the circles are colliding or not.
	*/
	/**************************************************************************/
	bool StaticRectToStaticRect(AEVec2 * Rect0, float Width0, float Height0, AEVec2 * Rect1, float Width1, float Height1)
	{
		// Calculate the final height (sum of heights of AABBs divided by 2)
		float FinalHeight = (Height0 + Height1) / 2;
		// Calculate the final width (sum of widths of AABBs divided by 2)
		float FinalWidth = (Width0 + Width1) / 2;
		// The distance between the AABBs
		AEVec2 distance;
	
		// If rect0->x is bigger than rect1->x, compute the substraction between them
		if (Rect0->x >= Rect1->x)
			distance.x = Rect0->x - Rect1->x;
		// If not, invert the values to get a positive value
		else
			distance.x = Rect1->x - Rect0->x;
	
		// If rect0->y is bigger than rect1->y, compute the substraction between them
		if (Rect0->y >= Rect1->y)
			distance.y = Rect0->y - Rect1->y;
		// If not, invert the values to get a positive value
		else
			distance.y = Rect1->y - Rect0->y;
	
		// Check if both distances are less or equal than the final width/height
		if (distance.x <= FinalWidth && distance.y <= FinalHeight)
			return true; // Colliding!
		else
			return false; // Not colliding!
	}
	
	/**************************************************************************/
	/*!
	  \fn    
	    StaticRectToStaticCirlce
	
	  \brief 
	    Checks if an AABB is colliding with a circle.
	    
	  \param Rect
	    A pointer to the center of the AABB.
	    
	  \param Width
	    The width of the AABB.
	
	  \param Height
		The height of the AABB.
	
	  \param Center
	    A pointer to the center of the circle.
	    
	  \param Radius
	    The radius of the circle.
	 
	  \return
	    The checkup if the objects are colliding or not.
	*/
	/**************************************************************************/
	bool StaticRectToStaticCirlce(AEVec2 * Rect, float Width, float Height, AEVec2 * Center, float Radius)
	{
		AEVec2 result;
		// Call 'MyClamp' to get the closest value from the segment and the center of the circle
		result.x = MyClamp(Rect->x - Width / 2, Center->x, Rect->x + Width / 2);
		// Do the same but with y
		result.y = MyClamp(Rect->y - Height / 2, Center->y, Rect->y + Height / 2);
		
		// Call StaticPointToStaticCircle to check if the AABB is colliding with the circle
		if (StaticPointToStaticCircle(&result, Center, Radius))
			return true; // Colliding!
		else
			return false; // Not colliding!
	}
	
	/**************************************************************************/
	/*!
	  \fn    
	    OrientedRectToStaticCirlce
	
	  \brief 
	    Checks if a OBB is colliding with a circle.
	    
	  \param Rect
	    A pointer to the center of the OBB.
	    
	  \param Width
	    The width of the OBB.
	
	  \param Height
		The height of the OBB.
	
	  \param AngleRad
		The angle formed between the OBB and the x-axis in radians.
	
	  \param Center
	    A pointer to the center of the circle.
	    
	  \param Radius
	    The radius of the circle.
	 
	  \return
	    The checkup if the objects are colliding or not.
	*/
	/**************************************************************************/
	bool OrientedRectToStaticCirlce(AEVec2 * Rect, float Width, float Height, float AngleRad, AEVec2 * Center, float Radius)
	{
		// Compute the inverse of the rotation matrix
		AEMtx33 rotMtx = rotMtx.RotRad(-AngleRad);
		// Compute the inverse of the translation matrix
		AEMtx33 transMtx = rotMtx.Translate(-Rect->x, -Rect->y);
		// Concatenate the matrices
		AEMtx33 MtxResult = rotMtx * transMtx;
		// Compute the linear transformation of the position of the circle
		AEVec2 circle_result = MtxResult * *Center;
	
		// Call StaticRectToStaticCirlce to check if the OBB is colliding with the circle
		if (StaticRectToStaticCirlce(&AEVec2(0,0), Width, Height, &circle_result, Radius))
			return true; // Colliding!
		else
			return false; // Not colliding!
	}
	
	/**************************************************************************/
	/*!
	  \fn    
	    MyClamp
	
	  \brief 
	    Does the same function as AEClamp: checks for the closest value
		between a segment (formed by 'min' and 'max') and 'center'.
	
	  \param min
	    The minimum value of the segment.
	    
	  \param center
	    The value we want to compute in the segment.
	    
	  \param max
	    The maximum value of the segment.
	 
	  \return
	    The closest value from the segment as a float.
	*/
	/**************************************************************************/
	float MyClamp(float min, float center, float max)
	{
		// Check if the minimum value is bigger than the inputted value
		if (min > center)
			return min; // Return min
		// Check if the inputted value is bigger than the maximum value
		else if (center > max)
			return max; // Return max
		// If not...
		else
			return center; // Return the inputted value
	}
	
}
