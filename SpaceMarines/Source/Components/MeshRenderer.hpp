#pragma once
#include "../Prerequisites.hpp"

namespace SpaceMarines
{

class MeshRenderer : public ActiveComponent
{
public:
	static const ObjectTypeName type = ObjectType::MeshRenderer;
	MeshRenderer(const char* assetPath, bool isAnimated = false);
	virtual ~MeshRenderer();
	void start();
	void update();
	virtual const char* getComponentType() const { return "MeshRenderer"; }
	bool addAnimation(const char* animationName, const char* rootName, unsigned short animationStage = 0, unsigned short layer = 0, bool additive = false);
protected:
	H3DRes modelResource;
	H3DNode modelNode;
	bool _isAnimated;
private:
	struct AnimationEntry
	{
		H3DRes animation;
		unsigned short stage;
		unsigned short layer;
		bool isAdditive;
		const char* rootNode;
		AnimationEntry(H3DRes res, unsigned short stage, unsigned short layer, const char* rootNode, bool isAdditive)
		{
			animation = res;
			this->stage = stage;
			this->layer = layer;
			this->isAdditive = isAdditive;
			this->rootNode = rootNode;
		}
	};
	std::vector<AnimationEntry> animations;
};

} /* namespace SpaceMarines */
