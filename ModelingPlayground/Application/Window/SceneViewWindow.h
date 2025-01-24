#pragma once

#include "Window.h"
#include "glad/glad.h"

class SceneViewWindow : public Window
{
public:
	SceneViewWindow();
	~SceneViewWindow();

	void Render() override;

	inline static const std::string Name = "Scene View";

private:
	void InitializeOpenGLObjects();

	GLuint m_triangleVBO;
	GLuint m_triangleVAO;

	GLuint m_framebuffer;
	GLuint m_framebufferTexture;
	GLuint m_framebufferRenderbuffer;
	
	GLuint m_defaultShader;
};

