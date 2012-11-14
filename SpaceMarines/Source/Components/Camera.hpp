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
	void start() {}
	const char* getComponentType() const { return "Camera";}
protected:
	H3DNode cam;
	friend class Renderer;
private:
	Vector2 viewSize;
	float fovDegrees;
	Vector2 clipDistances;
	Renderer* renderer;
};

} /* namespace SpaceMarines */
