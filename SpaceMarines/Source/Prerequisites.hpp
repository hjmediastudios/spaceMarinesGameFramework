#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <typeinfo>
#include <hash_set>

#include <tinyxml2.h>
#include <horde3d/Horde3D.h>
#include <horde3d/Horde3DUtils.h>

#include <bullet/btBulletDynamicsCommon.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glfw.h>



namespace SpaceMarines
{
	class Exception : public std::exception
	{
	public:
		Exception(const std::string& message = "") : message(message) {}
		virtual ~Exception() throw() {}
		const char* what() const throw() {return message.c_str();}
	private:
		std::string message;
	};

	struct Time
	{
		static double time_d;
		static double deltaTime_d;
		static float time;
		static float deltaTime;
		static const double fixedDeltaTime_d;
		static const float fixedDeltaTime;
		static float fps;
		static const float animationFPS;
	};

	typedef unsigned short Layer;
	static const unsigned short Constant_MaxNumLayers = 10;
}

#include "Math.hpp"
#include "Input.hpp"

#include "HighLevelTypes/Interfaces.hpp"
#include "HighLevelTypes/Object.hpp"

#define PP_DebugDraw





