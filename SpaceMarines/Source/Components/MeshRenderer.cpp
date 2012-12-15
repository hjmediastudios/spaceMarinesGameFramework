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
	h3dRemoveNode(modelNode);
}

void MeshRenderer::registerStart()
{
	if (modelNode != 0) return;
	if (gameObject == nullptr) throw Exception("SpotLight isn't attached to a GameObject");
	if (!h3dIsResLoaded(modelResource)) throw Exception("Model resource is not loaded!");
	modelNode = h3dAddNodes(gameObject->getTransform()->getNode(), modelResource);

	if (modelNode == 0) throw Exception("Model node is not added!");
}

H3DNode MeshRenderer::getModelNode() const
{
	return modelNode;
}

void MeshRenderer::getMeshParameters(H3DNode node)
{

}


/*************************************
 * 	Animated Mesh Renderer stuff
 *************************************/
AnimatedMeshRenderer::AnimatedMeshRenderer(const char* assetPath) : MeshRenderer(assetPath)
{
	clips = AnimationClipMap();
}

AnimatedMeshRenderer::~AnimatedMeshRenderer()
{

}

void AnimatedMeshRenderer::addAnimation(const char* assetPath, const char* animName, const char* rootNode)
{
	try
	{
		clips.at(std::string(animName));
	}
	catch (std::exception &ex)
	{
		AnimationClip clip;
		clip.animationRes = h3dAddResource(H3DResTypes::Animation, assetPath, 0);
		if (clip.animationRes == 0) throw Exception("Can't load animation from path \"" + std::string(assetPath) + "\"");
		clip.name = animName;
		h3dSetupModelAnimStage(modelNode, 0, clip.animationRes, 0, rootNode, false);
		clip.additive = false;
		clip.layer = 0;
		clip.rootNode = rootNode;

		std::cout << "Added animation clip \"" << clip.name << "\"\n";
		clips.insert(std::pair<std::string, AnimationClip>(clip.name, clip));
	}
}

void AnimatedMeshRenderer::playAnimation(const char* animationName, unsigned short layer, float weight, bool additive)
{
	AnimationClip* clip;
	try
	{
		clip = &clips.at(std::string(animationName));
	}
	catch(std::exception &ex)
	{
		throw Exception("Clip \"" + std::string(animationName) + "\" doesn't exist");
	}

	if (clip->layer > 15)
		throw Exception("Only layers 0-15 are supported for skeletal animation");

	if (clip->layer != layer || clip->weight != weight || clip->additive != additive)
	{
		clip->animationTime = 0.0f;
		clip->layer = layer;
		clip->weight = weight;
		clip->additive = additive;
		h3dSetupModelAnimStage(modelNode, clip->layer, clip->animationRes, clip->layer, clip->rootNode, clip->additive);
	}

	if (weight > 0.0f)
		h3dSetModelAnimParams(modelNode, clip->layer, (clip->animationTime += Time::deltaTime_d) * Time::animationFPS, clip->weight);

}


} /* namespace SpaceMarines */
