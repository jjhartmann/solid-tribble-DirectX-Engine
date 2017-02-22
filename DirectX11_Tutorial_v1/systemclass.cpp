//////////////////////////////////////////////////////////////////////////
// Filename: systemclass.cpp
// Author: Jeremy J. Hartmann
//////////////////////////////////////////////////////////////////////////
#include "systemclass.h"



SystemClass::SystemClass() :
    m_Input(NULL),
    m_Graphics(NULL)
{
}

SystemClass::SystemClass(const SystemClass&)
{}

SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
    int screenHeight = 0, screenWidth = 0;
    bool result = false;
    
    
    // Initialize the windows API
    InitializeWindows(screenWidth, screenHeight);

    // Create the input object. Keyboard input form user. 
    m_Input = new InputClass();
    if (!m_Input)
    {
        return false;
    }

    // Initialize the input object
    //m_Input->Initialize()

    // Create the graphics object. Handle rendering of all graphics. 
    m_Graphics = new GraphicsClass();
    if (!m_Graphics)
    {
        return false;
    }


    // Initialize the graphics object
    //result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
    if (result) {
        return false;
    }
    

    return true;
}

void SystemClass::Shutdown()
{
    // Release the graphics object
    if (m_Graphics) {
        //m_Graphics->Shutdown();
        delete m_Graphics;
        m_Graphics = 0;
    }
}

void SystemClass::Run()
{

}

bool SystemClass::Frame()
{
    return false;
}

void SystemClass::InitializeWindows(int&, int&)
{

}

void SystemClass::ShutdownWindows()
{

}

LRESULT CALLBACK SystemClass::MessageHandler(HWND, UINT, WPARAM, LPARAM)
{
    return 0;
}
