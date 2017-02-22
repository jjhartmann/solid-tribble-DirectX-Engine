#include "systemclass.h"



SystemClass::SystemClass()
{
}


SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
    return false;
}

void SystemClass::Shutdown()
{

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
