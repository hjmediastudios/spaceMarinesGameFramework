/*
 * Main.cpp
 *
 *  Created on: Nov 10, 2012
 *      Author: nick
 */

#include "Core/Prerequisites.hpp"

#include "../TestApplication/Application1.hpp"


using namespace SpaceMarines;
using namespace std;

int main(int argc, char* argv[])
{
	std::string appPath = Math::extractAppPath(argv[0]);
	std::string assetPath = appPath + "../Assets";

	Application1* app = new Application1(assetPath.c_str(), "pipelines/mypipe.pipeline.xml", Vector2(1280, 720), false);
	app->start();

	delete app;

	return 0;
}




