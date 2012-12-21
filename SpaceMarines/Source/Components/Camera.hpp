#pragma once
#include "../Core/Prerequisites.hpp"

namespace SpaceMarines
{

namespace Module{ class Renderer; }

class Camera : public Component
{
public:
	Camera(const std::string &name);
	virtual ~Camera();
	void setView(const Vector2 &size, const float fovDegrees, const Vector2 &clipDistances);
	void registerStart();
	const char* getComponentType() const { return "Camera";}
	Matrix4 getProjectionMatrix() const { return projMat; }
	Matrix4 getViewMatrix() const;

	Ray getPickRayViewport(const Vector2 &viewportCoordinates) const;
	Ray getPickRayScreen(const Vector2 &screenCoordinates) const;

	Vector2 viewportToScreen(const Vector2 &co) const;
	Vector2 screenToViewport(const Vector2 &co) const;

	Vector2 worldToViewport(const Vector3 &worldCoordinates) const;
	Vector2 worldToScreen(const Vector3 &worldCoordiantes) const;

protected:
	H3DNode cameraNode;
	friend class Module::Renderer;
private:
	Vector2 viewSize;
	float fovDegrees;
	Vector2 clipDistances;
	Matrix4 projMat;
	Matrix4 viewMat;
};

} /* namespace SpaceMarines */
