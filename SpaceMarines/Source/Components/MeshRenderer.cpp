#include "MeshRenderer.hpp"

namespace SpaceMarines
{

MeshRenderer::MeshRenderer(const char* assetPath)
{
	if((modelResource = h3dAddResource(H3DResTypes::SceneGraph, assetPath, 0)) == 0)
		throw Exception(std::string("Unable to add mesh resource \"") + assetPath + "\"");
	modelNode = 0;
}

MeshRenderer::~MeshRenderer()
{
	std::cout << "Deleting MeshRenderer.\n";
	h3dRemoveNode(modelNode);
}

void MeshRenderer::start()
{
	if (gameObject == nullptr) throw Exception("SpotLight isn't attached to a GameObject");
	if (!h3dIsResLoaded(modelResource)) throw Exception("Model resource is not loaded!");
	modelNode = h3dAddNodes(gameObject->getTransform()->getNode(), modelResource); //TODO enable addition of parent nodes

	if (modelNode == 0) throw Exception("Model node is not added!");
}

} /* namespace SpaceMarines */
