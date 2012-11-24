#include "Camera.hpp"

namespace SpaceMarines
{

Camera::Camera(const std::string &name, Renderer* renderer)
{
	if ((cameraNode = h3dAddCameraNode(H3DRootNode, name.c_str(), renderer->getPipelineHandle())) == 0)
		throw Exception("Failed to create camera");
	fovDegrees = 60.0f;
	clipDistances = Vector2(0.1f, 1000.0f);
	this->renderer = renderer;
	gameObject = nullptr;
	projMat = Matrix4();
	setView(renderer->getWindowSize(), 60.0f, Vector2(0.1f, 1000.0f));
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
    h3dResizePipelineBuffers(renderer->getPipelineHandle(), viewSize.x, viewSize.y);

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

} /* namespace SpaceMarines */
