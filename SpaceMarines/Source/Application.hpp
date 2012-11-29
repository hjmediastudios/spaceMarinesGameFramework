#pragma once
#include "Core/Prerequisites.hpp"
#include "HighLevelTypes/Object.hpp"
#include "Core/Core.hpp"

#include "Components/MeshRenderer.hpp"
#include "Components/Light.hpp"
#include "Components/Collider.hpp"

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
	void addObject(GameObject* obj);
private:
	bool init();
	std::string assetPath;
	std::string renderingPipeline;
	std::vector<GameObject*> objects;
	Vector2 screenSize;
	void update();
	void fixedUpdate();
	bool drawDebug;
};

} /* namespace SpaceMarines */
