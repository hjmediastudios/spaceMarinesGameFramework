#pragma once

#include <GL/glfw.h>
#include <GL/gl.h>
#include <string>

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
}
