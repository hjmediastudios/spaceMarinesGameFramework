#pragma once

#include "../Prerequisites.hpp"
#include "Module_Renderer.hpp"
#include "Module_Physics.hpp"

namespace SpaceMarines
{

class Modules
{
public:
	static bool init();
	static void cleanup();

	static void setAssetPath(const char* assetPath);
	static void setRenderingPipelinePath(const char* renderingPipelinePath);
	static void setScreenSize(const Vector2 &screenSize);

	static PhysicsWorld& physics();
	static Renderer& renderer();
	static DebugDrawer& debug();

private:
	static std::string assetPath;
	static std::string renderingPipeline;
	static Vector2 screenSize;

private:
	static bool _isInitialized;

	//Core modules
	static Renderer* module_renderer;
	static PhysicsWorld* module_physics;
};

}
