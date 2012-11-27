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
	renderer->getDebugDrawer()->setDrawMode(DebugDrawMode::SystemAndBullet);

	for (size_t i = 0; i < Constant_NumMinifigs; i++)
	{
		runners[i] = new Minifig(Vector3(
				Math::randomFloatInRange(-50.0f, 50.0f),
				Math::randomFloatInRange(5.0f, 10.0f),
				Math::randomFloatInRange(-50.0f, 50.0f)));
		addObject(runners[i]);
	}

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
	plane->getTransform()->setScale(Vector3(10.0f));
	plane->addComponent(new GroundPlaneCollider());
	plane->addComponent(new RigidBody(plane->getComponent<GroundPlaneCollider>(), 0.0f));
	addObject(plane);
}

void Application1::customLogicLoop()
{

	Transform* cameraTrans = renderer->getCamera()->getGameObject()->getTransform();
	//FixMe poll for mouse position
	cameraTrans->rotate(Quaternion(Vector3::UP, -Input::getMouseViewportPos().x * 3.0f * Time::deltaTimeF));
	cameraTrans->rotate(Quaternion(cameraTrans->right(), -Input::getMouseViewportPos().y * 1.25f * Time::deltaTimeF));

	for (int i=0; i < Constant_NumMinifigs; i++)
	{
		GameObject* runner = runners[i];
		float speed = runner->getComponent<RigidBody>()->getVelocity().lengthSquared();
		bool running = (speed > Math::Epsilon);

		Vector3 pt = physics->rayCastComplex(renderer->getCamera()->getPickRayViewport(Input::getMouseViewportPos()), 100.0f).point;
		renderer->getDebugDrawer()->drawAxis(pt, 1.1f);

		if (Input::isMouseButtonPressed(0))
		{
			if (pt == Vector3::ZERO)
				pt = runner->getTransform()->getPosition() + Vector3::UP * 50.0f;
			renderer->getDebugDrawer()->drawLine(pt, runner->getTransform()->getPosition(), Vector3::UP);
			runner->getComponent<RigidBody>()->applyForce((pt - runner->getTransform()->getPosition()).normalized() * 50.0f, ForceMode::Impulse);
		}

		if (running)
		{
			runner->getComponent<AnimatedMeshRenderer>()->playAnimation("Run", 0, 0.25f);
			runner->getComponent<AnimatedMeshRenderer>()->playAnimation("Body_Run", 1, 1.0f);
		}
	}


}

} /* namespace SpaceMarines */
