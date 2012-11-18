#include "Light.hpp"

namespace SpaceMarines
{

/**************************
 * General light functions
 **************************/
Light::Light(const char* lightMaterialResourceFilename)
{
	lightMaterialResource = h3dAddResource(H3DResTypes::Material, lightMaterialResourceFilename, 0);
	if (lightMaterialResource == 0) throw Exception("Unable to add light resource");
	lightNode = h3dAddLightNode(H3DRootNode, "SpotLight", lightMaterialResource, "SPOTLIGHT", "SHADOWMAP");
	if (lightNode == 0) throw Exception("Unable to add spot light node");
	Vector3 color = Vector3::ONE;
	for (unsigned short i=0; i < 3; i++)
		h3dSetNodeParamF(lightNode, H3DLight::ColorF3, i, color[i]);
}

Light::~Light()
{
	std::cout << "Deleting light.\n";
	h3dRemoveNode(lightNode);
}

void Light::start()
{
	if (gameObject == nullptr) throw Exception("Light isn't attached to a GameObject");
	h3dSetNodeParent(lightNode, gameObject->getTransform()->getNode());
}

void Light::setColor(const Vector3 &color)
{
	for (unsigned short i=0; i < 3; i++)
	{
		h3dSetNodeParamF(lightNode, H3DLight::ColorF3, i, color[i]);
	}
}

/**************************
 * Point light functions
 **************************/

PointLight::PointLight(const char* lightMaterialResourceFilename, const float radius) : Light(lightMaterialResourceFilename)
{
	h3dSetNodeParamStr(lightNode, H3DLight::LightingContextStr, "POINTLIGHT");
	h3dSetNodeParamF(lightNode, H3DLight::FovF, 0, 360.0f);
	h3dSetNodeParamI(lightNode, H3DLight::ShadowMapCountI, 0);
	setRadius(radius);
}

void PointLight::setRadius(const float radius)
{
	h3dSetNodeParamF(lightNode, H3DLight::RadiusF, 0, radius);
}

/**************************
 * Spot light functions
 **************************/

SpotLight::SpotLight(const char* lightMaterialResourceFilename, const float angleDegrees, const float radius) : Light(lightMaterialResourceFilename)
{
	setRadius(radius);
	setAngle(angleDegrees);
}

void SpotLight::setRadius(const float radius)
{
	h3dSetNodeParamF(lightNode, H3DLight::RadiusF, 0, radius);
}

void SpotLight::setLightRotation(const Quaternion &q)
{
	h3dSetNodeTransformQ(lightNode, 0, 0, 0, q.x, q.y, q.z, q.w, 1, 1, 1);
}

void SpotLight::setLightRotation(const Vector3 &eulerAngles)
{
	h3dSetNodeTransform(lightNode, 0, 0, 0, eulerAngles.x, eulerAngles.y, eulerAngles.z, 1, 1, 1);
}

void SpotLight::setShadowMaps(unsigned short int numShadowMaps)
{
	h3dSetNodeParamI(lightNode, H3DLight::ShadowMapCountI, numShadowMaps > 0 ? numShadowMaps : 0);
}

void SpotLight::setAngle(const float spotAngleDegrees)
{
	h3dSetNodeParamF(lightNode, H3DLight::FovF, 0, spotAngleDegrees);
}




} /* namespace SpaceMarines */
