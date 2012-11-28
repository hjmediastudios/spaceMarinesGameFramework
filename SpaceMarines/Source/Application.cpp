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

Vector2 Input::screenSize = Vector2(1, 1);
Vector2 Input::mouseViewportPosition = Vector2(0, 0);
Vector2 Input::lastMouseViewportPosition = Vector2(0, 0);

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
	physics->setDebugDrawer(renderer->getDebugDrawer());
	if (drawDebug) h3dSetOption(H3DOptions::DebugViewMode, 1);
	return true;
}

void Application::start()
{
	init();
	customSetupFunction();

	renderer->start();
	Input::screenSize = renderer->getScreenSize();

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->start();
	}

	std::cout << "Done starting\n";

	Time::time_d = 0.0;
	double currentTime = glfwGetTime();
	double accumulator = 0.0;

	//Run main loop
	while (!Input::isKeyPressed(KeyCodes::Esc))
	{
		Input::pollMouse();
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
	physics->debugDraw();
	renderer->update();
}

void Application::fixedUpdate()
{
	physics->fixedUpdate();
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
