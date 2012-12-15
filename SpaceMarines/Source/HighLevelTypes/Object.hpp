#pragma once
#include "../Core/Prerequisites.hpp"
#include "../Core/Reflection.hpp"
#include "Interfaces.hpp"
#include "../Core/Event.hpp"

namespace SpaceMarines
{

/************************************************************8
 * Type enums
 */
namespace ObjectType
{ enum List {
	Object = 0,
	GameObject = 1,
	Component = 2,
	ActiveComponent = 3,
	PassiveComponent = 4,
	MeshRenderer = 10
};}
typedef ObjectType::List ObjectTypeName;

/************************************************************8
 * Basic object class
 */
class Object
{
public:
	Object() {}
	static const ObjectTypeName type = ObjectType::Object;
};

/************************************************************8
 * Component handling
 */
class GameObject;
namespace ComponentType
{ enum List {
	MeshRenderer = 10,
};}
typedef ComponentType::List CompTypeName;
class Component : public Object
{
public:
	static const ObjectTypeName type = ObjectType::ActiveComponent;
	Component()
	{
		gameObject = nullptr;
	}
	virtual ~Component() {}
	virtual void registerStart() = 0;
	virtual const char* getComponentType() const = 0;
	void setGameObject(GameObject* obj) { gameObject = obj; }
	GameObject* getGameObject() { return gameObject; }
protected:
	GameObject* gameObject;
};

/************************************************************8
 * GameObject
 */

class Transform
{
public:
	Vector3 position;
	Vector3 scale;
	Quaternion rotation;
public:
	~Transform()
	{
		h3dRemoveNode(transformNode);
	}
	Transform(const Transform &other)
	{
		position = other.position;
		rotation = other.rotation;
		scale = other.scale;
		transformNode = h3dAddGroupNode(H3DRootNode, "");
		if (transformNode == 0) throw Exception("Group node couldn't be added");
	}
	Transform(const Vector3 &position, const Quaternion &rotation, const Vector3 &scale = Vector3::ONE)
	{
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
		transformNode = h3dAddGroupNode(H3DRootNode, "");
		if (transformNode == 0) throw Exception("Group node couldn't be added");
	}

	void setPosition(const Vector3 &position) { this->position = position; }
	void setRotation(const Quaternion &rotation) { this->rotation = rotation; }
	void setScale(const Vector3 &scale) { this->scale = scale; }
	void rotate(const Quaternion &q) { this->rotation = q * this->rotation; }
	void translate(const Vector3 &v) { position += v; }

	void setForward(const Vector3 &fwd)
	{
		this->rotation = Quaternion::rotationBetweenVectors(Vector3::UP, fwd);
	}

	Vector3 forward() const { return rotation * Vector3::FORWARD; }
	Vector3 up() const { return rotation * Vector3::UP; }
	Vector3 right() const { return rotation * Vector3::RIGHT; }

	void update()
	{
		setHorde3DNodeTransform(transformNode);
	}

	Vector3 getPosition() const { return position; }
	Vector3 getScale() const { return scale; }
	Quaternion getRotation() const { return rotation; }

	void setHorde3DNodeTransform(H3DNode node)
	{
		h3dSetNodeTransformQ(node, position.x, position.y, position.z,
								  rotation.x, rotation.y, rotation.z, rotation.w,
								  scale.x, scale.y, scale.z);
	}

	void lookInDirection(const Vector3 &direction, const Vector3 &up = Vector3::UP);
	void lookAt(const Vector3 &pt, const Vector3 &up = Vector3::UP);

	H3DNode getNode() const {return transformNode;}
protected:
	H3DNode transformNode;
};

class GameObject : public Object
{
public:
	Event<> eventUpdate;
	Event<> eventFixedUpdate;

	static const ObjectTypeName type = ObjectType::GameObject;
	GameObject() : Object(), transform(Vector3::ZERO, Quaternion::IDENTITY, Vector3::ONE)
	{
		parent = nullptr;
		layer = 0;
	}

	Layer getLayer() const
	{
		return layer;
	}

	template<typename T>
	T* getComponent()
	{
		try
		{
			return (T*) components.at(GetTypeName<T>());
		}
		catch(std::exception &ex)
		{
			return nullptr;
		}
		return nullptr;
	}

	void setLayer(Layer layer)
	{
		if ((layer + 1) > Constant_MaxNumLayers) throw Exception("Can't set layers higher than the maximum");
		this->layer = layer;
	}

	template<typename T>
	void addComponent(T* comp)
	{
		Component* c = dynamic_cast<Component*>(comp);
		components.insert(std::pair<const char*, Component*>(GetTypeName<T>(), c));
		c->setGameObject(this);
	}

	virtual ~GameObject()
	{
		for (std::map<const char*, Component*>::iterator it = components.begin(); it != components.end(); it++)
		{
			delete it->second;
		}
	}

	void start()
	{
		for (std::map<const char*, Component*>::iterator it = components.begin(); it != components.end(); it++)
		{
			it->second->registerStart();
		}
	}

	void update()
	{
		eventUpdate.fire();
		transform.update();
	}
	void fixedUpdate()
	{
		eventFixedUpdate.fire();
		transform.update();
	}

	Transform* getTransform()
	{
		return &transform;
	}

	void setParent(GameObject* parent)
	{
		if (this == parent) throw Exception("Cannot parent an object to itself!");
		this->parent = parent;
		if (parent != nullptr)
			h3dSetNodeParent(transform.getNode(), parent->getTransform()->getNode());
	}

	GameObject* getParent()
	{
		return parent;
	}

protected:
	Transform transform;
	Layer layer;
	GameObject* parent;
private:
	std::map<const char*, Component*> components;
};

}

