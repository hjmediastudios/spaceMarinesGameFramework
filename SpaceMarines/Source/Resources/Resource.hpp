#pragma once

#include "../Core/Prerequisites.hpp"

namespace SpaceMarines
{

class Resource
{
public:
	Resource(const std::string &filename = "") : filename(filename) {}
	std::string getFilename() const { return filename; }
protected:
	std::string filename;
};

}
