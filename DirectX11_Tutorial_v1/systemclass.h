//////////////////////////////////////////////////////////////////////////
// Filename: systemclass.h
// Author: Jeremy J. Hartmann
//////////////////////////////////////////////////////////////////////////
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

//////////////////////////////////////////////////////////////////////////
// PRE-PROCESS: reduces size of header files.
//////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN

//////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////
#include <windows.h>

// CLASES
#include "graphicsclass.h"
#include "inputclass.h"


//////////////////////////////////////////////////////////////////////////
// Class name: SystemClass
//////////////////////////////////////////////////////////////////////////
#pragma once
class SystemClass
{
public:
    SystemClass();
    SystemClass(const SystemClass& other);
    ~SystemClass();

    bool Initialize();
    void Shutdown();
    void Run();


    LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);


private:
    bool Frame();
    void InitializeWindows(int&, int&);
    void ShutdownWindows();

private:
    LPCWSTR m_applicationName;
    HINSTANCE m_hinstance;
    HWND m_hwnd;

    InputClass *m_Input;
    GraphicsClass *m_Graphics;
     
};

//////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////
static LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

//////////////////////////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////////////////////////
static SystemClass *ApplicationHandle = 0;


#endif // _SYSTEMCLASS_H_

