#pragma once

#include "Core.h"
#include <functional>


class GLFWwindow;
using ResizeCallbackFn = std::function<void(uint32_t, uint32_t)>;

struct WindowData
{
	uint32_t Width = 1;
	uint32_t Height = 1;
	ResizeCallbackFn ResizeCallback = nullptr;
};

class Window
{
public:
	Window(const std::string& title, uint32_t width, uint32_t height);
	~Window();

	void OnUpdate();

	bool ShouldClose();
	void SetWindowResizeCallback(const ResizeCallbackFn& callback);

	int GetKey(int keycode);

	uint32_t GetWidth() const { return m_WindowData.Width; }
	uint32_t GetHeight() const { return m_WindowData.Height; }

private:
	GLFWwindow* m_WindowHandle;
	WindowData m_WindowData;;
};
