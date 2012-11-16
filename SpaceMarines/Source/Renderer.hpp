#pragma once
#include "Prerequisites.hpp"
#include "Components/Camera.hpp"
namespace SpaceMarines
{

class Camera;

class Renderer
{
public:
	Renderer(const std::string &assetPath, const Vector2& windowSize, const char* pipelineResource);
	virtual ~Renderer();
	bool init();
	void start();
	bool loadResources();
	Vector2 getWindowSize() const;
	H3DRes getPipelineHandle() const { return pipeline; }
	void setCamera(Camera* camera);
	void update();
	std::string getAssetPath() const;
private:
	std::string assetPath;
	Vector2 windowSize;
	bool setupWindow();
	H3DRes pipeline;
	std::string pipelineFilePath;
	Camera* camera;
	unsigned short _started;
};

} /* namespace SpaceMarines */
