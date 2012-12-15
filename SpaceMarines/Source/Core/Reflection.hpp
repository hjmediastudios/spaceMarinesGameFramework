#ifndef _Header_Reflection
#define _Header_Reflection

namespace SpaceMarines
{

template <typename TYPE> 
const char* GetTypeName()
{
	return typeid(TYPE).name(); 
}

template <> inline const char* GetTypeName<char>() { return "char"; }
template <> inline const char* GetTypeName<unsigned char>() { return "unsigned char"; }
template <> inline const char* GetTypeName<short>() { return "short"; }
template <> inline const char* GetTypeName<unsigned short>() { return "unsigned short"; }
template <> inline const char* GetTypeName<int>() { return "int"; }
template <> inline const char* GetTypeName<unsigned int>() { return "unsigned int"; }
template <> inline const char* GetTypeName<long>() { return "long"; }
template <> inline const char* GetTypeName<unsigned long>() { return "unsigned long"; }
template <> inline const char* GetTypeName<bool>() { return "bool"; }
template <> inline const char* GetTypeName<float>() { return "float"; }
template <> inline const char* GetTypeName<double>() { return "double"; }
template <> inline const char* GetTypeName<GameObject>() { return "GameObject"; }
template <> inline const char* GetTypeName<Modules::Events>() { return "Modules::Events"; }
template <> inline const char* GetTypeName<Module::DebugDrawer>() { return "Module::DebugDrawer"; }
template <> inline const char* GetTypeName<DebugDrawMode::List>() { return "DebugDrawMode::List"; }
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
template <> inline const char* GetTypeName<Module::GUI>() { return "Module::GUI"; }
template <> inline const char* GetTypeName<Module::Input>() { return "Module::Input"; }
template <> inline const char* GetTypeName<RayCastMode::List>() { return "RayCastMode::List"; }
template <> inline const char* GetTypeName<RayCastHit>() { return "RayCastHit"; }
template <> inline const char* GetTypeName<Module::PhysicsWorld>() { return "Module::PhysicsWorld"; }
template <> inline const char* GetTypeName<Module::Renderer>() { return "Module::Renderer"; }
template <> inline const char* GetTypeName<Exception>() { return "Exception"; }
template <> inline const char* GetTypeName<Time>() { return "Time"; }
template <> inline const char* GetTypeName<Layer>() { return "Layer"; }
template <> inline const char* GetTypeName<Dirtiable>() { return "Dirtiable"; }
template <> inline const char* GetTypeName<HasFixedUpdate>() { return "HasFixedUpdate"; }
template <> inline const char* GetTypeName<ObjectType::List>() { return "ObjectType::List"; }
template <> inline const char* GetTypeName<Object>() { return "Object"; }
template <> inline const char* GetTypeName<GameObject>() { return "GameObject"; }
template <> inline const char* GetTypeName<ComponentType::List>() { return "ComponentType::List"; }
template <> inline const char* GetTypeName<Component>() { return "Component"; }
template <> inline const char* GetTypeName<ActiveComponent>() { return "ActiveComponent"; }
template <> inline const char* GetTypeName<PassiveComponent>() { return "PassiveComponent"; }
template <> inline const char* GetTypeName<Transform>() { return "Transform"; }
template <> inline const char* GetTypeName<Layer>() { return "Layer"; }
template <> inline const char* GetTypeName<Resource>() { return "Resource"; }
template <> inline const char* GetTypeName<ModelResource>() { return "ModelResource"; }
template <> inline const char* GetTypeName<Shader>() { return "Shader"; }
template <> inline const char* GetTypeName<Application>() { return "Application"; }

}

#endif