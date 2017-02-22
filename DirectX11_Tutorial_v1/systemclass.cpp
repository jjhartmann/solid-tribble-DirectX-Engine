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
    m_Input->Initialize();

    // Create the graphics object. Handle rendering of all graphics. 
    m_Graphics = new GraphicsClass();
    if (!m_Graphics)
    {
        return false;
    }


    // Initialize the graphics object
    result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
    if (!result) {
        return false;
    }
    

    return true;
}

void SystemClass::Shutdown()
{
    // Release the graphics object
    if (m_Graphics) {
        m_Graphics->Shutdown();
        delete m_Graphics;
        m_Graphics = 0;
    }

    // Release input object
    if (m_Input) {
        delete m_Input;
        m_Input = NULL;
    }

    // Shutdown the window
    ShutdownWindows();
}

void SystemClass::Run()
{
    MSG msg;
    bool done = false, result = false;

    // Initialize the message structure
    ZeroMemory(&msg, sizeof(MSG));

    // Loop until quite message received
    while (!done) {
        // Handle the window message
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Exit if windows signals end. 
        if (msg.message == WM_QUIT) {
            done = true;
        }
        else {
            // Process frame
            result = Frame();
            if (!result) {
                done = true;
            }
        }
    }
}

bool SystemClass::Frame()
{
    bool result = false;

    // Check if user has pressed escape
    if (m_Input->IsKeyDown(VK_ESCAPE)) {
        return false;
    }

    // Process the frame for the graphics object
    result = m_Graphics->Frame();
    if (!result) {
        return false;
    }

    return true;
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
    WNDCLASSEX wc;
    DEVMODE dmScreenSettings;
    int posX = 0, posY = 0;

    // Get external pointer
    ApplicationHandle = this;

    // Get instance of the application
    m_hinstance = GetModuleHandle(NULL);

    // Application name (wide char string)
    m_applicationName = L"Engine";

    // Setup window class
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_hinstance;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = m_applicationName;
    wc.lpszMenuName = NULL;
    wc.cbSize = sizeof(WNDCLASSEX);

    // Register window class
    RegisterClassEx(&wc);

    // Determine the resolution of the clients screen
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Handle full screen mode
    if (FULL_SCREEN) {
        // Set menu to maximum size 
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
        dmScreenSettings.dmScale = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
        dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
        dmScreenSettings.dmBitsPerPel = 32;
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // Change display settings
        ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

        // Set position of window 
        posX = posY = 0;
    }
    else {
        // Set to default screen size of 600x800
        screenWidth = 800;
        screenHeight = 600;

        // Place position
        posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
        posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
    }

    // Create the window 
    m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, 
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, screenWidth, screenHeight, 
        NULL, NULL, m_hinstance, NULL);

    // Set window as main focus
    ShowWindow(m_hwnd, SW_SHOW);
    SetForegroundWindow(m_hwnd);
    SetFocus(m_hwnd);

    // Hide mouse cursor
    ShowCursor(false);

}

void SystemClass::ShutdownWindows()
{
    // Display mouse cursor
    ShowCursor(true);

    // Fix display setting when leaving full screen 
    if (FULL_SCREEN) {
        ChangeDisplaySettings(NULL, 0);
    }

    // Remove window
    DestroyWindow(m_hwnd);
    m_hwnd = 0;

    // Remove application instance
    UnregisterClass(m_applicationName, m_hinstance);
    m_hinstance = NULL;

    // release pointer to class
    ApplicationHandle = NULL;

}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
    switch (umsg)
    {
        // Check Keydown 
        case WM_KEYDOWN:
        {
            // Record state in input object
            m_Input->KeyDown((unsigned int)wparam);
            return 0;
        }

        // Check keyup 
        case WM_KEYUP:
        {
            m_Input->KeyUp((unsigned int)wparam);
            return 0;
        }

        // All other messages send back to message handler
        default:
        {
            return DefWindowProc(hwnd, umsg, wparam, lparam);
        }
    }
}


//////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPE
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
    switch (umsg) 
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
        case WM_CLOSE:
        {
            PostQuitMessage(0);
            return 0;
        }
        default:
        {
            return ApplicationHandle->MessageHandler(hwnd, umsg, wparam, lparam);
        }
    }
}
