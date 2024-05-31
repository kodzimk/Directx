#include "Camera.h"

Camera::Camera()
{
	this->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->posVector = XMLoadFloat3(&this->pos);
	this->rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateViewMatrix();
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
	this->projectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}

const XMMATRIX& Camera::GetViewMatrix() const
{
	return this->viewMatrix;
}

const XMMATRIX& Camera::GetProjectionMatrix() const
{
	return this->projectionMatrix;
}

const XMVECTOR& Camera::GetPositionVector() const
{
	return this->posVector;
}

const XMFLOAT3& Camera::GetPositionFloat3() const
{
	return this->pos;
}

const XMVECTOR& Camera::GetRotationVector() const
{
	return this->rotVector;
}

const XMFLOAT3& Camera::GetRotationFloat3() const
{
	return this->rot;
}


void Camera::SetPosition(float x, float y, float z)
{
	this->pos = XMFLOAT3(x, y, z);
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateViewMatrix();
}

void Camera::AdjustPosition(const XMVECTOR& pos)
{
	this->posVector += pos;
	XMStoreFloat3(&this->pos, this->posVector);
	this->UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z)
{
	this->rot = XMFLOAT3(x, y, z);
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateViewMatrix();
}


void Camera::AdjustRotation(float x, float y, float z)
{
	this->rot.x += x;
	this->rot.y += y;
	this->rot.z += z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateViewMatrix();
}

void Camera::UpdateViewMatrix() //Updates view matrix and also updates the movement vectors
{
	//Calculate camera rotation matrix
	XMMATRIX camRotMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);

	XMVECTOR camTarget = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, camRotMatrix);

	camTarget += posVector;

	XMVECTOR upDirection = XMVector3TransformCoord(DEFAULT_UP_VECTOR, camRotMatrix);

	viewMatrix = XMMatrixLookAtLH(posVector, camTarget, upDirection);

	XMMATRIX yawMatrix = XMMatrixRotationRollPitchYaw(0.0f, rot.y, 0.0f);
	vec_forward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, yawMatrix);
	vec_backward = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, yawMatrix);
	vec_left = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, yawMatrix);
	vec_right = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, yawMatrix);
}