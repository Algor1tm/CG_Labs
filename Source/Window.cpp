#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


static void GLFWErrorCallback(int error, const char* description)
{
	LOG_ERROR("GLFW ({}) : {}", error, description);
}


Window::Window(const std::string& title, uint32_t width, uint32_t height)
{
	int success = glfwInit();
	ASSERT(success != 0, "Failed to initialize GLFW!");

	if (!success)
	{
		m_WindowHandle = nullptr;
		return;
	}

	glfwSetErrorCallback(GLFWErrorCallback);

	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	m_WindowHandle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

	m_WindowData.Width = width;
	m_WindowData.Height = height;
	glfwSetWindowUserPointer(m_WindowHandle, &m_WindowData);

	glfwSetWindowSizeCallback(m_WindowHandle, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

			data.Width = width;
			data.Height = height;

			if(data.ResizeCallback)
				data.ResizeCallback(data.Width, data.Height);	
		});

	LOG_INFO("Create GLFW window '{}', (width = {}, height = {})", title, width, height);


#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
#endif

	glfwMakeContextCurrent(m_WindowHandle);
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	ASSERT(status != 0, "Failed to initialize glad!");

	// Enable VSync
	glfwSwapInterval(1);

	LOG_INFO("Create OpenGL Graphics Context:");
	LOG_INFO("	OpenGL version: {0}", (const char*)glGetString(GL_VERSION));
	LOG_INFO("	Graphics Card: {0}", (const char*)glGetString(GL_RENDERER));
	LOG_INFO("	Vendor: {0}", (const char*)glGetString(GL_VENDOR));

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
}

Window::~Window()
{
	glfwDestroyWindow(m_WindowHandle);
	glfwTerminate();

	LOG_INFO("Terminate GLFW");
}

void Window::OnUpdate()
{
	glfwPollEvents();
	glfwSwapBuffers(m_WindowHandle);
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(m_WindowHandle);
}

void Window::SetWindowResizeCallback(const ResizeCallbackFn& callback)
{
	m_WindowData.ResizeCallback = callback;
	m_WindowData.ResizeCallback(GetWidth(), GetHeight());
}

int Window::GetKey(int keycode)
{
	return glfwGetKey(m_WindowHandle, keycode);
}
