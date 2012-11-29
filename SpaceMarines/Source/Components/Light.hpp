#pragma once
#include "../Core/Prerequisites.hpp"

namespace SpaceMarines
{

class Light : public PassiveComponent
{
public:
	Light(const char* lightMaterialResourceFilename);
	virtual ~Light();
	void start();
	void setColor(const Vector3 &color);
protected:
	H3DNode lightNode;
	H3DRes lightMaterialResource;
};

class PointLight : public Light
{
public:
	const char* getComponentType() const { return "PointLight";}
	PointLight(const char* lightMaterialResourceFilename, const float radius);
	void setRadius(const float radius);
};

class DirectionalLight : public Light
{
public:
	const char* getComponentType() const { return "DirectionalLight";}
	DirectionalLight(const char* lightMaterialResourceFilename);
	void setLightDirection(const Vector3 &direction);
	void setLightDirection(const Quaternion& q);
};

class SpotLight : public Light
{
public:
	SpotLight(const char* lightMaterialResourceFilename, const float angleDegrees, const float radius);
	const char* getComponentType() const { return "SpotLight";}
	void setLightRotation(const Quaternion &q);
	void setLightRotation(const Vector3 &eulerAngles);
	void setAngle(const float spotAngleDegrees);
	void setRadius(const float radius);
	void setShadowMaps(const unsigned short numShadowMaps);
};

} /* namespace SpaceMarines */
