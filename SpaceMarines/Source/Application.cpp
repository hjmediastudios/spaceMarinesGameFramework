#include "Application.hpp"
#include "HighLevelTypes/Object.hpp"

namespace SpaceMarines
{

const double Time::fixedDeltaTime_d = 0.01;
const float Time::fixedDeltaTime = (float) Time::fixedDeltaTime_d;

double Time::time_d = 0.0;
float Time::time = 0.0f;

double Time::deltaTime_d = 0.0;
float Time::deltaTime = 0.0f;

float Time::fps = 0.0f;
const float Time::animationFPS = 24.0f;

Application::Application(const char* assetPath, const char* renderingPipeline, const Vector2 &windowSize, bool drawDebug)
{
	objects = std::vector<GameObject*>();
	this->drawDebug = drawDebug;
	this->assetPath = std::string(assetPath);
	this->renderingPipeline = std::string(renderingPipeline);
	this->screenSize = windowSize;
	//TODO screensize
}

Application::~Application()
{
	for (size_t i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}
	Modules::cleanup();
}

bool Application::init()
{
	Modules::setAssetPath(assetPath.c_str());
	Modules::setRenderingPipelinePath(renderingPipeline.c_str());
	Modules::setScreenSize(screenSize);
	Modules::init();


	return true;
}

void Application::start()
{
	init();
	customInitFunction();

	Modules::renderer().start();

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->start();
	}

	std::cout << "Done starting\n";

	customStartFunction();

	Time::time_d = 0.0;
	double currentTime = glfwGetTime();
	double accumulator = 0.0;

	//Run main loop
	while (!Modules::input().isKeyPressed(KeyCodes::Esc))
	{
		Modules::input().pollMouse();
		double newTime = glfwGetTime();
		Time::deltaTime_d = newTime - currentTime;
		Time::deltaTime = (float) Time::deltaTime_d;
		Time::fps = 1.0f / Time::deltaTime;
		currentTime = newTime;

		accumulator += Time::deltaTime_d;
		while(accumulator >= Time::fixedDeltaTime_d)
		{
			//Fixed update
			fixedUpdate();
			accumulator -= Time::fixedDeltaTime_d;
			Time::time_d += Time::fixedDeltaTime_d;
			Time::time += Time::fixedDeltaTime;
		}

		update();
	}
}

void Application::update()
{
	customLogicLoop();
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->update();
	}
	Modules::physics().debugDraw();
	Modules::renderer().update();
}

void Application::fixedUpdate()
{
	Modules::physics().fixedUpdate();
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->fixedUpdate();
	}
}

void Application::addObject(GameObject* obj)
{
	objects.push_back(obj);
}

} /* namespace SpaceMarines */
