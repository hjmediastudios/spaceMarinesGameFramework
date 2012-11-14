#pragma once
#include "../Prerequisites.hpp"

namespace SpaceMarines
{

/************************************************************8
 * Type enums
 */
namespace ObjectType
{ enum _enum {
	Object = 0,
	GameObject = 1,
	Component = 2,
	ActiveComponent = 3,
	PassiveComponent = 4,
	MeshRenderer = 10
};}
typedef ObjectType::_enum ObjectTypeName;

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
{ enum _enum {
	MeshRenderer = 10,
};}
typedef ComponentType::_enum CompTypeName;
class Component : public Object
{
public:
	static const ObjectTypeName type = ObjectType::ActiveComponent;
	Component()
	{
		gameObject = nullptr;
	}
	virtual ~Component() {}
	virtual void start() = 0;
	virtual bool isActive() = 0;
	virtual const char* getComponentType() const = 0;
	void setGameObject(GameObject* obj) { gameObject = obj; }
protected:
	GameObject* gameObject;
};

class ActiveComponent : public Component
{
public:
	static const ObjectTypeName type = ObjectType::ActiveComponent;
	bool isActive() {return true;}
	ActiveComponent() : Component() {}
	virtual ~ActiveComponent() {}
	virtual void start() = 0;
	virtual void update() = 0;
	virtual void fixedUpdate() = 0;
	virtual const char* getComponentType() const = 0;
};

class PassiveComponent : public Component
{
public:
	static const ObjectTypeName type = ObjectType::PassiveComponent;
	bool isActive() {return false;}
	PassiveComponent() : Component() {}
	virtual ~PassiveComponent() {}
	virtual void start() = 0;
	virtual const char* getComponentType() const = 0;
};

/************************************************************8
 * GameObject
 */
class GameObject : public Object
{
public:
	static const ObjectTypeName type = ObjectType::GameObject;
	GameObject() : Object()
	{

	}

	template<typename T>
	T* getComponent()
	{
		try
		{
			return (T*) activeComponents.at(typeid(T).name());
		}
		catch(std::exception ex)
		{
			try
			{
				return (T*) passiveComponents.at(typeid(T).name());
			}
			catch (std::exception ex)
			{
				throw Exception(std::string("Component of type \"") + typeid(T).name() + "\" does not exist in this object.");
			}
		}
		return nullptr;
	}

	template<typename T>
	void addComponent(T* comp)
	{
		if (comp->isActive())
			activeComponents.insert(std::pair<const char*, ActiveComponent*>(typeid(T).name(), (ActiveComponent*) comp));
		else
			passiveComponents.insert(std::pair<const char*, PassiveComponent*>(typeid(T).name(), (PassiveComponent*) comp));

		comp->setGameObject(this);
	}

	virtual ~GameObject()
	{
		for (std::map<const char*, ActiveComponent*>::iterator it = activeComponents.begin(); it != activeComponents.end(); it++)
		{
			delete it->second;
		}
		for (std::map<const char*, PassiveComponent*>::iterator it = passiveComponents.begin(); it != passiveComponents.end(); it++)
		{
			delete it->second;
		}
	}

	void start()
	{
		for (std::map<const char*, ActiveComponent*>::iterator it = activeComponents.begin(); it != activeComponents.end(); it++)
		{
			it->second->start();
		}
		for (std::map<const char*, PassiveComponent*>::iterator it = passiveComponents.begin(); it != passiveComponents.end(); it++)
		{
			it->second->start();
		}
	}

	void update()
	{
		for (std::map<const char*, ActiveComponent*>::iterator it = activeComponents.begin(); it != activeComponents.end(); it++)
		{
			it->second->update();
		}
	}

	void fixedUpdate()
	{
		for (std::map<const char*, ActiveComponent*>::iterator it = activeComponents.begin(); it != activeComponents.end(); it++)
		{
			it->second->fixedUpdate();
		}
	}

private:
	std::map<const char*, ActiveComponent*> activeComponents;
	std::map<const char*, PassiveComponent*> passiveComponents;
};

}

