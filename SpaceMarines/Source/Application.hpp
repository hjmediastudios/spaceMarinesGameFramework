#pragma once
#include "Prerequisites.hpp"
#include "Renderer.hpp"
#include "Physics.hpp"
#include "HighLevelTypes/Object.hpp"
#include "Components/MeshRenderer.hpp"
#include "Components/Light.hpp"

namespace SpaceMarines
{

class Application
{
public:
	Application(const char* assetPath, const char* renderingPipeline, const Vector2 &windowSize, bool drawDebug);
	virtual ~Application();
	virtual void customSetupFunction() = 0;
	virtual void customLogicLoop() = 0;
	void start();
protected:
	Renderer* renderer;
	PhysicsWorld* physics;
	void addObject(GameObject* obj);
private:
	bool init();
	std::string assetPath;
	std::string renderingPipeline;
	std::vector<GameObject*> objects;
	void update();
	bool drawDebug;
};

} /* namespace SpaceMarines */
