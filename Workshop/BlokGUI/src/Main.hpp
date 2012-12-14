#pragma once

#include <GL/glfw.h>
#include <GL/gl.h>
#include <string>
#include <iostream>
#include <math.h>
#include <sstream>

namespace BlokGUI
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

	static float clamp(float val, float min, float max)
	{
		if (val > max)
			return max;
		else if (val < min)
			return min;
		else
			return val;
	}

}
