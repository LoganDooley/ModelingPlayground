#include "InputManager.h"

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::Initialize(GLFWwindow* window)
{
    glfwSetWindowUserPointer(window, this);
    
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
}

void InputManager::SubscribeToKeyEvents(std::function<void(int, int)> callback)
{
    m_keyEventSubscribers.push_back(callback);
}

void InputManager::SubscribeToCursorPosEvents(std::function<void(double, double)> callback)
{
    m_cursorPosEventSubscribers.push_back(callback);
}

void InputManager::SubscribeToMouseButtonEvents(std::function<void(int, int)> callback)
{
    m_mouseButtonEventSubscribers.push_back(callback);
}

void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    for (const auto& subscriber : inputManager->m_keyEventSubscribers)
    {
        subscriber(key, action);
    }
}

void InputManager::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    for (const auto& subscriber : inputManager->m_cursorPosEventSubscribers)
    {
        subscriber(xpos, ypos);
    }
}

void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    for (const auto& subscriber : inputManager->m_mouseButtonEventSubscribers)
    {
        subscriber(button, action);
    }
}
