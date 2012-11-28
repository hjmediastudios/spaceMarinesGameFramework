#pragma once
#include "../Source/Application.hpp"

namespace SpaceMarines
{

class Minifig : public GameObject
{
public:
	Minifig(const Vector3 &position) : GameObject()
	{
		addComponent(new AnimatedMeshRenderer("models/Minifig/Minifig.scene.xml"));
		getComponent<AnimatedMeshRenderer>()->addAnimation("animations/Minifig/Legs_Run.anim", "Run", "Root");
		getComponent<AnimatedMeshRenderer>()->addAnimation("animations/Minifig/Body_Run.anim", "Body_Run", "Torso.Lower");
		transform.setPosition(position);
		transform.setRotation(Quaternion(Vector3::UP, Math::degToRad(45)));
		addComponent(new CapsuleCollider(0.5f, 1.6f, Vector3(0, 0.8f, 0)));
		addComponent(new RigidBody(getComponent<CapsuleCollider>(), 185.0f));
		getComponent<RigidBody>()->setLockRotation(true);
	}
};

class Application1 : public Application
{
public:
	Application1(const char* assetPath, const char* renderingPipeline, const Vector2 &windowSize, bool drawDebug);
	void customSetupFunction();
	void customLogicLoop();
	virtual ~Application1();
private:
#define Constant_NumMinifigs 25
	GameObject* runners[Constant_NumMinifigs];
};

} /* namespace SpaceMarines */
