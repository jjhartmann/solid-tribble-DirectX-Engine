//////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
// Class: GraphicsClass
// Description: Contains logic for the rendering of the window and scene. 
// Author: Jeremy J. Hartmann
//////////////////////////////////////////////////////////////////////////

#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

// INCLUDE
#include <windows.h>
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "colorshaderclass.h"



// GLOBALS
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
public:
    GraphicsClass();
    GraphicsClass(const GraphicsClass& other);
    ~GraphicsClass();

    bool Initialize(int screenWidth, int screenHeight, HWND in_hwnd);
    void Shutdown();
    bool Frame();
    void MoveUp(float delta);

private:
    bool Render();

private:
    D3DClass *m_Direct3D;
    CameraClass *m_Camera;
    ModelClass *m_Model;
    ColorShaderClass *m_ColorShader;
};

#endif // !_GRAPHICSCLASS_H_
