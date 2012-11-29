#pragma once
#include "../Core/Prerequisites.hpp"

namespace SpaceMarines
{

/*************************************
 * 	Mesh Renderer class
 *************************************/
class MeshRenderer : public ActiveComponent
{
public:
	static const ObjectTypeName type = ObjectType::MeshRenderer;
	MeshRenderer(const char* assetPath);
	virtual ~MeshRenderer();
	void start();
	void update();
	virtual const char* getComponentType() const { return "MeshRenderer"; }
protected:
	H3DRes modelResource;
	H3DNode modelNode;

};

/*************************************
 * 	Animated Mesh Renderer class
 *************************************/
class AnimatedMeshRenderer : public MeshRenderer
{
public:
	virtual const char* getComponentType() const { return "AnimatedMeshRenderer"; }
	AnimatedMeshRenderer(const char* assetPath);
	virtual ~AnimatedMeshRenderer();
	void addAnimation(const char* assetPath, const char* animName, const char* rootNode);
	void playAnimation(const char* animationName,
			unsigned short layer = 0, float weight = 1.0f, bool additive = false);
	//ToDo getBone(const char* name)
private:
	struct AnimationClip
	{
		std::string name;
		H3DRes animationRes;

		unsigned short layer;
		float weight;
		bool additive;
		const char* rootNode;

		float animationTime;
	};

	typedef std::map<std::string, AnimationClip> AnimationClipMap;
	AnimationClipMap clips;
};

} /* namespace SpaceMarines */
