#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Game {
public:
	Game() {
	}

	~Game() {
		glfwTerminate();
	}

	int Init() {
		if (!glfwInit()) {
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, m_windowTitle, NULL, NULL);
		if (!m_window)
		{
			std::cout << "Failed to create window: " << m_windowTitle << std::endl;
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(m_window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}

		glViewport(0, 0, m_windowWidth, m_windowHeight);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	}

	void processInput()
	{
		if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(m_window, true);
	}

	void OnUpdate() {

	}

	void Run() {
		while (!glfwWindowShouldClose(m_window)) {
			processInput();
			
			glfwPollEvents();

			glClear(GL_COLOR_BUFFER_BIT);

			OnUpdate();

			glfwSwapBuffers(m_window);
		}
	}

private:
	GLFWwindow* m_window = nullptr;
	const unsigned int m_windowWidth = 1280;
	const unsigned int m_windowHeight = 720;
	const char* m_windowTitle = "GameN";
};

int main(int argc, char** argv) {
	Game game;

	game.Init();
	game.Run();

	return 0;
}
