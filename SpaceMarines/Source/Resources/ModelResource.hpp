#pragma once

#include "Resource.hpp"

namespace SpaceMarines
{

class ModelResource : public Resource
{
public:
	ModelResource(const std::string &filename);
	virtual ~ModelResource();
protected:

	struct Vertex
	{
		Vector3 position;
		Vector3 normal;
		Vector3 vertexColor;
		Vector2 uv0;
	};

	class MeshSource : public Resource
	{
	public:
		MeshSource() {} //Default constructor
		MeshSource(const std::string &filename);
		virtual ~MeshSource();
	private:
		//Note: We support only up 65k-vert/poly meshes
		unsigned short numVertices;
		unsigned short numFaces;
		unsigned short numIndices;

		Vertex* vertices;
		unsigned short* indices;
	};

	MeshSource meshSources[3];

};

} /* namespace SpaceMarines */
