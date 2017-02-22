#include "inputclass.h"



InputClass::InputClass()
{
}

InputClass::InputClass(const InputClass& other)
{

}


InputClass::~InputClass()
{
}

void InputClass::Initialize()
{
    // Initialize all keys to being released (not pressed)
    for (int i = 0; i < 256; i++) {
        m_keys[i] = false;
    }
}

void InputClass::KeyDown(unsigned int input)
{
    // If a key is pressed, svae the state in key array
    m_keys[input] = true;
}

void InputClass::KeyUp(unsigned int input)
{
    // Reset key array when key is released. 
    m_keys[input] = false;
}

bool InputClass::IsKeyDown(unsigned int key)
{
    // Return what state the key is in
    return m_keys[key];
}
