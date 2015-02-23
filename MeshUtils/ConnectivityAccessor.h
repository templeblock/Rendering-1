/*
 This file is part of the Rendering library.
 Copyright (C) 2015 Sascha Brandt <myeti@mail.upb.de>

 This library is subject to the terms of the Mozilla Public License, v. 2.0.
 You should have received a copy of the MPL along with this library; see the
 file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef RENDERING_CONNECTIVITYACCESSOR_H_
#define RENDERING_CONNECTIVITYACCESSOR_H_

#include "TriangleAccessor.h"

#include <Util/References.h>
#include <Util/ReferenceCounter.h>

#include <tuple>
#include <vector>

namespace Geometry {
template<typename _T> class _Vec3;
typedef _Vec3<float> Vec3;
template<typename T_> class Triangle;
typedef Triangle<Vec3> Triangle3;
}

namespace Rendering {
class Mesh;
class MeshVertexData;
class MeshIndexData;
class PositionAttributeAccessor;
namespace MeshUtils {

/**
 * Allows to get connectivity informations of vertices and triangles of a mesh
 *
 * @author Sascha Brandt
 * @verbatim
 *      v2
 *       X
 *      / \
 *     / c1\      /
 *    /-----\____/
 *   /  t0  /\c2/
 *  /      /c0\/
 * X------/----X
 * v0          v1
 *
 * t0 - triangle (see getTriangle)
 * v0,v1,v2 - vertices (see getVertex)
 * c0,c1,c2 - corners (see getCorner)
 * c0 - corner of v1 and t0 (see getCorner, getVertexCorner, getCornerVertex, getTriangleCorner, getCornerTriangle)
 * c1 - corner of t0 and next triangle corner of c0 (see getNextTriangleCorner)
 * c2 - corner of v1 and next vertex corner of c0 (see getNextVertexCorner)
 * @endverbatim
 */
class ConnectivityAccessor : public Util::ReferenceCounter<ConnectivityAccessor> {
private:
	MeshIndexData& indices;
	Util::Reference<PositionAttributeAccessor> posAcc;
	Util::Reference<TriangleAccessor> triAcc;
	std::vector<uint32_t> vertexCorners;
	std::vector<uint32_t> triangleNextCorners;
protected:
	void assertCornerRange(uint32_t cIndex) const;
	void assertVertexRange(uint32_t vIndex) const;
	void assertTriangleRange(uint32_t tIndex) const;
	ConnectivityAccessor(Mesh* mesh);
public:
	/*! (static factory)
		Create a ConnectivityAccessor for the given Mesh.
		If no Accessor can be created, an std::invalid_argument exception is thrown. */
	static Util::Reference<ConnectivityAccessor> create(Mesh* mesh);

	virtual ~ConnectivityAccessor() {}

	/**
	 * Return the coordinates of a vertex.
	 * @param vIndex the vertex index
	 * @return the vertex position
	 */
	Geometry::Vec3 getVertex(uint32_t vIndex) const;

	/**
	 * Return the three vertex indices of a triangle.
	 * @param tIndex the triangle index
	 * @return a tuple of 3 vertex indices
	 */
	TriangleAccessor::TriangleIndices_t getTriangle(uint32_t tIndex) const;

	/**
	 * Return the corner joining a vertex with a triangle
	 * @param vIndex the vertex index
	 * @param tIndex the triangle index
	 * @return the corner index joining the vertex with the triangle
	 */
	uint32_t getCorner(uint32_t vIndex, uint32_t tIndex) const;

	/**
	 * Return one of the corners incident to a vertex
	 * @param vIndex the vertex index
	 * @return the corner index incident to the vertex
	 */
	uint32_t getVertexCorner(uint32_t vIndex) const;

	/**
	 * Return one of the corners of a triangle.
	 * @param tIndex the triangle index
	 * @return the corner index incident to the triangle
	 */
	uint32_t getTriangleCorner(uint32_t tIndex) const;

	/**
	 * Return the vertex associated with a corner.
	 * @param cIndex the corner index
	 * @return the vertex index associated with the corner.
	 */
	uint32_t getCornerVertex(uint32_t cIndex) const;

	/**
	 * Return the triangle associated with a corner.
	 * @param cIndex the corner index
	 * @return the triangle index associated with the corner.
	 */
	uint32_t getCornerTriangle(uint32_t cIndex) const;

	/**
	 * Return the next corner incident on the vertex associated with a corner.
	 * @param cIndex the corner index
	 * @return the next corner index incident on the vertex associated with the corner
	 */
	uint32_t getNextVertexCorner(uint32_t cIndex) const;

	/**
	 * Return the next corner within the triangle associated with a corner.
	 * @param cIndex the corner index
	 * @return the next corner within the triangle associated with the corner.
	 */
	uint32_t getNextTriangleCorner(uint32_t cIndex) const;
};

} /* namespace MeshUtils */
} /* namespace Rendering */

#endif /* RENDERING_CONNECTIVITYACCESSOR_H_ */
