/*
	This file is part of the Rendering library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef MESHBUILDER_H_
#define MESHBUILDER_H_

#include "../Mesh/VertexAttribute.h"
#include "../Mesh/VertexDescription.h"
#include <Util/ReferenceCounter.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <memory>

namespace Geometry {
template<typename value_t> class _Box;
typedef _Box<float> Box;
template<typename _T> class _Matrix4x4;
typedef _Matrix4x4<float> Matrix4x4;
template<typename T_> class _SRT;
typedef _SRT<float> SRT;
template<typename _T> class _Vec2;
typedef _Vec2<float> Vec2;
template<typename _T> class _Vec3;
typedef _Vec3<float> Vec3;
typedef _Vec3<float> Vec3f;
typedef _Vec3<char> Vec3b;
}

namespace Util {
class Color4f;
class Color4ub;
class Bitmap;
}

namespace Rendering {
class Mesh;

namespace MeshUtils {

class MeshBuilder : public Util::ReferenceCounter<MeshBuilder> {
	/*! @name Static mesh-creation helper */
	// @{
	public:
		/**
		 * Return the mesh of a three-dimensional, axis-aligned box.
		 * Positions, and normals are supported.
		 *
		 * @param vertexDesc Vertex description specifying the vertex information to generate
		 * @param box Geometric specification of the box
		 * @return Mesh of the box
		 */
		static Mesh * createBox(const VertexDescription & vertexDesc, const Geometry::Box & box);
		
		//! Adds a box to the given meshBuilder. \see createBox(...)
		static void addBox(MeshBuilder & mb, const Geometry::Box & box);

		/*! Creates a mesh for a sky dome with texture coordinates.
			\note adapted from [alc] http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=13887&sid=25788d2b93c73294fd7abe76db6fab1e */
		static Mesh * createDome(const double radius = 100.0,
						   const int horiRes = 40,
						   const int vertRes = 40,
						   const double halfSphereFraction = 1.0,
						   const double imagePercentage = 1.0);

		/**
		 * Return a sphere, which is generated by subdividing the sphere surface into segements.
		 * The surface is subdivided into the given number of subdivisions of the inclination angle and the azimuth angle, respectively.
		 * The sphere is centered at the origin and the vertices are lying on the unit sphere.
		 * The given vertex description specifies which information is generated for every vertex.
		 * Positions, normals, colors, and texture coordinates are supported.
		 *
		 * @param vertexDesc Vertex description specifying the vertex information to generate
		 * @param inclinationSegments Number of subdivisions of the inclination angle
		 * @param azimuthSegments Number of subdivisions of the azimuth angle
		 * @return Sphere Mesh
		 */
		static Mesh * createSphere(const VertexDescription & vertexDesc, uint32_t inclinationSegments, uint32_t azimuthSegments);
		
		//! Adds a shere to the given meshBuilder. \see createSphere(...)
		static void addSphere(MeshBuilder & mb, uint32_t inclinationSegments, uint32_t azimuthSegments);

		/**
		 * Return a sector of a disc.
		 * The center of the disc is the origin.
		 * The disc is aligned with the y-z plane and the normals are pointing into negative x direction.
		 * Positions and normals for the vertices are generated.
		 *
		 * @param radius Radius of the disc
		 * @param numSegments Number of segments for the disc
		 * @param angle Central angle of the sector in degrees
		 * @return Disc Mesh or @c nullptr if the number of segments is smaller than two.
		 */
		static Mesh * createDiscSector(float radius, uint8_t numSegments, float angle = 360.0f);

		 /**
		 * Return a sector of a ring.
		 * The center of the disc is the origin.
		 * The disc is aligned with the y-z plane and the normals are pointing into negative x direction.
		 * Positions and normals for the vertices are generated.
		 *
		 * @param innerRadius inner radius of the ring
		 * @param outerRadius outer radius of the ring
		 * @param numSegments Number of segments for the disc
		 * @param angle Central angle of the sector in degrees
		 * @return Disc Mesh or @c nullptr if the number of segments is smaller than two or innerRadius >= outerRadius
		 */
		static Mesh * createRingSector(float innerRadius, float outerRadius, uint8_t numSegments, float angle = 360.0f);

		/**
		 * Return a cone with the radius given.
		 * The cap end is not created.
		 * The center of the base is the origin.
		 * The apex lies on the x-axis at distance @a height from the origin.
		 * Positions and normals for the vertices are generated.
		 *
		 * @param radius Radius of the base
		 * @param height Distance between the base and the apex
		 * @param numSegments Number of segments for the circle at the base and the lateral surface
		 * @return Cone Mesh or @c nullptr if the number of segments is smaller than two
		 * @see createDisc() for creation of a cap end
		 */
		static Mesh * createCone(float radius, float height, uint8_t numSegments);

		/**
		 * Return a conical frustum with the radii given.
		 * The cap ends are not created.
		 * The center of the bottom circle is the origin.
		 * The center of the top circle lies on the x-axis at distance @a height from the origin.
		 * If both radii have the same value, a cylinder is created.
		 * Positions and normals for the vertices are generated.
		 *
		 * @param radiusBottom Radius of the bottom circle
		 * @param radiusTop Radius of the top circle
		 * @param height Distance between both circles
		 * @param numSegments Number of segments for the circles and the lateral surface
		 * @return Conical frustum Mesh or @c nullptr if the number of segments is smaller than two
		 * @see createDisc() for creation of cap ends
		 */
		static Mesh * createConicalFrustum(float radiusBottom, float radiusTop, float height, uint8_t numSegments);

		/**
		 * Returns an arrow which consists of a ConicalFrustum and 2 Cones at one end.
		 *
		 * @param radius Radius of the arrow
		 * @param length Overall length of the arrow
		 * @return Arrow Mesh
		 */
		static Mesh * createArrow(float radius, float length);

		/**
		 * Returns a Rectangle consisting of 2 triangles with origin in the center (oriented in x-y-plane)
		 *
		 * @param width
		 * @param height
		 * @return Rectangle Mesh
		 */
		static Mesh * createRectangle(const VertexDescription & desc,float width, float height);

