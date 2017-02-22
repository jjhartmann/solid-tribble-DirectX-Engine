//////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
// Class: GraphicsClass
// Description: Contains logic for the rendering of the window and scene. 
// Author: Jeremy J. Hartmann
//////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"



GraphicsClass::GraphicsClass() :
    m_Direct3D(0)
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
    //result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, in_hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
    if (!result) {
        MessageBox(in_hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
        return false;
    }

    return true;
}

void GraphicsClass::Shutdown()
{
    // Release D3DClass object
    if (m_Direct3D) {
        //m_Direct3D->Shutdown();
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
    // Clear the buffers to begin scene
    //m_Direct3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

    // Presen the rendered scene to the screen
    //m_Direct3D->EndScene();

    return true;
}
