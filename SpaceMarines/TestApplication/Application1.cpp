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
	for (size_t i = 0; i < Constant_NumMinifigs; i++ )
	{
		delete runners[i];
	}
	std::cout << "Done!\n";
}

void Application1::customSetupFunction()
{
	Modules::debug().setDrawMode(DebugDrawMode::None);

	for (size_t i = 0; i < Constant_NumMinifigs; i++)
	{
		runners[i] = new Minifig(Vector3(
				Math::randomFloatInRange(-50.0f, 50.0f),
				Math::randomFloatInRange(5.0f, 10.0f),
				Math::randomFloatInRange(-50.0f, 50.0f)));
		addObject(runners[i]);
	}

	GameObject* camera = new GameObject();
	camera->addComponent(new Camera("Camera"));
	Modules::renderer().setCamera(camera->getComponent<Camera>());
	camera->getTransform()->setPosition(Vector3(0, 2, -3));
	camera->getTransform()->setRotation(Quaternion(Vector3::UP, Math::degToRad(180.0f)));
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
	plane->getTransform()->setScale(Vector3(10.0f));
	plane->addComponent(new GroundPlaneCollider());
	plane->addComponent(new RigidBody(plane->getComponent<GroundPlaneCollider>(), 0.0f));
	addObject(plane);
}

void Application1::customLogicLoop()
{

	Transform* cameraTrans = Modules::renderer().getCamera()->getGameObject()->getTransform();
	if (Modules::input().isMouseButtonPressed(1))
	{
		cameraTrans->rotate(Quaternion(Vector3::UP, -Modules::input().getMouseViewportPos().x * 3.0f * Time::deltaTime * (fabs(Modules::input().getMouseViewportPos().x) > 0.1f)));
		cameraTrans->rotate(Quaternion(cameraTrans->right(), -Modules::input().getMouseViewportPos().y * 1.25f * Time::deltaTime * (fabs(Modules::input().getMouseViewportPos().y) > 0.1f)));
	}
	if (Modules::input().isKeyPressed('W'))
		cameraTrans->translate(-cameraTrans->forward() * Time::deltaTime * 10.0f);
	if (Modules::input().isKeyPressed('S'))
		cameraTrans->translate(cameraTrans->forward() * Time::deltaTime * 10.0f);
	if (Modules::input().isKeyPressed('A'))
		cameraTrans->translate(-cameraTrans->right() * Time::deltaTime * 10.0f);
	if (Modules::input().isKeyPressed('D'))
		cameraTrans->translate(cameraTrans->right() * Time::deltaTime * 10.0f);

	Vector3 pt = Modules::physics().rayCastComplex(Modules::renderer().getCamera()->getPickRayViewport(Modules::input().getMouseViewportPos()), 100.0f).point;
	Modules::renderer().getDebugDrawer()->drawAxis(pt, 1.1f);

	for (int i=0; i < Constant_NumMinifigs; i++)
	{
		GameObject* runner = runners[i];
		float speed = runner->getComponent<RigidBody>()->getVelocity().lengthSquared();
		bool running = (speed > Math::Epsilon);

		if (Modules::input().isMouseButtonPressed(0))
		{
			Modules::debug().drawLine(runner->getTransform()->getPosition(), pt);
			runner->getComponent<RigidBody>()->applyForce((pt - runner->getTransform()->getPosition()), ForceMode::Impulse);
		}
		runner->getTransform()->lookInDirection(runner->getComponent<RigidBody>()->getVelocityHorizontal());

		if (running)
		{
			runner->getComponent<AnimatedMeshRenderer>()->playAnimation("Run", 0, 0.25f);
			runner->getComponent<AnimatedMeshRenderer>()->playAnimation("Body_Run", 1, 1.0f);
		}
	}


}

} /* namespace SpaceMarines */
