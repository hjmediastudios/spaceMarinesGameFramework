#pragma once
#include "Prerequisites.hpp"

namespace SpaceMarines
{

class Renderer
{
public:
	Renderer(const std::string &assetPath, const Vector2& windowSize);
	virtual ~Renderer();
	bool init();
	Vector2 getWindowSize() const;
private:
	std::string assetPath;
	Vector2 windowSize;
	bool setupWindow();
};

} /* namespace SpaceMarines */
