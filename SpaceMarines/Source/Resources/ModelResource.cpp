/*
 * ModelResource.cpp
 *
 *  Created on: Nov 10, 2012
 *      Author: nick
 */

#include "ModelResource.hpp"

using namespace std;
namespace SpaceMarines
{

ModelResource::ModelResource(const std::string &filename) : Resource(filename)
{
	cout << "Loading model file \"" << filename << "\"...\n";
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	if (doc->LoadFile(filename.c_str()) != tinyxml2::XML_NO_ERROR) throw Exception("Unable to load file \"" + filename + "\"");

	tinyxml2::XMLElement* root = doc->RootElement();
	if (root == nullptr) throw Exception("File \"" + filename + "\" is not a valid smMesh file.");

	tinyxml2::XMLElement* sources = root->FirstChildElement("sources");
	if (sources == nullptr) throw Exception("Unable to find source data in mesh file \"" + filename + "\"");

	//Load the .geo sources
	cout << "Loading sources...\n";
	for (tinyxml2::XMLElement* cursor = sources->FirstChildElement("geo"); cursor != nullptr; cursor = cursor->NextSiblingElement("geo"))
	{
		unsigned short lod = 0;
		string source = cursor->Attribute("source");
		if (cursor->Attribute("lod") != nullptr)
			lod = cursor->IntAttribute("lod");
		if (lod > 2) throw Exception("LOD cannot exceed 2");
		cout << "\tGeometry source[" << lod << "]: \"" << source << "\"\n";
		meshSources[lod] = MeshSource(source);
	}

	delete doc;
}

ModelResource::~ModelResource()
{
	delete[] meshSources;
}

/************************
 * MeshSource data
 ************************/
ModelResource::MeshSource::MeshSource(const string &filename) : Resource(filename)
{
	string tabs = "\t\t";
	cout << tabs << "Loading mesh source \"" << this->filename << "\"...\n";
	Math::BinaryFile file(filename);

	file.read(&numVertices);
	cout << tabs << numVertices << " vertices.\n";
	file.read(&numFaces);
	numIndices = numFaces * 3;
	cout << tabs << numFaces << " faces (" << numIndices << " indices).\n";

	vertices = new Vertex[numVertices];
	indices = new unsigned short[numIndices];

	cout << "\tDone loading mesh source.\n";
}
ModelResource::MeshSource::~MeshSource()
{
	delete[] vertices;
	delete[] indices;
}


} /* namespace SpaceMarines */
