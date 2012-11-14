#include "Camera.hpp"

namespace SpaceMarines
{

Camera::Camera(const std::string &name, Renderer* renderer)
{
	if ((cam = h3dAddCameraNode(H3DRootNode, name.c_str(), renderer->getPipelineHandle())) == 0)
		throw Exception("Failed to create camera");
	fovDegrees = 60.0f;
	clipDistances = Vector2(0.1f, 1000.0f);
	this->renderer = renderer;
	gameObject = nullptr;
}

Camera::~Camera()
{

}

void Camera::setView(const Vector2 &size, const float fovDegrees, const Vector2 &clipDistances)
{
	this->viewSize = size;
	this->fovDegrees = fovDegrees;
	this->clipDistances = clipDistances;

	if (gameObject == nullptr) throw Exception("Camera's GameObject is null!");
	h3dSetNodeParamI(cam, H3DCamera::ViewportXI, 0);
	h3dSetNodeParamI(cam, H3DCamera::ViewportYI, 0);
	h3dSetNodeParamI(cam, H3DCamera::ViewportWidthI, size.x);
	h3dSetNodeParamI(cam, H3DCamera::ViewportHeightI, size.y);

	h3dSetupCameraView(cam, fovDegrees, viewSize.x / viewSize.y, clipDistances.x, clipDistances.y);
    h3dResizePipelineBuffers(renderer->getPipelineHandle(), viewSize.x, viewSize.y);

	h3dSetNodeTransform(cam, 0, 1.8f, 10.0f, 0 , 0, 0, 1, 1, 1);
}

} /* namespace SpaceMarines */
