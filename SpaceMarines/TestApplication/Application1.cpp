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
	runner = new GameObject();
	runner->addComponent(new AnimatedMeshRenderer("models/Minifig/Minifig.scene.xml"));
	runner->getComponent<AnimatedMeshRenderer>()->addAnimation("animations/Minifig/Legs_Run.anim", "Run", "Root");
	runner->getComponent<AnimatedMeshRenderer>()->addAnimation("animations/Minifig/Body_Run.anim", "Body_Run", "Torso.Lower");
	runner->getTransform()->setPosition(Vector3(0, 0, 0));
	runner->getTransform()->setRotation(Quaternion(Vector3::UP, Math::degToRad(45)));
	runner->addComponent(new BoxCollider());
	runner->addComponent(new RigidBody(runner->getComponent<BoxCollider>(), 1.0f));
	addObject(runner);

	GameObject* camera = new GameObject();
	camera->addComponent(new Camera("Camera", renderer));
	renderer->setCamera(camera->getComponent<Camera>());
	camera->getTransform()->setPosition(Vector3(0, 2, -3));
	camera->getTransform()->setRotation(Quaternion(Vector3::UP, Math::degToRad(180.0f)));
//	camera->setParent(runner);
	addObject(camera);

	GameObject* runnerLamp = new GameObject();
	runnerLamp->addComponent(new SpotLight("materials/light.material.xml", 90.0f, 25.0f));
	runnerLamp->getComponent<SpotLight>()->setLightRotation(Vector3(-150.0f, 0.0f, 0.0f));
	runnerLamp->getTransform()->setPosition(Vector3(0.0f, 1.5f, 0.5f));
	runnerLamp->getComponent<SpotLight>()->setColor(Vector3(2.0f, 2.0f, 1.5f));
	runnerLamp->setParent(runner);
	addObject(runnerLamp);

	GameObject* lamp = new GameObject();
	lamp->addComponent(new SpotLight("materials/light.material.xml", 80.0f, 25.0f));
	lamp->getComponent<SpotLight>()->setLightRotation(Vector3(-90.0f, 20, 0));
	lamp->getComponent<SpotLight>()->setColor(Vector3::ONE * 0.25f);
	lamp->getTransform()->setPosition(Vector3(0, 10, 0));
	lamp->getComponent<SpotLight>()->setShadowMaps(1);
	addObject(lamp);
	lamp->setParent(runner);

	GameObject* sun = new GameObject();
	sun->addComponent(new DirectionalLight("materials/light.material.xml"));
	sun->getComponent<DirectionalLight>()->setColor(Vector3::ONE * 0.8f);
	sun->getComponent<DirectionalLight>()->setLightDirection(Quaternion(Vector3::RIGHT, 20.0f));
	sun->getTransform()->setPosition(Vector3::UP * 3);
	sun->getTransform()->setRotation(Quaternion(Vector3::RIGHT, -80.0f));
	addObject(sun);

	GameObject* plane = new GameObject();
	plane->addComponent(new MeshRenderer("models/Plane/Plane.scene.xml"));
	addObject(plane);
}

bool running = false;

void Application1::customLogicLoop()
{
	running = false;
	if (Input::isKeyPressed(KeyCodes::Right))
	{
		runner->getTransform()->rotate(Quaternion(Vector3::UP, -3.0f * Time::deltaTime));
		running = true;
	}
	if (Input::isKeyPressed(KeyCodes::Left))
	{
		runner->getTransform()->rotate(Quaternion(Vector3::UP, 3.0f * Time::deltaTime));
		running = true;
	}
	if (Input::isKeyPressed(KeyCodes::Up))
	{
		runner->getTransform()->translate(runner->getTransform()->forward() * 3.0f * Time::deltaTime);
		running = true;
	}
	if (Input::isKeyPressed(KeyCodes::Down))
	{
		runner->getTransform()->translate(-runner->getTransform()->forward() * 3.0f * Time::deltaTime);
		running = true;
	}

	if (running)
	{
		runner->getComponent<AnimatedMeshRenderer>()->playAnimation("Run", 0, 0.25f);
		runner->getComponent<AnimatedMeshRenderer>()->playAnimation("Body_Run", 1, 1.0f);
	}
}

} /* namespace SpaceMarines */
