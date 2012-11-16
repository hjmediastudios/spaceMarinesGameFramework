#pragma once
#include "../Prerequisites.hpp"

namespace SpaceMarines
{

class MeshRenderer : public PassiveComponent
{
public:
	static const ObjectTypeName type = ObjectType::MeshRenderer;
	MeshRenderer(const char* assetPath);
	virtual ~MeshRenderer();
	virtual void start() {}
	virtual const char* getComponentType() const { return "MeshRenderer"; }
protected:
	H3DRes modelResource;
};

} /* namespace SpaceMarines */
