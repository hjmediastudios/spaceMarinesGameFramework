#include "Application.hpp"
#include "HighLevelTypes/Object.hpp"

namespace SpaceMarines
{

const double Time::fixedDeltaTime = 0.01;
double Time::time = 0.0;
double Time::deltaTime = 0.0;

Application::Application(const char* assetPath, const char* renderingPipeline, const Vector2 &windowSize, bool drawDebug)
{
	renderer = new Renderer(assetPath, windowSize, renderingPipeline);
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
		currentTime = newTime;

		accumulator += Time::deltaTime;
		while(accumulator >= Time::fixedDeltaTime)
		{
			//Fixed update
			accumulator -= Time::fixedDeltaTime;
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
	renderer->update();
}

void Application::addObject(GameObject* obj)
{
	objects.push_back(obj);
}

} /* namespace SpaceMarines */
