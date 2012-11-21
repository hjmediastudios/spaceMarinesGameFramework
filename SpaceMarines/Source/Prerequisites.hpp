#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <typeinfo>
#include <hash_set>

#include <GL/glfw.h>
#include <GL/gl.h>
#include <tinyxml2.h>
#include <horde3d/Horde3D.h>
#include <horde3d/Horde3DUtils.h>
#include <bullet/btBulletDynamicsCommon.h>


class Exception : public std::exception
{
public:
	Exception(const std::string& message = "") : message(message) {}
	virtual ~Exception() throw() {}
	const char* what() const throw() {return message.c_str();}
private:
	std::string message;
};

namespace SpaceMarines
{
	struct Time
	{
		static double time;
		static double deltaTime;
		static float timeF;
		static float deltaTimeF;
		static const double fixedDeltaTime;
		static const float fixedDeltaTimeF;
		static float fps;
		static const float animationFPS;
	};
}

#include "Math.hpp"
#include "Input.hpp"

#include "HighLevelTypes/Interfaces.hpp"
#include "HighLevelTypes/Object.hpp"

//#define DEBUG



