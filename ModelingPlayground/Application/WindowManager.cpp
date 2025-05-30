#include "WindowManager.h"
#include <filesystem>
#include <iostream>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "glad/glad.h"

#include "../../../ModelingEngine/ModelingEngine/ModelingEngine/Libraries/tinyfiledialogs/tinyfiledialogs.h"
#include "../Utils/SceneLoader.h"
#include "Rendering/RenderingManager.h"
#include "Window/HierarchyWindow.h"
#include "Window/InspectorWindow.h"
#include "Window/RenderingSettingsWindow.h"
#include "Window/SceneViewWindow.h"
#include "Window/Window.h"

WindowManager::WindowManager()
{
}

WindowManager::~WindowManager()
{
}

void WindowManager::Initialize(const std::unique_ptr<GlfwWindow>& glfwWindow,
                               std::shared_ptr<SceneHierarchy> sceneHierarchy,
                               std::shared_ptr<RenderingManager> renderingManager)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
    io.ConfigDockingWithShift = false; // Dock just by grabbing window title
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(glfwWindow->GetWindowPointer(), true);
    ImGui_ImplOpenGL3_Init();

    m_windows.push_back(std::make_shared<HierarchyWindow>(sceneHierarchy, renderingManager));
    m_windows.push_back(std::make_shared<SceneViewWindow>(renderingManager, glfwWindow->GetInputManager()));
    m_windows.push_back(std::make_shared<InspectorWindow>(sceneHierarchy));
    m_windows.push_back(std::make_shared<RenderingSettingsWindow>(renderingManager));

    m_sceneHierarchy = sceneHierarchy;
    m_renderingManager = renderingManager;
}

void WindowManager::Update(double seconds) const
{
    for (const auto& window : m_windows)
    {
        window->Update(seconds);
    }
}

void WindowManager::Render(const std::unique_ptr<GlfwWindow>& glfwWindow) const
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
            if (firstTime)
            {
                firstTime = false;
                // Clear out existing layout
                ImGui::DockBuilderRemoveNode(rootDockSpace);
                // Add empty node
                ImGui::DockBuilderAddNode(rootDockSpace, dockspaceFlags | ImGuiDockNodeFlags_DockSpace);
                // Main node should cover entire window
                ImGui::DockBuilderSetNodeSize(rootDockSpace, ImGui::GetWindowSize());
                // Create a dock node for the middle docked window
                ImGuiID middleDockSpace = ImGui::DockBuilderSplitNode(rootDockSpace, ImGuiDir_Right, 0.75f, nullptr,
                                                                      &rootDockSpace);
                // Create a dock node for the right docked window
                ImGuiID rightDockSpace = ImGui::DockBuilderSplitNode(middleDockSpace, ImGuiDir_Right, 0.33f, nullptr,
                                                                     &middleDockSpace);

                // Create a dock node for right bottom
                ImGuiID rightBottomDockSpace = ImGui::DockBuilderSplitNode(
                    rightDockSpace, ImGuiDir_Down, 0.5f, nullptr, &rightDockSpace);

                ImGui::DockBuilderDockWindow(HierarchyWindow::Name.c_str(), rootDockSpace);
                ImGui::DockBuilderDockWindow(SceneViewWindow::Name.c_str(), middleDockSpace);
                ImGui::DockBuilderDockWindow(InspectorWindow::Name.c_str(), rightDockSpace);
                ImGui::DockBuilderDockWindow(RenderingSettingsWindow::Name.c_str(), rightBottomDockSpace);
                ImGui::DockBuilderFinish(rootDockSpace);
            }
        }

        // Menu bar
        {
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    const char* lFilterPatterns[1] = {"*.json"};
                    std::string existingFilePath = m_sceneHierarchy->GetFilePath();
                    if (ImGui::MenuItem("New"))
                    {
                        SceneLoader::LoadScene(m_sceneHierarchy, m_renderingManager);
                    }
                    if (ImGui::MenuItem("Open"))
                    {
                        const char* filePath = tinyfd_openFileDialog(
                            "Select a scene to open",
                            "",
                            1,
                            lFilterPatterns,
                            "json files",
                            0
                        );
                        SceneLoader::LoadScene(m_sceneHierarchy, m_renderingManager, filePath);
                    }
                    if (ImGui::MenuItem("Open External Scene"))
                    {
                        const char* externalSceneFilterPatterns[8] = {
                            "*.gltf", "*.fbx", "*.3ds", "*.ase", "*.ply", "*.obj", "*.dxf", "*.x"
                        };
                        const char* filePath = tinyfd_openFileDialog(
                            "Select a scene to open",
                            "",
                            8,
                            externalSceneFilterPatterns,
                            "files",
                            0
                        );
                        SceneLoader::LoadExternalScene(m_sceneHierarchy, m_renderingManager,
                                                       filePath);
                    }
                    if (ImGui::MenuItem("Save", nullptr, false, !existingFilePath.empty()))
                    {
                        SceneLoader::SaveScene(m_sceneHierarchy, m_renderingManager, existingFilePath.c_str());
                    }
                    if (ImGui::MenuItem("Save as..."))
                    {
                        const char* filePath = tinyfd_saveFileDialog(
                            "Save scene as",
                            "",
                            1,
                            lFilterPatterns,
                            "json files"
                        );
                        SceneLoader::SaveScene(m_sceneHierarchy, m_renderingManager, filePath);
                    }
                    ImGui::Separator();
                    if (ImGui::MenuItem("Import Texture"))
                    {
                        const char* textureFilterPatterns[2] = {"*.png", "*.jpg"};
                        const char* filePath = tinyfd_openFileDialog(
                            "Choose texture to import",
                            "",
                            2,
                            textureFilterPatterns,
                            "image files",
                            0
                        );
                        m_renderingManager->AddTexture(filePath);
                    }
                    if (ImGui::MenuItem("Import Mesh"))
                    {
                        const char* meshFilterPatterns[2] = {"*.obj"};
                        const char* filePath = tinyfd_openFileDialog(
                            "Choose mesh to import",
                            "",
                            1,
                            meshFilterPatterns,
                            "obj files",
                            0
                        );
                        m_renderingManager->AddPrimitive(filePath);
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
        }

        ImGui::End();
    }

    for (auto window : m_windows)
    {
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
