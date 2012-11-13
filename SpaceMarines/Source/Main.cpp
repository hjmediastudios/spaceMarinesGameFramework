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

using namespace SpaceMarines;
using namespace std;

int main(int argc, char* argv[])
{
	std::string appPath = Math::extractAppPath(argv[0]);
	std::string assetPath = appPath + "../Assets";

//	Renderer* renderer = new Renderer(appPath, Vector2(1280, 720));
//	renderer->init();

//	H3DNode model = 0, cam = 0;
//
//
//    // Add pipeline resource
//    H3DRes pipeRes = h3dAddResource( H3DResTypes::Pipeline, "pipelines/deferred.pipeline.xml", 0 );
//    // Add model resource
//    H3DRes modelRes = h3dAddResource( H3DResTypes::SceneGraph, "models/Monkey/Monkey.scene.xml", 0 );
//	H3DRes lightMatRes = h3dAddResource( H3DResTypes::Material, "materials/light.material.xml", 0 );
//    // Add animation resource
//    // Load added resources
//    if (!h3dutLoadResourcesFromDisk(assetPath.c_str())) {h3dutDumpMessages(); throw Exception("Error loading resources!");}
//
//    // Add model to scene
//    model = h3dAddNodes( H3DRootNode, modelRes );
//
//    // Add light source
//	H3DNode light = h3dAddLightNode( H3DRootNode, "Light1", lightMatRes, "LIGHTING", "SHADOWMAP" );
//	h3dSetNodeTransform( light, 0, 50, 20, -30, 0, 0, 1, 1, 1 );
//	h3dSetNodeParamF( light, H3DLight::RadiusF, 0, 200 );
//	h3dSetNodeParamF( light, H3DLight::FovF, 0, 90 );
//	h3dSetNodeParamI( light, H3DLight::ShadowMapCountI, 3 );
//	h3dSetNodeParamF( light, H3DLight::ShadowSplitLambdaF, 0, 0.9f );
//	h3dSetNodeParamF( light, H3DLight::ShadowMapBiasF, 0, 0.001f );
//	h3dSetNodeParamF( light, H3DLight::ColorF3, 0, 0.9f );
//	h3dSetNodeParamF( light, H3DLight::ColorF3, 1, 0.9f );
//	h3dSetNodeParamF( light, H3DLight::ColorF3, 2, 1.0f );
//
//
//    // Add camera
//    cam = h3dAddCameraNode( H3DRootNode, "Camera", pipeRes );
//
//    // Setup viewport and render target sizes
//    h3dSetNodeParamI( cam, H3DCamera::ViewportXI, 0 );
//    h3dSetNodeParamI( cam, H3DCamera::ViewportYI, 0 );
//
//    Vector2 winSize = renderer->getWindowSize();
//    h3dSetNodeParamI( cam, H3DCamera::ViewportWidthI, winSize.x );
//    h3dSetNodeParamI( cam, H3DCamera::ViewportHeightI, winSize.y );
//    h3dSetupCameraView( cam, 45.0f, (float)winSize.x / winSize.y, 0.5f, 2048.0f );
//    h3dResizePipelineBuffers( pipeRes, winSize.x, winSize.y );
//
//    for (int i=0; i < 5000; i++)
//    {
//        static float t = 0;
//
//        // Increase animation time
//        t = t + 10.0f * (1 / 24.0f);
//
//        // Set new model position
//        h3dSetNodeTransform( model, 0, 0, 0,  // Translation
//                             0, 0, 0,              // Rotation
//                             1, 1, 1 );            // Scale
//    	h3dSetNodeTransform( cam, 0, 1.8f, 10.0f, 0 , 0, 0, 1, 1, 1 );
//        // Render scene
//        h3dRender( cam );
//
//        // Finish rendering of frame
//        h3dFinalizeFrame();
//        h3dutDumpMessages();
//        glfwSwapBuffers();
//    }

	//Test model ability
	GameObject* object = new GameObject();
	object->addComponent(new MeshRenderer());
	MeshRenderer* mr = object->getComponent<MeshRenderer>();
	cout << mr->getComponentType() << endl;
	delete object;

//    delete renderer;
	return 0;
}




