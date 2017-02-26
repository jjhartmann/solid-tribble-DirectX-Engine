//////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
// Function: Setup view matrix 
//////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"



CameraClass::CameraClass() :
    m_positionX(0.0f),
    m_positionY(0.0f),
    m_positionZ(0.0f),
    m_rotationX(0.0f),
    m_rotationY(0.0f),
    m_rotationZ(0.0f)
{
}

CameraClass::CameraClass(const CameraClass& other)
{
    ; // Empty
}


CameraClass::~CameraClass()
{
    ; // Empty
}

void CameraClass::SetPosition(float x, float y, float z)
{
    m_positionX = x;
    m_positionY = y;
    m_positionZ = z;
}

void CameraClass::SetRotation(float x, float y, float z)
{
    m_rotationX = x;
    m_rotationY = y;
    m_rotationZ = z;
}

DirectX::XMFLOAT3 CameraClass::GetPosition()
{
    return XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}

DirectX::XMFLOAT3 CameraClass::GetRotation()
{
    return XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}

void CameraClass::Render()
{
    XMFLOAT3 up, position, lookAt;
    XMVECTOR upVector, positionVector, lookAtVector;
    float yaw, pitch, roll;
    XMMATRIX rotationMatrix;

    // Setup up vector. 
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;

    // Load into XMECTOR 
    upVector = XMLoadFloat3(&up);

    // Setup Position of camera
    position.x = m_positionX;
    position.y = m_positionY;
    position.z = m_positionZ;

    // Load into XMMVECTOR 
    positionVector = XMLoadFloat3(&position);

    // Setup where the camera is looking (default)
    lookAt.x = 0.0f;
    lookAt.y = 0.0f;
    lookAt.z = 1.0f;

    // Load into XMVECTOR
    lookAtVector = XMLoadFloat3(&lookAt);

    // Set yaw (y-axis, pitch (x-axis) and roll (z-axis) rotations
    pitch = m_rotationX * RAD;
    yaw = m_rotationY * RAD;
    roll = m_rotationZ * RAD;

    // Create rotiation matrix from yaw, pitch and roll
    rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

    // Transform the lookAt and up vector by the rotation matrix so view is correctly rotated at origin
    lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
    upVector = XMVector3TransformCoord(upVector, rotationMatrix);

    // Create view matrix from the three updated vectors
    m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

void CameraClass::GetViewMatrix(XMMATRIX& matrix)
{
    matrix = m_viewMatrix;
}