//		// transMat = inverseModelViewProjection
		static Mesh * createMeshFromBitmaps(const VertexDescription & d, Util::Bitmap * depth,
											Util::Bitmap * color=nullptr, Util::Bitmap * normals=nullptr );

	// @}
	// -----
private:
	/**
	 * Helper-struct for storing vertex information
	 */
	struct MBVertex {
		std::unique_ptr<uint8_t[]> data;
		explicit MBVertex(size_t size) : data(new uint8_t[size]) {
		}
		MBVertex(const MBVertex & vertex, size_t size) : data(new uint8_t[size]) {
			std::copy(vertex.data.get(), vertex.data.get() + size, data.get());
		}
		float * floatPtr(const VertexAttribute & attr) {
			return reinterpret_cast<float *>(data.get()+attr.getOffset());
		}
		uint8_t * uint8Ptr(const VertexAttribute & attr) {
			return reinterpret_cast<uint8_t *>(data.get()+attr.getOffset());
		}
		int8_t * int8Ptr(const VertexAttribute & attr) {
			return reinterpret_cast<int8_t *>(data.get()+attr.getOffset());
		}
		void setPosition(const VertexAttribute & attr,const Geometry::Vec2 & pos);
		void setPosition(const VertexAttribute & attr,const Geometry::Vec3 & pos);
		void setNormal(const VertexAttribute & attr, const Geometry::Vec3f & n);
		void setNormal(const VertexAttribute & attr, const Geometry::Vec3b & n);
		void setColor(const VertexAttribute & attr,const Util::Color4f & color);
		void setColor(const VertexAttribute & attr,const Util::Color4ub & color);
		void setTex0(const VertexAttribute & attr,const Geometry::Vec2 & uv);
	};
public:
	MeshBuilder();
	explicit MeshBuilder(const VertexDescription & description);
	~MeshBuilder();

	/*!	true if no no vertices were added so far.	*/
	bool isEmpty()const					{	return verts.empty();	}

	//!	Build a new mesh using the internal vertex and index buffer.
	Mesh * buildMesh();

	/*! Sets the current vertex data for the following vertices (like a state in OpenGL). 
		If a tranformation is set, the position and normal are transformed accordingly before being set. */
	void position(const Geometry::Vec2 & v);
	void position(const Geometry::Vec3 & v);
	void normal(const Geometry::Vec3f & n);
	void normal(const Geometry::Vec3b & n);
	void color(const Util::Color4f & c)						{	currentVertex.setColor(colorAttr,c);	}
	void color(const Util::Color4ub & c)					{	currentVertex.setColor(colorAttr,c);	}
	void texCoord0(const Geometry::Vec2 & uv)				{	currentVertex.setTex0(tex0Attr,uv);	}

	/*! Add a vertex with the current data (set by position(...),normal(...) etc.).
		The index of the new vertex is returned.*/
	uint32_t addVertex();

	/*!	Add a vertex to the internal buffer. The index of the new vertex is returned.  deprecated!!!!!	*/
	uint32_t addVertex(const Geometry::Vec3& pos, const Geometry::Vec3& n,
						float r, float g, float b, float a,
						float u, float v);

	/*!	Add a index to the interal buffer	*/
	void addIndex(uint32_t idx);

	/*!	Adds a quad to the internal buffer, clockwise.	*/
	void addQuad(uint32_t idx0, uint32_t idx1, uint32_t idx2, uint32_t idx3);

	/*!	Adds a three indices	*/
	void addTriangle(uint32_t idx0, uint32_t idx1, uint32_t idx2);

	/*!	Get current vertex count which is the index of next vertex added. */
	uint32_t getNextIndex()const 							{	return verts.size();	}

	Geometry::Matrix4x4 getTransformation() const;

	//! The transformation is applied to following 'position' and 'normal' calls.
	void setTransformation(const Geometry::Matrix4x4 & m);
	void setTransformation(const Geometry::SRT & s);

private:
	VertexDescription description;
	std::deque<MBVertex> verts; //!< vertex buffer
	std::deque<uint32_t> inds; //!< index buffer
	const VertexAttribute & posAttr;
	const VertexAttribute & normalAttr;
	const VertexAttribute & colorAttr;
	const VertexAttribute & tex0Attr;

	MBVertex currentVertex;
	std::unique_ptr<Geometry::Matrix4x4> transMat;
};

}
}

#endif /* MESHBUILDER_H_ */
