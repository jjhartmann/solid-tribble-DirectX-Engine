//////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
// Class: GraphicsClass
// Description: Contains logic for the rendering of the window and scene. 
// Author: Jeremy J. Hartmann
//////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"



GraphicsClass::GraphicsClass() :
    m_Direct3D(NULL), 
    m_Camera(NULL),
    m_Model(NULL),
    m_ColorShader(NULL)
{
    ;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
    ;
}

GraphicsClass::~GraphicsClass()
{
    ;
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND in_hwnd)
{
    bool result = false;

    // Create Direct3D object
    m_Direct3D = new D3DClass();
    if (!m_Direct3D) {
        return false;
    }

    // Initialize Direct3D object
    result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, in_hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
    if (!result) {
        MessageBox(in_hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
        return false;
    }

    // Create camera object
    m_Camera = new CameraClass();
    if (!m_Camera)
        return false;

    // Set initial position of the camera
    m_Camera->SetPosition(0.0, 0.0, -5.0);

    // Create model object
    m_Model = new ModelClass();
    if (!m_Model)
        return false;

    // Initalize model object
    result = m_Model->Initialize(m_Direct3D->GetDevice());
    if (!result) {
        MessageBox(in_hwnd, L"Could not intialize model object", L"Error", MB_OK);
        return false;
    }

    // Create color shader object
    m_ColorShader = new ColorShaderClass();
    if (!m_ColorShader)
        return false;

    // Initialize color shader obect
    result = m_ColorShader->Initialize(m_Direct3D->GetDevice(), in_hwnd);
    if (!result) {
        MessageBox(in_hwnd, L"Could not initialize the color shader object", L"Error", MB_OK);
        return false;
    }

    return true;
}

void GraphicsClass::Shutdown()
{
    // Relase Color shader
    if (m_ColorShader) {
        m_ColorShader->Shutdown();
        delete m_ColorShader;
        m_ColorShader = NULL;
    }

    // Release model object
    if (m_Model) {
        m_Model->Shutdown();
        delete m_Model;
        m_Model = NULL;
    }

    // Release Camera 
    if (m_Camera) {
        delete m_Camera;
        m_Camera = NULL;
    }

    // Release D3DClass object
    if (m_Direct3D) {
        m_Direct3D->Shutdown();
        delete m_Direct3D;
        m_Direct3D = NULL;
    }
}

bool GraphicsClass::Frame()
{
    bool result;

    // Render the graphics scene
    result = Render();
    if (!result) {
        return false;
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////
// PRIVATE
//////////////////////////////////////////////////////////////////////////
bool GraphicsClass::Render()
{
    XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
    bool result = false;

    // Clear the buffers to begin scene (Black)
    m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

    // Generate View matrix based on cameras position
    m_Camera->Render();

    // Get world, view, and projection matrices form camera and d3d object
    m_Direct3D->GetWorldMatrix(worldMatrix);
    m_Camera->GetViewMatrix(viewMatrix);
    m_Direct3D->GetProjectionMatrix(projectionMatrix);

    // Put model vertex and index buffers on graphics pipeline 
    m_Model->Render(m_Direct3D->GetDeviceContext());

    // Render model using color shader
    result = m_ColorShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
    if (!result)
        return false;

    // Presen the rendered scene to the screen
    m_Direct3D->EndScene();

    return true;
}
