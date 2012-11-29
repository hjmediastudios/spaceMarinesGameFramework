#pragma once
#include "../Prerequisites.hpp"
#include "../Core/Module_Renderer.hpp"

namespace SpaceMarines
{

class Renderer;

class Camera : public PassiveComponent
{
public:
	Camera(const std::string &name);
	virtual ~Camera();
	void setView(const Vector2 &size, const float fovDegrees, const Vector2 &clipDistances);
	void start();
	const char* getComponentType() const { return "Camera";}
	Matrix4 getProjectionMatrix() const { return projMat; }
	Matrix4 getViewMatrix() const;

	Ray getPickRayViewport(const Vector2 &viewportCoordinates) const;
	Ray getPickRayScreen(const Vector2 &screenCoordinates) const;

	Vector2 viewportToScreen(const Vector2 &co) const;
	Vector2 screenToViewport(const Vector2 &co) const;

protected:
	H3DNode cameraNode;
	friend class Renderer;
private:
	Vector2 viewSize;
	float fovDegrees;
	Vector2 clipDistances;
	Matrix4 projMat;
};

} /* namespace SpaceMarines */
