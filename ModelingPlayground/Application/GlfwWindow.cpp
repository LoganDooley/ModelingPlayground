#include "GlfwWindow.h"
#include "glad/glad.h"

GlfwWindow::GlfwWindow() :
	m_window(nullptr)
{
}

GlfwWindow::~GlfwWindow()
{
}

int GlfwWindow::Initialize()
{
	if (!glfwInit()) {
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	m_window = glfwCreateWindow(1280, 720, "Modeling Playground", nullptr, nullptr);
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

bool GlfwWindow::ShouldClose() {
	return glfwWindowShouldClose(m_window);
}

void GlfwWindow::PollEvents()
{
	glfwPollEvents();
}

void GlfwWindow::SwapBuffers() {
	glfwSwapBuffers(m_window);
}

void GlfwWindow::Close()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

GLFWwindow* GlfwWindow::GetWindowPointer() {
	return m_window;
}
