#include "Camera.hpp"
#include "../Core/Core.hpp"

namespace SpaceMarines
{

Camera::Camera(const std::string &name)
{
	if ((cameraNode = h3dAddCameraNode(H3DRootNode, name.c_str(), Modules::renderer().getPipelineHandle())) == 0)
		throw Exception("Failed to create camera");
	fovDegrees = 60.0f;
	clipDistances = Vector2(0.1f, 1000.0f);
	gameObject = nullptr;
	projMat = Matrix4();
	setView(Modules::renderer().getScreenSize(), 60.0f, Vector2(0.1f, 1000.0f));
}

Camera::~Camera()
{
	std::cout << "Deleting camera.\n";
}

void Camera::setView(const Vector2 &size, const float fovDegrees, const Vector2 &clipDistances)
{
	this->viewSize = size;
	this->fovDegrees = fovDegrees;
	this->clipDistances = clipDistances;

	h3dSetNodeParamI(cameraNode, H3DCamera::ViewportXI, 0);
	h3dSetNodeParamI(cameraNode, H3DCamera::ViewportYI, 0);
	h3dSetNodeParamI(cameraNode, H3DCamera::ViewportWidthI, size.x);
	h3dSetNodeParamI(cameraNode, H3DCamera::ViewportHeightI, size.y);

	h3dSetupCameraView(cameraNode, fovDegrees, viewSize.x / viewSize.y, clipDistances.x, clipDistances.y);
    h3dResizePipelineBuffers(Modules::renderer().getPipelineHandle(), viewSize.x, viewSize.y);

	h3dSetNodeTransform(cameraNode, 0, 1.8f, 10.0f, 0 , 0, 0, 1, 1, 1);

	h3dGetCameraProjMat(cameraNode, projMat.ptr());
	projMat = Matrix4::initPerspMat(fovDegrees, size.x / size.y, clipDistances.x, clipDistances.y);
}

void Camera::start()
{
	if (gameObject == nullptr) throw Exception("Camera isn't attached to a GameObject");
	h3dSetNodeParent(cameraNode, gameObject->getTransform()->getNode());
}

Matrix4 Camera::getViewMatrix() const
{
	const float* view = 0;
	h3dGetNodeTransMats(cameraNode, 0, &view);
	return Matrix4(view).inverted();
}

Vector2 Camera::viewportToScreen(const Vector2 &co) const
{
	return Vector2((co.x + 1.0f) * 0.5f, (co.y + 1.0f) * 0.5f) * Modules::renderer().getScreenSize();
}

Vector2 Camera::screenToViewport(const Vector2 &co) const
{
	return co / Modules::renderer().getScreenSize() * 2.0f - 1.0f;
}

Ray Camera::getPickRayViewport(const Vector2 &viewportCoordinates) const
{
	Ray ray;
	h3dutPickRay(cameraNode, viewportCoordinates.x * 0.5f + 0.5f,
			-viewportCoordinates.y * 0.5f + 0.5f,
			&ray.origin.x, &ray.origin.y, &ray.origin.z,
			&ray.direction.x, &ray.direction.y, &ray.direction.z);
	ray.direction.normalize();
	return ray;
}

Ray Camera::getPickRayScreen(const Vector2 &screenCoordinates) const
{
	return getPickRayViewport(screenToViewport(screenCoordinates));
}

} /* namespace SpaceMarines */
