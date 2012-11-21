#include "Application.hpp"
#include "HighLevelTypes/Object.hpp"

namespace SpaceMarines
{

const double Time::fixedDeltaTime = 0.01;
const float Time::fixedDeltaTimeF = (float) Time::fixedDeltaTime;

double Time::time = 0.0;
float Time::timeF = 0.0f;

double Time::deltaTime = 0.0;
float Time::deltaTimeF = 0.0f;

float Time::fps = 0.0f;
const float Time::animationFPS = 24.0f;

Application::Application(const char* assetPath, const char* renderingPipeline, const Vector2 &windowSize, bool drawDebug)
{
	renderer = new Renderer(assetPath, windowSize, renderingPipeline);
	physics = PhysicsWorld::getSingleton();
	objects = std::vector<GameObject*>();
	this->drawDebug = drawDebug;
}

Application::~Application()
{
	for (size_t i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}
	delete renderer;
	delete physics;
}

bool Application::init()
{
	if (!renderer->init()) return false;
	if (drawDebug) h3dSetOption(H3DOptions::DebugViewMode, 1);
	return true;
}

void Application::start()
{
	init();
	customSetupFunction();

	renderer->start();

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->start();
	}

	std::cout << "Done starting\n";

	Time::time = 0.0;
	double currentTime = glfwGetTime();
	double accumulator = 0.0;

	//Run main loop
	while (!Input::isKeyPressed(KeyCodes::Esc))
	{
		double newTime = glfwGetTime();
		Time::deltaTime = newTime - currentTime;
		Time::deltaTimeF = (float) Time::deltaTime;
		Time::fps = 1.0f / Time::deltaTimeF;
		currentTime = newTime;

		accumulator += Time::deltaTime;
		while(accumulator >= Time::fixedDeltaTime)
		{
			//Fixed update
			accumulator -= Time::fixedDeltaTime;
			Time::time += Time::fixedDeltaTime;
			Time::timeF += Time::fixedDeltaTimeF;
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
	renderer->update();
}

void Application::addObject(GameObject* obj)
{
	objects.push_back(obj);
}

} /* namespace SpaceMarines */
