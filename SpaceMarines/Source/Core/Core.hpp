#pragma once

#include "Prerequisites.hpp"
#include "Module_Renderer.hpp"
#include "Module_Physics.hpp"
#include "Module_Input.hpp"
#include "Module_GUI.hpp"

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

	static Module::PhysicsWorld& physics();
	static Module::Renderer& renderer();
	static Module::DebugDrawer& debug();
	static Module::Input &input();
	static Module::GUI &gui();

private:
	static std::string assetPath;
	static std::string renderingPipeline;
	static Vector2 screenSize;

private:
	static bool _isInitialized;

	//Core modules
	static Module::Renderer* module_renderer;
	static Module::PhysicsWorld* module_physics;
	static Module::Input* module_input;
	static Module::GUI* module_gui;
};

}
