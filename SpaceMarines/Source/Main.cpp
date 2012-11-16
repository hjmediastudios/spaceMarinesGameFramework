/*
 * Main.cpp
 *
 *  Created on: Nov 10, 2012
 *      Author: nick
 */

#include "Prerequisites.hpp"

#include "Resources/ModelResource.hpp"
#include "Components/MeshRenderer.hpp"
#include "Renderer.hpp"
#include "Components/Camera.hpp"

using namespace SpaceMarines;
using namespace std;

int main(int argc, char* argv[])
{
	std::string appPath = Math::extractAppPath(argv[0]);
	std::string assetPath = appPath + "../Assets";

	Renderer* renderer = new SpaceMarines::Renderer(assetPath, Vector2(1280, 720), "pipelines/mypipe.pipeline.xml");
	renderer->init();

    // Add model resource
	H3DRes lightMatRes = h3dAddResource( H3DResTypes::Material, "materials/light.material.xml", 0 );

	if (!renderer->loadResources()) {h3dutDumpMessages(); throw Exception("Unable to load resources");}

    // Add light source
	H3DNode light = h3dAddLightNode( H3DRootNode, "Light1", lightMatRes, "LIGHTING", "SHADOWMAP" );
	h3dSetNodeTransform( light, 0, 50, 20, -30, 0, 0, 1, 1, 1 );
	h3dSetNodeParamF( light, H3DLight::RadiusF, 0, 200 );
	h3dSetNodeParamF( light, H3DLight::FovF, 0, 90 );
	h3dSetNodeParamI( light, H3DLight::ShadowMapCountI, 3 );
	h3dSetNodeParamF( light, H3DLight::ShadowSplitLambdaF, 0, 0.9f );
	h3dSetNodeParamF( light, H3DLight::ShadowMapBiasF, 0, 0.001f );
	h3dSetNodeParamF( light, H3DLight::ColorF3, 0, 0.9f );
	h3dSetNodeParamF( light, H3DLight::ColorF3, 1, 0.9f );
	h3dSetNodeParamF( light, H3DLight::ColorF3, 2, 1.0f );

	//Add camera
	GameObject* camObj = new GameObject();
	camObj->addComponent(new Camera("Hey", renderer));
	camObj->getComponent<Camera>()->setView(Vector2(1280, 720), 45.0f, Vector2(0.5f, 1000.0f));
	renderer->setCamera(camObj->getComponent<Camera>());

	GameObject* minifig = new GameObject();
	minifig->addComponent(new MeshRenderer("models/Monkey/Monkey.scene.xml"));
	minifig->getTransform()->setPosition(Vector3(1.0f, 0.0f, 1.0f));

	renderer->start();
	minifig->start();
    for (int i=0; i < 9000; i++)
    {
        static float t = 0;

        // Increase animation time
        t = t + 10.0f * (1 / 24.0f);
        minifig->getTransform()->rotation = Quaternion(Vector3::UP, t*0.001f);
        minifig->update();
        // Render scene
        renderer->update();
    }

    delete camObj;
//    delete minifig;
    delete renderer;
	return 0;
}




