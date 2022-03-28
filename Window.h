#pragma once
#include <iostream>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	 Window(const char* name);
	~Window();

	GLFWwindow* GetWindow();

private:
	int WindowSetup();
	int InitGlfw();
	void WindowHints();
	int CreateWindow();
	int InitGLAD();

private:
	const char* m_Name;
	GLFWwindow* m_Window;
	GLFWmonitor* monitor;
	const GLFWvidmode* mode;
};

