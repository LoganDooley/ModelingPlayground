#include "Window.h"
#include "glad/glad.h"

Window::Window() :
	m_window(nullptr)
{
}

Window::~Window()
{
}

int Window::Initialize()
{
	if (!glfwInit()) {
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	m_window = glfwCreateWindow(1280, 720, "Test", nullptr, nullptr);
	if (m_window == nullptr) {
		return -1;
	}
	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		return -1;
	}
	return 0;
}

bool Window::ShouldClose() {
	return glfwWindowShouldClose(m_window);
}

void Window::PollEvents()
{
	glfwPollEvents();
}

void Window::SwapBuffers() {
	glfwSwapBuffers(m_window);
}

void Window::Close()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

GLFWwindow* Window::GetWindowPointer() {
	return m_window;
}
