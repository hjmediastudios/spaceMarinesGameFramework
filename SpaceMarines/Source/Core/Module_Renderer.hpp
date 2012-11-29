#pragma once
#include "Prerequisites.hpp"
#include "../Components/Camera.hpp"
#include "../Resources/Shader.hpp"
#include "Module_Debug.hpp"

namespace SpaceMarines
{

class Camera;

namespace Module
{

class DebugDrawer;

class Renderer
{
public:
	Renderer(const std::string &assetPath, const Vector2& windowSize, const char* pipelineResource);
	virtual ~Renderer();
	bool init();
	void start();
	bool loadResources();
	Vector2 getScreenSize() const;
	H3DRes getPipelineHandle() const { return pipeline; }
	void setCamera(Camera* camera);
	Camera* getCamera();
	void update();
	std::string getAssetPath() const;
	DebugDrawer* getDebugDrawer() { return debugDrawer; }

protected:
	Camera* camera;
	DebugDrawer* debugDrawer;
	friend class DebugDrawer;
private:
	std::string assetPath;
	Vector2 screenSize;
	bool setupWindow();
	H3DRes pipeline;
	std::string pipelineFilePath;
	unsigned short _started;

};

}
} /* namespace SpaceMarines */
