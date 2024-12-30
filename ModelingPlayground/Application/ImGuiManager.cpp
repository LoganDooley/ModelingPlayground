#include "ImGuiManager.h"
#include "imgui.h"
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
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window->GetWindowPointer(), true);
	ImGui_ImplOpenGL3_Init();
}

void ImGuiManager::Render()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Menu bar
	{
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiManager::Close()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
