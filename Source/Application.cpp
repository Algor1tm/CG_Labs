#include "Application.h"

#include <chrono>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>


Application::Application()
{
	m_Window = new Window("Lab1", 800, 800);
	m_Window->SetWindowResizeCallback([this](uint32_t width, uint32_t height) { OnWindowResize(width, height); });

	OnInit();
}

Application::~Application()
{
	delete m_Mesh;
	delete m_Shader;

	delete m_Window;
}

void Application::OnInit()
{
	Vertex vertices[] = {
		// 2 big triangles in middle
		{ { -0.1f, -0.1f } },
		{ {  0.2f, -0.1f } },
		{ { -0.1f,  0.2f } },
		{ {  0.2f, -0.4f } },

		// 2 bottom triangles
		{ { -0.1f, -0.4f } },
		{ {  0.05f, -0.25f } },
		{ { -0.1f, -0.25f } },

		// front small triangle
		{ { -0.1f, -0.2f } },
		{ { -0.1f, -0.0f } },
		{ { -0.2f, -0.1f } },

		// Head square
		{ { -0.1f,  0.4f } },
		{ { -0.3f,  0.4f } },
		{ { -0.3f,  0.2f } },

		// Ears quad
		{ {  0.1f,  0.4f } },
		{ {  0.2f,   0.50f } },
		{ {  0.0f,   0.50f } },
	};

	uint32_t indices[] = { 
		// 2 big triangles in middle
		0, 1, 2, 0, 3, 1,

		// 2 bottom triangles
		4, 5, 3, 4, 5, 6,

		// front small triangle
		7, 8, 9,

		// Head square
		12, 2, 10, 10, 11, 12,

		// Ears quad
		10, 13, 14, 14, 15, 10
	};

	m_Mesh = new Mesh(vertices, std::size(vertices), indices, std::size(indices));
	m_Shader = new Shader("Shaders/FlatColor.vert", "Shaders/FlatColor.frag");

	m_MeshTransform = glm::mat4(1);

	glLineWidth(2.5f);
}

void Application::OnUpdate(float frameTime)
{
	// Update
	{
		glm::vec2 objectOffset = m_Direction * m_ObjectSpeed * frameTime;

		m_MeshTransform = glm::translate(m_MeshTransform, glm::vec3(objectOffset, 0));

		glm::vec2 cameraDir = glm::vec2(0);

		if (m_Window->GetKey(GLFW_KEY_A))
			cameraDir += glm::vec2(-1, 0);

		if (m_Window->GetKey(GLFW_KEY_S))
			cameraDir += glm::vec2(0, -1);

		if (m_Window->GetKey(GLFW_KEY_D))
			cameraDir += glm::vec2(1, 0);

		if (m_Window->GetKey(GLFW_KEY_W))
			cameraDir += glm::vec2(0, 1);

		if (cameraDir != glm::vec2(0))
		{
			cameraDir = glm::normalize(cameraDir);

			glm::vec2 cameraOffset = cameraDir * m_CameraSpeed * frameTime;

			glm::mat4 cameraTransform = glm::translate(glm::mat4(1), glm::vec3(cameraOffset, 0));
			glm::mat4 cameraInvTransform = glm::inverse(cameraTransform);

			m_MeshTransform = cameraInvTransform * m_MeshTransform;
		}
	}

	// Render
	{
		glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
		glClear(GL_COLOR_BUFFER_BIT);

		m_Shader->Bind();
		m_Mesh->Bind();

		m_Shader->SetMat4("u_Transform", m_MeshTransform);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		m_Shader->SetVector4("u_Color", m_MeshColor);
		glDrawElements(GL_TRIANGLES, m_Mesh->GetIndicesCount(), GL_UNSIGNED_INT, nullptr);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		m_Shader->SetVector4("u_Color", glm::vec4(0, 0, 0, 1));
		glDrawElements(GL_TRIANGLES, m_Mesh->GetIndicesCount(), GL_UNSIGNED_INT, nullptr);
	}
}

void Application::Run()
{
	using namespace std::chrono;

	m_Running = true;

	system_clock::time_point start, end;
	end = start = system_clock::now();

	while (m_Running)
	{
		float frameTime = duration_cast<duration<double, std::milli>>(end - start).count() * 0.001;

		OnUpdate(frameTime);

		m_Window->OnUpdate();

		if (m_Window->ShouldClose())
			m_Running = false;

		start = end;
		end = system_clock::now();
	}
}

void Application::OnWindowResize(uint32_t width, uint32_t height)
{
	glViewport(0, 0, width, height);
}
