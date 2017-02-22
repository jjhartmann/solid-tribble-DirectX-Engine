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

private:
    bool Render();
};

#endif // !_GRAPHICSCLASS_H_
