#pragma once
#include "../Prerequisites.hpp"
#include "../Renderer.hpp"

namespace SpaceMarines
{

class Renderer;

class Camera : public PassiveComponent
{
public:
	Camera(const std::string &name, Renderer* renderer);
	virtual ~Camera();
	void setView(const Vector2 &size, const float fovDegrees, const Vector2 &clipDistances);
	void start();
	const char* getComponentType() const { return "Camera";}
	Matrix4 getProjectionMatrix() const { return projMat; }
	Matrix4 getViewMatrix() const;
protected:
	H3DNode cameraNode;
	friend class Renderer;
private:
	Vector2 viewSize;
	float fovDegrees;
	Vector2 clipDistances;
	Renderer* renderer;
	Matrix4 projMat;
};

} /* namespace SpaceMarines */
