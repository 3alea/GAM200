// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:		Polygon2D.cpp
//	Purpose:		This file stores the functions used for 2D polygon construction.
//	Project:		CS230_alejandro.balea_3
//	Author:			Alejandro Balea Moreno, alejandro.balea (540002118)
// ----------------------------------------------------------------------------
#include "Polygon2D.h"

namespace AEX
{
	/**************************************************************************/
	/*!
	  \fn    
		Polygon2D (Default constructor)

	  \brief 
		Default constructor taking the size of the polygon.
	*/
	/**************************************************************************/
	Polygon2D::Polygon2D()
	{
		// Does nothing
	}

	/**************************************************************************/
	/*!
	  \fn    
		Polygon2D (Conversion constructor)

	  \brief 
		Custom constructor taking the size of the polygon.

	  \param size
		The number of vertices of the polygon.
	*/
	/**************************************************************************/
	Polygon2D::Polygon2D(u32 size)
	{
		// Create a for loop depending on size
		for (u32 i = 0; i < size; i++)
		{
			// Create a normalized vector which stores the position of the vertex depending on i and its size
			AEVec2 vertex;
			vertex.FromAngle(2 * PI * i / size);

			// Call AddVertex to implement the vertex
			AddVertex(vertex);
		}
	}

	/**************************************************************************/
	/*!
	  \fn    
		AddVertex

	  \brief 
		Adds a vertex to the polygon.

	  \param vtx
		The vertex we want to add to the list.
	*/
	/**************************************************************************/
	void Polygon2D::AddVertex(const AEVec2 & vtx)
	{
		// Push the vertex to mVertices
		mVertices.push_back(vtx);
	}

	/**************************************************************************/
	/*!
	  \fn    
		operator[]

	  \brief 
		Overloads the subscript operator to return the vertex at the index specifed.

	  \param idx
		The index of the array.
 
	  \return
		The vector we want to access.
	*/
	/**************************************************************************/
	AEVec2 & Polygon2D::operator[](u32 idx)
	{
		// Check id the size of mVertices is lower than index. If that's so, abort
		if (idx > mVertices.size())
			std::abort();

		// Return the indexed vertex
		return mVertices[idx];
	}

	/**************************************************************************/
	/*!
	  \fn    
		Clear

	  \brief 
		Removes all vertices in the polygon.
	*/
	/**************************************************************************/
	void Polygon2D::Clear()
	{
		// Call clear to remove all the vertices from the polygon
		mVertices.clear();
	}

	/**************************************************************************/
	/*!
	  \fn    
		GetSize

	  \brief 
		Gettor for the size of mVertices.
	*/
	/**************************************************************************/
	u32 Polygon2D::GetSize() const
	{
		// Just return the size of mVertices
		return mVertices.size();
	}

	/**************************************************************************/
	/*!
	  \fn    
		SetSize

	  \brief 
		Settor for the size of mVertices.

	  \param size
		The new size of the array.
	*/
	/**************************************************************************/
	void Polygon2D::SetSize(u32 size)
	{
		// Call resize to change the size of mVertices
		mVertices.resize(size);
	}

	/**************************************************************************/
	/*!
	  \fn    
		GetTransformedVertices

	  \brief 
		Returns a copy of the vertices transformed by the provided matrix.

	  \param mat_transform
		The matrix used to transform the vertices.
 
	  \return
		An array/vector of vertices transformed by the inputted matrix.
	*/
	/**************************************************************************/
	std::vector<AEVec2> Polygon2D::GetTransformedVertices(const AEMtx33 & mat_transform) const
	{
		// Initialize the vertex array
		std::vector<AEVec2> vertexArray;

		// For all the size of mVertices, push back the transformated vertices
		for (u32 i = 0; i < mVertices.size(); i++)
			vertexArray.push_back(mat_transform * mVertices[i]);

		// return the computed vertex array
		return vertexArray;
	}

	/*! @PROVIDED
	*	\brief	Draws the polygon with the provided color and transform.
	*/
	/*void Polygon2D::Draw(u32 color, AEMtx33 * vtx_transform) const
	{
		// can't draw polygon if we don't have at least
		// 2 vertices
		if (mVertices.size() <= 1)
			return;

		// set transform if necessary transform 
		if (vtx_transform != NULL) {
			AEMtx33 mat = *vtx_transform;
			AEGfxSetTransform(&mat);
		}

		// Draw line between each vertices
		for (u32 i = 0; i < mVertices.size() - 1; ++i)
		{
			auto & v1 = mVertices[i];
			auto & v2 = mVertices[i + 1];
			AEGfxLine(v1.x, v1.y, 0, color, v2.x, v2.y, 0, color);
		}
		// Draw last line from last to first vertex
		auto & v1 = mVertices[0];
		auto & v2 = mVertices[mVertices.size() - 1];
		AEGfxLine(v1.x, v1.y, 0, color, v2.x, v2.y, 0, color);

		// force draw on graphics system
		AEGfxFlush();
	}*/


	// ------------------------------------------------------------------------
	// STATIC INTERFACE
	// ------------------------------------------------------------------------
	/*! @PROVIDED
	*	\brief	Creates a quad polygon.
	*/
	Polygon2D Polygon2D::MakeQuad()
	{
		// result polygon
		Polygon2D res(4);

		// create vertices
		res[0] = { -0.5f, 0.5f };
		res[1] = { -0.5f, -0.5f };
		res[2] = { 0.5f, -0.5f };
		res[3] = { 0.5f, 0.5f };

		return res;
	}

	/*! @PROVIDED
	*	\brief	Creates a 5-sided polygon.
	*/
	Polygon2D Polygon2D::MakePentagon()
	{
		return MakeStandardPoly(5);
	}

	/*! @PROVIDED
	*	\brief	Creates a 6-sided polygon.
	*/
	Polygon2D Polygon2D::MakeHexagon()
	{
		return MakeStandardPoly(6);
	}

	/*! @PROVIDED
	*	\brief	Creates a 7-sided polygon.
	*/
	Polygon2D Polygon2D::MakeSeptagon()
	{
		return MakeStandardPoly(7);
	}

	/*! @PROVIDED
	*	\brief	Creates a 8-sided polygon.
	*/
	Polygon2D Polygon2D::MakeOctagon()
	{
		return MakeStandardPoly(8);
	}

	/*! @PROVIDED
	*	\brief	Creates a n-sided polygon.
	*/
	Polygon2D Polygon2D::MakeStandardPoly(u32 side)
	{
		Polygon2D res(side);
		res[0] = { 0.5f, 0.0f };
		f32 alpha = (2.0f*PI) / (f32)side;
		for (u32 i = 1; i < side; ++i) {

			// set current point to previous
			// before rotation
			res[i] = res[i - 1];
		
			// compute rotation vars
			f32 tmp = res[i].x;
			f32 cA = cosf(alpha);
			f32 sA = sinf(alpha);

			// apply rotation to get new point
			res[i].x = cA * res[i].x - sA * res[i].y;
			res[i].y = sA * tmp + cA * res[i].y;
		}
		return res;
	}
}