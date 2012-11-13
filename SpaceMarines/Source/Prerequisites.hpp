#pragma once

#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <map>
#include <typeinfo>
#include <hash_set>

#include <GL/glfw.h>
#include <GL/gl.h>
#include <tinyxml2.h>
#include <horde3d/Horde3D.h>
#include <horde3d/Horde3DUtils.h>
#include <functional>

class Exception : public std::exception
{
public:
	Exception(const std::string& message = "") : message(message) {}
	virtual ~Exception() throw() {}
	const char* what() const throw() {return message.c_str();}
private:
	std::string message;
};

#include "Math.hpp"
#include "HighLevelTypes/Object.hpp"



