#include "Application.h"

Application::Application(): m_window(nullptr) {

}

Application::~Application() {
	glfwTerminate();
}

void Application::Init(const unsigned int windowWidth, const unsigned int windowHeight, const char* windowTitle) {
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	m_window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
	if (!m_window)
	{
		std::cout << "Failed to create window: " << windowTitle << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(m_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glViewport(0, 0, windowWidth, windowHeight);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	/*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/
}

void Application::OnUpdate () {
}

void Application::Run() {
	while (!glfwWindowShouldClose(m_window)) {
	//processInput();
	
	glfwPollEvents();

	glClear(GL_COLOR_BUFFER_BIT);

	OnUpdate();

	glfwSwapBuffers(m_window);
	}
}