#include "Core.hpp"

namespace SpaceMarines
{

bool Modules::_isInitialized = false;
std::string Modules::assetPath = "";
Vector2 Modules::screenSize = Vector2(1280, 720);
std::string Modules::renderingPipeline = "";

//Defaults
Module::Renderer* Modules::module_renderer = nullptr;
Module::PhysicsWorld* Modules::module_physics = nullptr;
Module::Input* Modules::module_input = nullptr;
Module::GUI* Modules::module_gui = nullptr;

std::list<Modules::Invocable*> Modules::invocables = std::list<Modules::Invocable*>();

Modules::Events Modules::_events = Modules::Events();

void Modules::setAssetPath(const char* assetPath)
{
	Modules::assetPath = std::string(assetPath);
}

void Modules::setRenderingPipelinePath(const char* renderingPipelinePath)
{
	renderingPipeline = std::string(renderingPipelinePath);
}

void Modules::setScreenSize(const Vector2 &screenSize)
{
	Modules::screenSize = screenSize;
}

bool Modules::init()
{
	if (assetPath.empty() || assetPath == "") throw Exception("Asset path is empty!");
	if (renderingPipeline.empty() || renderingPipeline == "") throw Exception("Rendering pipeline path is empty!");

	module_renderer = new Module::Renderer(assetPath, screenSize, renderingPipeline.c_str());
	if (module_renderer == nullptr) throw Exception("Unable to create renderer module");
	module_physics = new Module::PhysicsWorld();
	if (module_physics == nullptr) throw Exception("Unable to create physics module");
	module_input = new Module::Input();
	if (module_input == nullptr) throw Exception("Unable to create input module");
	module_gui = new Module::GUI(module_renderer);
	if (module_gui == nullptr) throw Exception("Unable to create GUI module");

	if (!module_renderer->init()) throw Exception("Error initializing Renderer module");
	module_physics->setDebugDrawer(module_renderer->getDebugDrawer());
	module_input->screenSize = module_renderer->getScreenSize();
	if (!module_gui->init()) throw Exception("Error initializing GUI");

	_isInitialized = true;

	return true;
}

void Modules::cleanup()
{
	if (!_isInitialized) throw Exception("Trying to clean up modules without initializaiton");

	delete module_physics;
	delete module_renderer;
	delete module_input;
	delete module_gui;
	for (auto i : invocables)
		delete i;
}

/*
 * Module return functions. These are all unchecked for performance reasons.
 */
Module::PhysicsWorld& Modules::physics()
	{return *module_physics;}
Module::Renderer& Modules::renderer()
	{return *module_renderer;}
Module::DebugDrawer& Modules::debug()
	{return *module_renderer->getDebugDrawer();}
Module::Input& Modules::input()
	{ return *module_input; }
Module::GUI& Modules::gui()
	{ return *module_gui; }
Modules::Events& Modules::events()
	{ return _events; }

/*
 * Invoker functions
 */


}

