#include "Window.h"

Window::Window(const char* name)
	:m_Name(name), 
	m_Window(nullptr)
{

	if (InitGlfw() == -1)
	{
		std::cout << "ERROR Initilization of GLFW failed" << std::endl;
		return;
	}

	monitor = glfwGetPrimaryMonitor();
	mode = glfwGetVideoMode(monitor);

	if (WindowSetup() == -1)
	{
		std::cout << "Window Setup Failed!" << std::endl;
		return;
	}

	//Enabling depth testing (z-buffer)
	glEnable(GL_DEPTH_TEST);

	//Window Viewport
	glViewport(0, 0, mode->width, mode->height);
}

Window::~Window()
{
	std::cout << "Window has been terminated! " << std::endl;
	glfwTerminate();
}

GLFWwindow* Window::GetWindow()
{
	return m_Window;
}

int Window::WindowSetup()
{
	WindowHints();

	if (CreateWindow() == -1)
	{
		std::cout << "Failed to create a window" << std::endl;
		return -1;
	}

	if (InitGLAD() == -1)
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
}

int Window::InitGlfw()
{
	//Initialization of GLFW
	if (!glfwInit())
		return -1;
}

void Window::WindowHints()
{
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

int Window::CreateWindow()
{
	//m_Window = glfwCreateWindow(mode->width, mode->height, m_Name, monitor, NULL); //FOR FULLSCREEN
	m_Window = glfwCreateWindow(1000, 800, m_Name, NULL, NULL); //FOR DEV
	if (!m_Window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(m_Window);
}

int Window::InitGLAD()
{
	//Initialization of GLAD
	gladLoadGL();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return -1;
	}
}
