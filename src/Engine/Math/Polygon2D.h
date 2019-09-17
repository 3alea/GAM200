#include "src/Engine/AEX.h"

/*!
*	\file		Polygon2D.h
*	\brief		Declaration of the Polygon2D structure.
*	\details	Structure to contain a convex polygon, whose vertices are stored
				in an std::vector<Vector2>. 
*	\author		Thomas Komair - tkomair@digipen.edu
*	\date		12/03/2018
*	\copyright	Copyright DigiPen Institute Of Technology. All Rights Reserved
*
*/

#ifndef CS230_POLYGON_2D_H_
#define CS230_POLYGON_2D_H_

namespace AEX
{
	struct
		Polygon2D
	{
	private:
		// Vertices of the polygon
		std::vector<AEVec2>	mVertices;
	public:

		/*!	@TODO
		*	\brief	Default constructor taking the size of the polygon.
		*	\remark	Does nothing.
		*/
		Polygon2D();

		/*!	@TODO
		*	\brief	Custom constructor taking the size of the polygon.
		*	\remark	This function should call the SetSize method below.
		*/
		Polygon2D(u32 size);

		// ------------------------------------------------------------------------
		// Vertex Management 

		/*!	@TODO
		*	\brief	Adds a vertex to the polygon.
		*	\remark	The vertex should be added at the end of the polygon. Use the
		*			push_back() method of std::vector.
		*/
		void		AddVertex(const AEVec2 & vtx);

		/*!	@TODO
		*	\brief	returns the vertex at the index specifed
		*	\remark	This function shouldn't have to do any sanity check as it returns by
		*			reference. If a erroneous index is passed. An error will be thrown.
		*/
		AEVec2 &	operator[](u32 idx);

		/*!	@TODO
		*	\brief	Removes all vertices in the polygon
		*	\remark	Use the clear method of std::vector. No need for more.
		*/
		void		Clear();


		/*!	@TODO
		*	\brief	Returns the size of the mVertices array
		*	\remark	Use the size method of std::vector. No need for more.
		*/
		u32			GetSize() const;

		/*! @TODO
		*	\brief	Sets the size of the mVertices array
		*	\remark	Use the resize method of std::vector. No need for more.
		*/
		void		SetSize(u32 size);

		/*! @TODO
		*	\brief	Returns a copy of the vertices transformed by the provided matrix
		*/
		std::vector<AEVec2> GetTransformedVertices(const AEMtx33 &mat_transform) const;

		/*! @PROVIDED
		*	\brief	Draws the polygon with the provided color and transform.
		*/
		//void Draw(u32 color, AEMtx33 * vtx_transform = NULL) const;

		// ------------------------------------------------------------------------
		// STATIC INTERFACE
		// ------------------------------------------------------------------------

		/*! @PROVIDED
		*	\brief	Creates a quad polygon.
		*/
		static Polygon2D MakeQuad();

		/*! @PROVIDED
		*	\brief	Creates a 5-sided polygon.
		*/
		static Polygon2D MakePentagon();

		/*! @PROVIDED
		*	\brief	Creates a 6-sided polygon.
		*/
		static Polygon2D MakeHexagon();

		/*! @PROVIDED
		*	\brief	Creates a 7-sided polygon.
		*/
		static Polygon2D MakeSeptagon();

		/*! @PROVIDED
		*	\brief	Creates a 8-sided polygon.
		*/
		static Polygon2D MakeOctagon();

		/*! @PROVIDED
		*	\brief	Creates a n-sided polygon.
		*/
		static Polygon2D MakeStandardPoly(u32 side);
	};
}
#endif // ! CS230_POLYGON_2D_H_
