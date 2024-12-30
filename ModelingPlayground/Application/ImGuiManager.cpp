#include "ImGuiManager.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "glad/glad.h"
#include <iostream>

ImGuiManager::ImGuiManager()
{
}

ImGuiManager::~ImGuiManager()
{
}

void ImGuiManager::Initialize(std::unique_ptr<Window>& window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigDockingWithShift = false;                        // Dock just by grabbing window title
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL(window->GetWindowPointer(), true);
	ImGui_ImplOpenGL3_Init();
}

void ImGuiManager::Render(std::unique_ptr<Window>& window)
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
			ImGuiID dockspace_id = ImGui::GetID("Root");
			ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			
			static bool firstTime = true;
			if (firstTime) {
				firstTime = false;
				// Clear out existing layout
				ImGui::DockBuilderRemoveNode(dockspace_id);
				// Add empty node
				ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
				// Main node should cover entire window
				ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetWindowSize());
				// get id of main dock space area
				ImGuiID dockspace_main_id = dockspace_id;
				// Create a dock node for the right docked window
				ImGuiID right = ImGui::DockBuilderSplitNode(dockspace_main_id, ImGuiDir_Right, 0.25f, nullptr, &dockspace_main_id);

				ImGui::DockBuilderDockWindow("Content", dockspace_main_id);
				ImGui::DockBuilderDockWindow("Sidebar", right);
				ImGui::DockBuilderFinish(dockspace_id);
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

	// Menu bar
	/*
	{
		ImGui::Begin("Main Window");
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Open")) {
					
				}
				if (ImGui::MenuItem("Save")) {

				}
				if (ImGui::MenuItem("Save as...")) {

				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		ImGui::End();
	}
	*/

	// Test
	{
		ImGui::Begin("Sidebar");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window 1!");
		ImGui::End();
	}

	// Test 2
	{
		ImGui::Begin("Content");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window 2!");
		ImGui::End();
	}

	// Hierarchy
	{

	}

	// Assets
	{

	}

	// Scene Viewer
	{

	}

	// Inspector
	{

	}

	// Render widgets
	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(window->GetWindowPointer(), &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
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

void ImGuiManager::Close()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
