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
	runner->getTransform()->setPosition(Vector3(0, 5, 0));
	runner->getTransform()->setRotation(Quaternion(Vector3::UP, Math::degToRad(45)));
	runner->addComponent(new CapsuleCollider(0.5f, 1.6f, Vector3(0, 0.8f, 0)));
	runner->addComponent(new RigidBody(runner->getComponent<CapsuleCollider>(), 185.0f));
	addObject(runner);

	GameObject* camera = new GameObject();
	camera->addComponent(new Camera("Camera", renderer));
	renderer->setCamera(camera->getComponent<Camera>());
	camera->getTransform()->setPosition(Vector3(0, 2, -3));
	camera->getTransform()->setRotation(Quaternion(Vector3::UP, Math::degToRad(180.0f)));
//	camera->setParent(runner);
	addObject(camera);

	GameObject* shadowLamp = new GameObject();
	shadowLamp->addComponent(new SpotLight("materials/light.material.xml", 80.0f, 25.0f));
	shadowLamp->getComponent<SpotLight>()->setLightRotation(Vector3(-60.0f, 90.0f, 0));
	shadowLamp->getComponent<SpotLight>()->setColor(Vector3::ONE * 0.5f);
	shadowLamp->getTransform()->setPosition(Vector3(10, 10, 0));
	shadowLamp->getComponent<SpotLight>()->setShadowMaps(1);
	addObject(shadowLamp);

	GameObject* sun = new GameObject();
	sun->addComponent(new DirectionalLight("materials/light.material.xml"));
	sun->getComponent<DirectionalLight>()->setColor(Vector3::ONE * 0.8f);
	sun->getComponent<DirectionalLight>()->setLightDirection(Quaternion(Vector3::RIGHT, 20.0f));
	sun->getTransform()->setPosition(Vector3::UP * 3);
	sun->getTransform()->setRotation(Quaternion(Vector3::RIGHT, -80.0f));
	addObject(sun);

	GameObject* plane = new GameObject();
	plane->addComponent(new MeshRenderer("models/Plane/Plane.scene.xml"));
	plane->addComponent(new StaticPlaneCollider());
	plane->addComponent(new RigidBody(plane->getComponent<StaticPlaneCollider>(), 0.0f));
	addObject(plane);
}

bool running = false;

void Application1::customLogicLoop()
{
	float speed = runner->getComponent<RigidBody>()->getVelocity().lengthSquared();
//	renderer->getDebugDrawer()->drawLine(Vector3::ZERO, runner->getTransform()->getPosition());
	renderer->getDebugDrawer()->drawAxis(runner->getTransform()->position, 2.0f, runner->getTransform()->rotation);
	running = (speed > Math::Epsilon);

	Vector3 pt = renderer->getPositionFromViewport(Input::getMousePosNormalized());
	std::cout << pt << std::endl;
	renderer->getDebugDrawer()->drawAxis(pt, 2.0f);

	Transform* cameraTrans = renderer->getCamera()->getTransform();

	if (Input::isKeyPressed(KeyCodes::Up))
		runner->getComponent<RigidBody>()->applyForce(Vector3::FORWARD * 80.0f);
	if (Input::isKeyPressed(KeyCodes::Down))
		runner->getComponent<RigidBody>()->applyForce(Vector3::FORWARD * -80.0f);
	if (Input::isKeyPressed(KeyCodes::Left))
		cameraTrans->rotate(Quaternion(cameraTrans->up(), Time::deltaTimeF * 3));
	if (Input::isKeyPressed(KeyCodes::Right))
		cameraTrans->rotate(Quaternion(cameraTrans->up(), Time::deltaTimeF * -3));

	if (running)
	{
		runner->getComponent<AnimatedMeshRenderer>()->playAnimation("Run", 0, 0.25f);
		runner->getComponent<AnimatedMeshRenderer>()->playAnimation("Body_Run", 1, 1.0f);
	}


}

} /* namespace SpaceMarines */
