#include "MeshRenderer.hpp"

namespace SpaceMarines
{

MeshRenderer::MeshRenderer(const char* assetPath, bool isAnimated)
{
	if((modelResource = h3dAddResource(H3DResTypes::SceneGraph, assetPath, 0)) == 0)
		throw Exception(std::string("Unable to add mesh resource \"") + assetPath + "\"");
	modelNode = 0;
	_isAnimated = isAnimated;
	if (!_isAnimated) return;
	animations = std::vector<AnimationEntry>();
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
	if (!_isAnimated) return;
	for (unsigned short i=0; i < animations.size(); i++)
	{
		h3dSetupModelAnimStage(modelNode, animations[i].stage, animations[i].animation, 0, animations[i].rootNode, animations[i].isAdditive);
	}
}

void MeshRenderer::update()
{
	if (!_isAnimated) return;
	for (unsigned short i=0; i < animations.size(); i++)
	{
		h3dSetModelAnimParams(modelNode, animations[i].stage, Time::time * 24.0f, 1.0f);
	}
}

bool MeshRenderer::addAnimation(const char* animationName, const char* rootName, unsigned short animationStage, unsigned short layer, bool additive)
{
	if (!_isAnimated) return false;

	H3DRes res = h3dAddResource(H3DResTypes::Animation, animationName, 0);
	if (res == 0) throw Exception(std::string("Can't find animation ") + animationName);
	animations.push_back(AnimationEntry(res, animationStage, layer, rootName, additive));
}

} /* namespace SpaceMarines */
