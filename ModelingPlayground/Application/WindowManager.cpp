#include "WindowManager.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "glad/glad.h"
#include <iostream>
#include <GLFW/glfw3.h>

#include "Window/HierarchyWindow.h"
#include "Window/SceneViewWindow.h"
#include "Window/InspectorWindow.h"
#include "Window/Window.h"

WindowManager::WindowManager(const std::shared_ptr<Scene>& scene):
	m_scene(scene)
{
}

WindowManager::~WindowManager()
{
}

void WindowManager::Initialize(std::unique_ptr<GlfwWindow>& glfwWindow)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigDockingWithShift = false;                        // Dock just by grabbing window title
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL(glfwWindow->GetWindowPointer(), true);
	ImGui_ImplOpenGL3_Init();

	m_windows.push_back(std::make_shared<HierarchyWindow>(m_scene));
	m_windows.push_back(std::make_shared<SceneViewWindow>(m_scene, glfwWindow->GetInputManager()));
	m_windows.push_back(std::make_shared<InspectorWindow>(m_scene));
}

void WindowManager::Update(double seconds) const
{
	for (const auto& window : m_windows)
	{
		window->Update(seconds);
	}
}

void WindowManager::Render(std::unique_ptr<GlfwWindow>& glfwWindow)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Main Window
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		ImGuiWindowFlags mainWindowFlags = ImGuiWindowFlags_NoDocking;
		mainWindowFlags |= ImGuiWindowFlags_NoBackground;
		mainWindowFlags |= ImGuiWindowFlags_MenuBar;
		mainWindowFlags |= ImGuiWindowFlags_NoResize;
		mainWindowFlags |= ImGuiWindowFlags_NoMove;
		mainWindowFlags |= ImGuiWindowFlags_NoCollapse;
		mainWindowFlags |= ImGuiWindowFlags_NoTitleBar;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.0f, 0.0f));
		ImGui::Begin("Root", nullptr, mainWindowFlags);
		ImGui::PopStyleVar();
		ImGui::PopStyleVar(2);

		// Dockspace
		{
			ImGuiID rootDockSpace = ImGui::GetID("Root");
			ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;
			ImGui::DockSpace(rootDockSpace, ImVec2(0.0f, 0.0f), dockspaceFlags);
			
			static bool firstTime = true;
			if (firstTime) {
				firstTime = false;
				// Clear out existing layout
				ImGui::DockBuilderRemoveNode(rootDockSpace);
				// Add empty node
				ImGui::DockBuilderAddNode(rootDockSpace, dockspaceFlags | ImGuiDockNodeFlags_DockSpace);
				// Main node should cover entire window
				ImGui::DockBuilderSetNodeSize(rootDockSpace, ImGui::GetWindowSize());
				// Create a dock node for the middle docked window
				ImGuiID middleDockSpace = ImGui::DockBuilderSplitNode(rootDockSpace, ImGuiDir_Right, 0.75f, nullptr, &rootDockSpace);
				// Create a dock node for the right docked window
				ImGuiID rightDockSpace = ImGui::DockBuilderSplitNode(middleDockSpace, ImGuiDir_Right, 0.33f, nullptr, &middleDockSpace);

				ImGui::DockBuilderDockWindow(HierarchyWindow::Name.c_str(), rootDockSpace);
				ImGui::DockBuilderDockWindow(SceneViewWindow::Name.c_str(), middleDockSpace);
				ImGui::DockBuilderDockWindow(InspectorWindow::Name.c_str(), rightDockSpace);
				ImGui::DockBuilderFinish(rootDockSpace);
			}
		}

		// Menu bar
		{
			if (ImGui::BeginMenuBar()) {
				if (ImGui::BeginMenu("File")) {
					if (ImGui::MenuItem("Open")) {

					}
					if (ImGui::MenuItem("Save")) {

					}
					if (ImGui::MenuItem("Save as...")) {

					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
		}

		ImGui::End();
	}

	for (auto window : m_windows) {
		window->Render();
	}

	// Render widgets
	ImGui::Render();
	int displayW, displayH;
	glfwGetFramebufferSize(glfwWindow->GetWindowPointer(), &displayW, &displayH);
	glViewport(0, 0, displayW, displayH);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
	GLFWwindow* backup_current_context = glfwGetCurrentContext();
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	glfwMakeContextCurrent(backup_current_context);
}

void WindowManager::Close()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
