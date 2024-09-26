#pragma once

#include "Core.h"
#include "Window.h"

#include "Mesh.h"
#include "Shader.h"


class Application
{
public:
	Application();
	~Application();

	void Run();

private:
	void OnInit();
	void OnUpdate(float frameTime); // seconds

	void OnWindowResize(uint32_t width, uint32_t height);

private:
	Window* m_Window = nullptr;
	bool m_Running = false;

	Mesh* m_Mesh;
	Shader* m_Shader;
	glm::mat4 m_MeshTransform;

	const glm::vec4 m_ClearColor = { 1.0, 0.95, 0.24, 1.0 };
	const glm::vec4 m_MeshColor  = { 0.00, 0.60, 0.98, 1 };
	const glm::vec2 m_Direction  = { 1, 0 };
	const float m_ObjectSpeed = 0.05f;
	const float m_CameraSpeed = 1.5f;
};

