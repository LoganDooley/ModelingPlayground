#include "GlfwWindow.h"

#include "glad/glad.h"

GlfwWindow::GlfwWindow() :
	m_window(nullptr),
	m_inputManager(std::make_shared<InputManager>())
{
}

GlfwWindow::~GlfwWindow()
{
}

int GlfwWindow::Initialize()
{
	if (!glfwInit())
	{
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	m_window = glfwCreateWindow(1280, 720, "Modeling Playground", nullptr, nullptr);
	if (m_window == nullptr)
	{
		return -1;
	}
	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return -1;
	}

	m_inputManager->Initialize(m_window);

	return 0;
}

bool GlfwWindow::ShouldClose() const
{
	return glfwWindowShouldClose(m_window);
}

void GlfwWindow::PollEvents()
{
	glfwPollEvents();
}

void GlfwWindow::SwapBuffers() const
{
	glfwSwapBuffers(m_window);
}

void GlfwWindow::Close()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

GLFWwindow* GlfwWindow::GetWindowPointer() const
{
	return m_window;
}

const std::shared_ptr<InputManager>& GlfwWindow::GetInputManager() const
{
	return m_inputManager;
}
