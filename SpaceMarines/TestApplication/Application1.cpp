/*
 * Application1.cpp
 *
 *  Created on: Nov 16, 2012
 *      Author: nick
 */

#include "Application1.hpp"

namespace SpaceMarines
{
float Minifig::speed = 1000.0f;

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


void Application1::customInitFunction()
{
	Modules::debug().setDrawMode(DebugDrawMode::SystemOnly);

	for (size_t i = 0; i < Constant_NumMinifigs; i++)
	{
		runners[i] = new Minifig(Vector3(
				Math::randomFloatInRange(-15.0f, 15.0f),
				Math::randomFloatInRange(5.0f, 10.0f),
				Math::randomFloatInRange(-15.0f, 15.0f)));
		addObject(runners[i]);
	}

	GameObject* camera = new GameObject();
	camera->addComponent(new Camera("Camera"));
	Modules::renderer().setCamera(camera->getComponent<Camera>());
	camera->getTransform()->setPosition(Vector3(0, 2, -3));
	camera->getTransform()->setRotation(Quaternion(Vector3::UP, Math::degToRad(180.0f)));
	addObject(camera);

	GameObject* shadowLamp = new GameObject();
	shadowLamp->addComponent(new SpotLight("materials/light.material.xml", 80.0f, 250.0f));
	shadowLamp->getComponent<SpotLight>()->setLightRotation(Vector3(-60.0f, 90.0f, 0));
	shadowLamp->getComponent<SpotLight>()->setColor(Vector3::ONE );
	shadowLamp->getTransform()->setPosition(Vector3(50, 50, 0));
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
	plane->getTransform()->setScale(Vector3(1.0f));
	plane->addComponent(new TriangleMeshColliderStatic(plane->getComponent<MeshRenderer>()));
	plane->addComponent(new RigidBody(plane->getComponent<TriangleMeshColliderStatic>(), 0.0f));
	plane->getTransform()->setPosition(Vector3::ZERO);
	addObject(plane);

	for (unsigned int i=0; i < Constant_NumCubes; i++)
	{
		cubes[i] = new GameObject();
		cubes[i]->addComponent(new MeshRenderer("models/Box/Box.scene.xml"));

		cubes[i]->getTransform()->setPosition(Vector3(Math::randomFloatInRange(-25.0f, 25.0f), 2.0f, Math::randomFloatInRange(-25.0f, 25.0f)));
		addObject(cubes[i]);
	}

//	GameObject* plane2 = new GameObject();
//	plane2->addComponent(new GroundPlaneCollider());
//	plane2->addComponent(new RigidBody(plane2->getComponent<GroundPlaneCollider>(), 0.0f));
//	addObject(plane2);
	Modules::gui().addPanel("Settings");
}

void Application1::customStartFunction()
{
	for (unsigned int i = 0; i < Constant_NumCubes; i++)
	{
		Vector3 pt = Vector3(Math::randomFloatInRange(-25.0f, 25.0f),
				25.0f, Math::randomFloatInRange(-25.0f, 25.0f));
		RayCastHit hit = Modules::physics().rayCastComplex(pt, -Vector3::UP, 28.0f);
		cubes[i]->getTransform()->position = hit.point + Vector3::UP * 0.5f;
		cubes[i]->getTransform()->rotation = Quaternion::rotationBetweenVectors(Vector3::UP, hit.normal);
		cubes[i]->addComponent(new BoxCollider(Vector3::ONE * 2.0f));
		cubes[i]->addComponent(new RigidBody(cubes[i]->getComponent<BoxCollider>(), 0.0f));
		cubes[i]->start();
	}
}

Vector3 targetPoint = Vector3::ZERO;
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

	if (Modules::input().isMouseButtonPressed(0))
		targetPoint = Modules::physics().rayCastComplex(Modules::renderer().getCamera()->getPickRayViewport(Modules::input().getMouseViewportPos()), 100.0f).point;

	Modules::renderer().getDebugDrawer()->drawAxis(targetPoint, 1.1f);

	for (unsigned int i=0; i < Constant_NumMinifigs; i++)
	{
		if (runners[i] == nullptr)
			continue;
		GameObject* runner = runners[i];
		if (runner->getTransform()->getPosition().y < -2.0f)
		{
//			delete runner; StartHere deletion of objects
			continue;
		}

		float speed = runner->getComponent<RigidBody>()->getVelocity().lengthSquared();
		bool running = (speed > Math::Epsilon);

		Vector3 ptToTarget = targetPoint - runner->getTransform()->getPosition();
		Modules::debug().drawLine(runners[i]->getTransform()->getPosition(), targetPoint, Vector3::FORWARD);

		if (ptToTarget.lengthSquared() > 2.0f)
			runner->getComponent<RigidBody>()->applyForce(Vector3(ptToTarget.x, 0, ptToTarget.z).normalized() * Minifig::speed, ForceMode::Force);
		else
			runner->getComponent<RigidBody>()->setVelocity(Vector3::ZERO);

		runner->getTransform()->lookInDirection(runner->getComponent<RigidBody>()->getVelocityHorizontal());

		if (running)
		{
			runner->getComponent<AnimatedMeshRenderer>()->playAnimation("Run", 0, 0.25f);
			runner->getComponent<AnimatedMeshRenderer>()->playAnimation("Body_Run", 1, 1.0f);
		}
	}

//	std::cout << Time::fps << std::endl;

}

} /* namespace SpaceMarines */
