#pragma once
#include "RigidBody.hpp"
#include "MeshRenderer.hpp"

namespace SpaceMarines
{

/*************************************
 * 	Base Collider class
 *************************************/
class Collider : public Component
{
public:
	Collider(const Vector3 &offset) { collisionShape = nullptr; this->offset = offset; }
	~Collider() { if (collisionShape != nullptr) delete collisionShape; }
	void registerStart()
	{
		collisionShape = createCollisionShape();
		collisionShape->setUserPointer(this);
	}
	const char* getComponentType() const { return "Collider"; }
protected:
	btCollisionShape* collisionShape;
	virtual btCollisionShape* createCollisionShape() = 0;
	friend class RigidBody;
	Vector3 offset;
};

/*************************************
 * 	Collider classes
 *************************************/
class BoxCollider : public Collider
{
public:
	BoxCollider(const Vector3 &extents = Vector3::ONE, const Vector3& offset = Vector3::ZERO) : Collider(offset) { this->extents = extents;}
protected:
	btCollisionShape* createCollisionShape()
	{
		return new btBoxShape(extents.bullet() * 0.5f);
	}
private:
	Vector3 extents;
};

class GroundPlaneCollider : public Collider
{
public:
	GroundPlaneCollider(const Vector3& offset = Vector3::ZERO) : Collider(offset) {}
protected:
	btCollisionShape* createCollisionShape()
	{
		return new btStaticPlaneShape(Vector3::UP.bullet(), offset.z);
	}
};

class CapsuleCollider : public Collider
{
public:
	CapsuleCollider(const float radius, const float height, const Vector3 &offset = Vector3::ZERO) : Collider(offset)
	{
		params = Vector2(radius, height * 0.5f);
	}
protected:
	btCollisionShape* createCollisionShape()
	{
		return new btCapsuleShape(params.x, params.y);
	}
private:
	Vector2 params;
};

class TriangleMeshColliderStatic : public Collider
{
public:
	TriangleMeshColliderStatic(MeshRenderer* mesh, const Vector3 &offset = Vector3::ZERO) : Collider(offset)
	{
		this->mesh = mesh;
		m_btTriangleMesh = nullptr;
	}
protected:
	btCollisionShape* createCollisionShape()
	{
		if (mesh->getModelNode() == 0) mesh->registerStart();

		m_btTriangleMesh = new btTriangleMesh();
		H3DNode meshNode = h3dGetNodeChild(mesh->getModelNode(), 0); //TODO handle multi-mesh stuff
		if (meshNode == 0) throw Exception("Couldn't get mesh node.\n");
		H3DRes geoRes = h3dGetNodeParamI(mesh->getModelNode(), H3DModel::GeoResI);
		unsigned int numVertices = h3dGetNodeParamI(meshNode, H3DMesh::VertREndI) - h3dGetNodeParamI(meshNode, H3DMesh::VertRStartI) + 1;//get mesh vertices count
		unsigned int numTriangleIndices = h3dGetNodeParamI(meshNode, H3DMesh::BatchCountI);//get mesh triangle indices count
		unsigned int vertRStart = h3dGetNodeParamI(meshNode, H3DMesh::VertRStartI);//get the first vertex of the mesh
		int off = 3;


		std::cout << numVertices << " vertices.\n";
		std::cout << numTriangleIndices << " indices.\n";

		int vertexOffset = vertRStart * 3;
		int indexOffset = h3dGetNodeParamI(meshNode, H3DMesh::BatchStartI);

		float* vertexBase = (float*) h3dMapResStream(geoRes, H3DGeoRes::GeometryElem, 0, H3DGeoRes::GeoVertPosStream, true, false);//gets pointer to the vertex data
		h3dUnmapResStream(geoRes);//closes vertex stream

		if(vertexBase) vertexBase += vertexOffset;

		unsigned int* TriangleBase32 = nullptr;
		unsigned short* TriangleBase16 = nullptr;

		if (h3dGetResParamI(geoRes, H3DGeoRes::GeometryElem, 0, H3DGeoRes::GeoIndices16I))
		{
			unsigned short* tb = (unsigned short*)h3dMapResStream(geoRes, H3DGeoRes::GeometryElem, 0, H3DGeoRes::GeoIndexStream, true, false);
			TriangleBase16 = new unsigned short[ numTriangleIndices ];
			memcpy(TriangleBase16, tb+indexOffset, sizeof(unsigned short)*numTriangleIndices);
			h3dUnmapResStream(geoRes);
			delete TriangleBase32;
		}
		else
		{
			unsigned int* tb = (unsigned int*)h3dMapResStream(geoRes, H3DGeoRes::GeometryElem, 0, H3DGeoRes::GeoIndexStream, true, false);
			TriangleBase32 = new unsigned int[numTriangleIndices];
			memcpy(TriangleBase32, tb+indexOffset, sizeof(unsigned int)*numTriangleIndices);
			h3dUnmapResStream(geoRes);
			delete TriangleBase16;
		}

		unsigned int indices[numTriangleIndices];

		bool index16 = false;
		if (TriangleBase16)
			index16 = true;

	    for (unsigned int i = 0; i < numTriangleIndices - 2; i += 3)
		 {
			unsigned int index1 = index16 ? (TriangleBase16[i]   - vertRStart) * 3 : (TriangleBase32[i]   - vertRStart) * 3;
			unsigned int index2 = index16 ? (TriangleBase16[i+1] - vertRStart) * 3 : (TriangleBase32[i+1] - vertRStart) * 3;
			unsigned int index3 = index16 ? (TriangleBase16[i+2] - vertRStart) * 3 : (TriangleBase32[i+2] - vertRStart) * 3;

			m_btTriangleMesh->addTriangle(
			   btVector3(vertexBase[index1], vertexBase[index1+1], vertexBase[index1+2] ),
			   btVector3(vertexBase[index2], vertexBase[index2+1], vertexBase[index2+2] ),
			   btVector3(vertexBase[index3], vertexBase[index3+1], vertexBase[index3+2]),
			   false
			);
		 }

	    return new btBvhTriangleMeshShape(m_btTriangleMesh, true, true);
	}
private:
	MeshRenderer* mesh;
	btTriangleMesh* m_btTriangleMesh;
};

class TriangleMeshCollider : public Collider
{
public:
	TriangleMeshCollider(MeshRenderer* mesh, const Vector3 &offset = Vector3::ZERO) : Collider(offset)
	{
		this->mesh = mesh;
		m_btTriangleMesh = nullptr;
	}
protected:
	btCollisionShape* createCollisionShape()
	{
		if (mesh->getModelNode() == 0) mesh->registerStart();

		m_btTriangleMesh = new btTriangleMesh();
		H3DNode meshNode = h3dGetNodeChild(mesh->getModelNode(), 0); //TODO handle multi-mesh stuff
		if (meshNode == 0) throw Exception("Couldn't get mesh node.\n");
		H3DRes geoRes = h3dGetNodeParamI(mesh->getModelNode(), H3DModel::GeoResI);
		unsigned int numVertices = h3dGetNodeParamI(meshNode, H3DMesh::VertREndI) - h3dGetNodeParamI(meshNode, H3DMesh::VertRStartI) + 1;//get mesh vertices count
		unsigned int numTriangleIndices = h3dGetNodeParamI(meshNode, H3DMesh::BatchCountI);//get mesh triangle indices count
		unsigned int vertRStart = h3dGetNodeParamI(meshNode, H3DMesh::VertRStartI);//get the first vertex of the mesh
		int off = 3;


		std::cout << numVertices << " vertices.\n";
		std::cout << numTriangleIndices << " indices.\n";

		int vertexOffset = vertRStart * 3;
		int indexOffset = h3dGetNodeParamI(meshNode, H3DMesh::BatchStartI);

		float* vertexBase = (float*) h3dMapResStream(geoRes, H3DGeoRes::GeometryElem, 0, H3DGeoRes::GeoVertPosStream, true, false);//gets pointer to the vertex data
		h3dUnmapResStream(geoRes);//closes vertex stream

		if(vertexBase) vertexBase += vertexOffset;

		unsigned int* TriangleBase32 = nullptr;
		unsigned short* TriangleBase16 = nullptr;

		if (h3dGetResParamI(geoRes, H3DGeoRes::GeometryElem, 0, H3DGeoRes::GeoIndices16I))
		{
			unsigned short* tb = (unsigned short*)h3dMapResStream(geoRes, H3DGeoRes::GeometryElem, 0, H3DGeoRes::GeoIndexStream, true, false);
			TriangleBase16 = new unsigned short[ numTriangleIndices ];
			memcpy(TriangleBase16, tb+indexOffset, sizeof(unsigned short)*numTriangleIndices);
			h3dUnmapResStream(geoRes);
			delete TriangleBase32;
		}
		else
		{
			unsigned int* tb = (unsigned int*)h3dMapResStream(geoRes, H3DGeoRes::GeometryElem, 0, H3DGeoRes::GeoIndexStream, true, false);
			TriangleBase32 = new unsigned int[numTriangleIndices];
			memcpy(TriangleBase32, tb+indexOffset, sizeof(unsigned int)*numTriangleIndices);
			h3dUnmapResStream(geoRes);
			delete TriangleBase16;
		}

		unsigned int indices[numTriangleIndices];

		bool index16 = false;
		if (TriangleBase16)
			index16 = true;

	    for (unsigned int i = 0; i < numTriangleIndices - 2; i += 3)
		 {
			unsigned int index1 = index16 ? (TriangleBase16[i]   - vertRStart) * 3 : (TriangleBase32[i]   - vertRStart) * 3;
			unsigned int index2 = index16 ? (TriangleBase16[i+1] - vertRStart) * 3 : (TriangleBase32[i+1] - vertRStart) * 3;
			unsigned int index3 = index16 ? (TriangleBase16[i+2] - vertRStart) * 3 : (TriangleBase32[i+2] - vertRStart) * 3;

			m_btTriangleMesh->addTriangle(
			   btVector3(vertexBase[index1], vertexBase[index1+1], vertexBase[index1+2] ),
			   btVector3(vertexBase[index2], vertexBase[index2+1], vertexBase[index2+2] ),
			   btVector3(vertexBase[index3], vertexBase[index3+1], vertexBase[index3+2]),
			   false
			);
		 }

	    return new btConvexTriangleMeshShape(m_btTriangleMesh);
	}
private:
	MeshRenderer* mesh;
	btTriangleMesh* m_btTriangleMesh;
};


}
