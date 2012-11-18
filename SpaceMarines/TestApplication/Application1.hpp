#pragma once
#include "../Source/Application.hpp"

namespace SpaceMarines
{

class Application1 : public Application
{
public:
	Application1(const char* assetPath, const char* renderingPipeline, const Vector2 &windowSize, bool drawDebug);
	void customSetupFunction();
	void customLogicLoop();
	virtual ~Application1();
private:
	GameObject* runner;
};

} /* namespace SpaceMarines */
