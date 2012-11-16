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
}

void MeshRenderer::update()
{
	Transform* trans = gameObject->getTransform();
	h3dSetNodeTransformQ(modelNode, trans->position.x, trans->position.y, trans->position.z,
			trans->rotation.x, trans->rotation.y, trans->rotation.z, trans->rotation.w,
			trans->scale.x, trans->scale.y, trans->scale.z);
}

void MeshRenderer::start()
{
	if (!h3dIsResLoaded(modelResource)) throw Exception("Model resource is not loaded!");
	modelNode = h3dAddNodes(H3DRootNode, modelResource); //TODO enable addition of parent nodes

	if (modelNode == 0) throw Exception("Model node is not added!");
}

} /* namespace SpaceMarines */
