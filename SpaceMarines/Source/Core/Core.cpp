#include "Core.hpp"

namespace SpaceMarines
{

bool Modules::_isInitialized = false;
std::string Modules::assetPath = "";
Vector2 Modules::screenSize = Vector2(1280, 720);
std::string Modules::renderingPipeline = "";

//Defaults
Renderer* Modules::module_renderer = nullptr;
PhysicsWorld* Modules::module_physics = nullptr;

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

	module_renderer = new Renderer(assetPath, screenSize, renderingPipeline.c_str());
	if (module_renderer == nullptr) throw Exception("Unable to create renderer module");
	module_physics = new PhysicsWorld();
	if (module_physics == nullptr) throw Exception("Unable to create physics module");

	if (!module_renderer->init()) throw Exception("Error initializing Renderer module");
	module_physics->setDebugDrawer(module_renderer->getDebugDrawer());

	_isInitialized = true;
	return true;
}

void Modules::cleanup()
{
	if (!_isInitialized) throw Exception("Trying to clean up modules without initializaiton");
	delete module_physics;
	delete module_renderer;
}

/**
 * Module return functions
 */
PhysicsWorld& Modules::physics()
	{return *module_physics;}
Renderer& Modules::renderer()
	{return *module_renderer;}
DebugDrawer& Modules::debug()
	{return *module_renderer->getDebugDrawer();}

}

