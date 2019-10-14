#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>
#include "shader.h"

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

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		return 1;
	}

	void processInput()
	{
		if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(m_window, true);
	}

	void OnUpdate() {
		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, 21, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void Run() {
		GLfloat tile[] = {
			-1.0f, 1.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,
			 0.0f, 1.0f, 0.0f,
			 0.0f, 0.0f, 0.0f,
			 0.0f, -1.0f, 0.0f,
			 1.0f, 0.0f, 0.0f,
			 1.0f, 1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f,
		};

		GLuint tileIndices[] = {
			0, 1, 2,
			4, 5, 6,
			6, 3, 0,
			3, 7, 5,
			2, 4, 5,
			8, 4, 5,
			8, 3, 4

		};

		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ibo);
		glGenVertexArrays(1, &m_vao);

		// Массив буфферов
		glBindVertexArray(m_vao);

		// Буффер вершин
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(tile), tile, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Буффер индексов
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tileIndices), tileIndices, GL_STATIC_DRAW);

		glBindVertexArray(0);

		Shader shader("res/shaders/Base.vert", "res/shaders/Base.frag");
		shader.Bind();

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
	GLuint m_vao, m_vbo, m_ibo;
};

int main(int argc, char** argv) {
	Game game;

	game.Init();

	game.Run();

	return 0;
}
