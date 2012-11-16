#pragma once
#include "../Prerequisites.hpp"

namespace SpaceMarines
{

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

} /* namespace SpaceMarines */
