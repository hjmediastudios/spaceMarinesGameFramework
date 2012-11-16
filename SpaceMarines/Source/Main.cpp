/*
 * Main.cpp
 *
 *  Created on: Nov 10, 2012
 *      Author: nick
 */

#include "Prerequisites.hpp"

#include "Resources/ModelResource.hpp"
#include "Components/MeshRenderer.hpp"
#include "Renderer.hpp"
#include "Components/Camera.hpp"
#include "Components/Light.hpp"

using namespace SpaceMarines;
using namespace std;

int main(int argc, char* argv[])
{
	std::string appPath = Math::extractAppPath(argv[0]);
	std::string assetPath = appPath + "../Assets";

	Renderer* renderer = new SpaceMarines::Renderer(assetPath, Vector2(1280, 720), "pipelines/mypipe.pipeline.xml");
	renderer->init();

	//Add camera
	GameObject* camObj = new GameObject();
	camObj->addComponent(new Camera("Hey", renderer));
	camObj->getComponent<Camera>()->setView(Vector2(1280, 720), 45.0f, Vector2(0.5f, 1000.0f));
	renderer->setCamera(camObj->getComponent<Camera>());

	GameObject* minifig = new GameObject();
	minifig->addComponent(new MeshRenderer("models/Monkey/Monkey.scene.xml"));
	minifig->getTransform()->setPosition(Vector3(1.0f, 0.0f, 1.0f));

	GameObject* plane = new GameObject();
	plane->addComponent(new MeshRenderer("models/Plane/Plane.scene.xml"));
	plane->getTransform()->setPosition(Vector3(0.0f, -1.0f, 0.0f));

	GameObject* light = new GameObject();
	light->addComponent(new PointLight("materials/light.material.xml", 5.0f));
	light->getComponent<PointLight>()->setColor(Vector3::UP);
//	light->getComponent<PointLight>()->setLightRotation(Vector3(-90.0f, 0.0f, 0));
//	light->getComponent<PointLight>()->setAngle(50.0f);
//	light->getComponent<PointLight>()->setShadowMaps(1);
    light->setParent(minifig);
    light->getTransform()->setPosition(Vector3::UP * 3.0f);

	renderer->start();
	minifig->start();
	plane->start();
	light->start();
    for (int i=0; i < 3000; i++)
    {
        static float t = 0;

        // Increase animation time
        t = t + 10.0f * (1 / 24.0f);
        minifig->getTransform()->rotation = Quaternion(Vector3::UP, t*0.001f);

        minifig->update();
        plane->update();
        light->update();
        light->getComponent<PointLight>()->setColor(Vector3(sinf(t*0.01f), 1.0f, cosf(t*0.01f)));
        // Render scene
        renderer->update();
    }

    delete plane;
    delete camObj;
    delete minifig;
    delete light;

    delete renderer;
	return 0;
}




