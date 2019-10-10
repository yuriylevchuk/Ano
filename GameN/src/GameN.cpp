#include <Ano.h>

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main(int argc, char** argv) {
	Ano::Init();

	const unsigned int windowWidth = 1024;
	const unsigned int windowHeight = 768;
	const char *windowTitle = "GameN";

	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
