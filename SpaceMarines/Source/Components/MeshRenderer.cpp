#include "MeshRenderer.hpp"

namespace SpaceMarines
{

MeshRenderer::MeshRenderer(const char* assetPath)
{
	if((modelResource = h3dAddResource(H3DResTypes::SceneGraph, assetPath, 0)) == 0)
		throw Exception(std::string("Unable to add mesh resource \"") + assetPath + "\"");
}

MeshRenderer::~MeshRenderer()
{
	std::cout << "Deleting MeshRenderer.\n";
}

} /* namespace SpaceMarines */
