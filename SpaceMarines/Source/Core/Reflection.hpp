#ifndef _Header_Reflection
#define _Header_Reflection

namespace SpaceMarines
{

template <typename TYPE> 
const char* GetTypeName()
{
	return typeid(TYPE).name(); 
}

class GameObject;
class Events;
class DebugDrawer;
class DebugVertex;
class Rectangle;
class Math::BinaryFile;
class Vector2;
class Vector3;
class Vector4;
class Matrix3;
class Quaternion;
class Matrix4;
class Plane;
class Ray;
class GUI;
class Input;
class RayCastHit;
class PhysicsWorld;
class Renderer;
class Exception;
class Time;
class Dirtiable;
class HasFixedUpdate;
class Object;
class GameObject;
class Component;
class ActiveComponent;
class PassiveComponent;
class Transform;
class Resource;
class ModelResource;
class Application;

template <> inline const char* GetTypeName<char>() { return "char"; }
template <> inline const char* GetTypeName<unsigned char>() { return "unsigned char"; }
template <> inline const char* GetTypeName<short>() { return "short"; }
template <> inline const char* GetTypeName<int>() { return "int"; }
template <> inline const char* GetTypeName<unsigned int>() { return "unsigned int"; }
template <> inline const char* GetTypeName<long>() { return "long"; }
template <> inline const char* GetTypeName<unsigned long>() { return "unsigned long"; }
template <> inline const char* GetTypeName<bool>() { return "bool"; }
template <> inline const char* GetTypeName<float>() { return "float"; }
template <> inline const char* GetTypeName<double>() { return "double"; }
template <> inline const char* GetTypeName<Events>() { return "Modules::Events"; }
template <> inline const char* GetTypeName<DebugDrawer>() { return "Module::DebugDrawer"; }
template <> inline const char* GetTypeName<DebugVertex>() { return "DebugVertex"; }
template <> inline const char* GetTypeName<Rectangle>() { return "Rectangle"; }
template <> inline const char* GetTypeName<Math::BinaryFile>() { return "Math::BinaryFile"; }
template <> inline const char* GetTypeName<Vector2>() { return "Vector2"; }
template <> inline const char* GetTypeName<Vector3>() { return "Vector3"; }
template <> inline const char* GetTypeName<Vector4>() { return "Vector4"; }
template <> inline const char* GetTypeName<Matrix3>() { return "Matrix3"; }
template <> inline const char* GetTypeName<Quaternion>() { return "Quaternion"; }
template <> inline const char* GetTypeName<Matrix4>() { return "Matrix4"; }
template <> inline const char* GetTypeName<Plane>() { return "Plane"; }
template <> inline const char* GetTypeName<Ray>() { return "Ray"; }
template <> inline const char* GetTypeName<GUI>() { return "Module::GUI"; }
template <> inline const char* GetTypeName<Input>() { return "Module::Input"; }
template <> inline const char* GetTypeName<RayCastHit>() { return "RayCastHit"; }
template <> inline const char* GetTypeName<PhysicsWorld>() { return "Module::PhysicsWorld"; }
template <> inline const char* GetTypeName<Renderer>() { return "Module::Renderer"; }
template <> inline const char* GetTypeName<Exception>() { return "Exception"; }
template <> inline const char* GetTypeName<Time>() { return "Time"; }
template <> inline const char* GetTypeName<Dirtiable>() { return "Dirtiable"; }
template <> inline const char* GetTypeName<HasFixedUpdate>() { return "HasFixedUpdate"; }
template <> inline const char* GetTypeName<Object>() { return "Object"; }
template <> inline const char* GetTypeName<GameObject>() { return "GameObject"; }
template <> inline const char* GetTypeName<Component>() { return "Component"; }
template <> inline const char* GetTypeName<ActiveComponent>() { return "ActiveComponent"; }
template <> inline const char* GetTypeName<PassiveComponent>() { return "PassiveComponent"; }
template <> inline const char* GetTypeName<Transform>() { return "Transform"; }
template <> inline const char* GetTypeName<Layer>() { return "Layer"; }
template <> inline const char* GetTypeName<Resource>() { return "Resource"; }
template <> inline const char* GetTypeName<ModelResource>() { return "ModelResource"; }
template <> inline const char* GetTypeName<Application>() { return "Application"; }

}

#endif
