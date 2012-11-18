/*
 * Application1.cpp
 *
 *  Created on: Nov 16, 2012
 *      Author: nick
 */

#include "Application1.hpp"

namespace SpaceMarines
{

Application1::Application1(const char* assetPath, const char* renderingPipeline, const Vector2 &windowSize, bool drawDebug) : Application(assetPath, renderingPipeline, windowSize, drawDebug)
{

}

Application1::~Application1()
{
}

void Application1::customSetupFunction()
{
	GameObject* camera = new GameObject();
	camera->addComponent(new Camera("Camera", renderer));
	renderer->setCamera(camera->getComponent<Camera>());
	camera->getTransform()->setPosition(Vector3(5, 1, 0));
	camera->getTransform()->setRotation(Quaternion(Vector3::UP, Math::degToRad(90.0f)));
	addObject(camera);

	runner = new GameObject();
	runner->addComponent(new MeshRenderer("models/Box/Box.scene.xml", true));
	runner->getComponent<MeshRenderer>()->addAnimation("animations/Box/Run.anim", "Root");
	runner->getTransform()->setPosition(Vector3(0, 0, 0));
	runner->getTransform()->setRotation(Quaternion(Vector3::UP, Math::degToRad(45)));
	runner->addComponent(new PointLight("materials/light.material.xml", 10.0f));
	runner->getComponent<PointLight>()->setColor(Vector3(100.0f, 100.0f, 90.0f));
	addObject(runner);

	GameObject* lamp = new GameObject();
	lamp->addComponent(new SpotLight("materials/light.material.xml", 80.0f, 25.0f));
	lamp->getComponent<SpotLight>()->setLightRotation(Vector3(-100.0f, 20, 0));
	lamp->getTransform()->setPosition(Vector3(1, 10, 0));
	lamp->getComponent<SpotLight>()->setShadowMaps(1);
	addObject(lamp);

	GameObject* plane = new GameObject();
	plane->addComponent(new MeshRenderer("models/Plane/Plane.scene.xml"));
	addObject(plane);
}

void Application1::customLogicLoop()
{
	if (Input::isKeyPressed(KeyCodes::Right))
		runner->getTransform()->rotate(Quaternion(Vector3::UP, -3.0f * Time::deltaTime));
	if (Input::isKeyPressed(KeyCodes::Left))
		runner->getTransform()->rotate(Quaternion(Vector3::UP, 3.0f * Time::deltaTime));
	if (Input::isKeyPressed(KeyCodes::Up))
		runner->getTransform()->translate(runner->getTransform()->forward() * 3.0f * Time::deltaTime);
}

} /* namespace SpaceMarines */