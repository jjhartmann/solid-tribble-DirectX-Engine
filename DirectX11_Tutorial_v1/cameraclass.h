//////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
// Function: Setup view matrix 
//////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

// INCLUDE
#include <DirectXMath.h>
using namespace DirectX;

// CLASS
class CameraClass
{
public:
    CameraClass();
    CameraClass(const CameraClass& other);
    ~CameraClass();

    void SetPosition(float x, float y, float z);
    void SetRotation(float x, float y, float z);

    XMFLOAT3 GetPosition();
    XMFLOAT3 GetRotation();

    void Render();
    void GetViewMatrix(XMMATRIX& matrix);

private:
    float m_positionX, m_positionY, m_positionZ;
    float m_rotationX, m_rotationY, m_rotationZ;
    XMMATRIX m_viewMatrix;

    float RAD = 0.0174532925f;

};

#endif
