#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>
#include "shader.h"

class Game {
public:
	Game() : m_ibo(0), m_vao(0), m_vao2(0), m_vbo(0), m_vbo2(0), m_vertexOffsetXLocation(0), m_texture1(0), m_texture2(0) {
	}

	~Game() {
		glfwTerminate();
	}

	void Init() {
		if (!glfwInit()) {
			std::cout << "Failed to initialize GLAD" << std::endl;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, m_windowTitle, NULL, NULL);
		if (!m_window)
		{
			std::cout << "Failed to create window: " << m_windowTitle << std::endl;
			glfwTerminate();
		}

		glfwMakeContextCurrent(m_window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
		}

		glViewport(0, 0, m_windowWidth, m_windowHeight);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		/*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/
	}

	void processInput()
	{
		if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(m_window, true);
	}

	void OnUpdate() {
		m_shader2.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture1);
		glUniform1i(glGetUniformLocation(m_shader2.GetID(), "ourTexture1"), 0);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, m_texture2);
		//glUniform1i(glGetUniformLocation(m_shader2.GetID(), "ourTexture2"), 1);

		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//glUniform1f(m_vertexOffsetXLocation, 0.5f);
		//glBindVertexArray(m_vao2);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindVertexArray(0);
	}

	void Run() {
		GLfloat triangle[] = {
			// positions        // colors
			0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  
		   -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
			0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
		};

		GLfloat tile[] = {
			// positions		//texture coords
			-0.3f, 0.5f, 0.0f,  0.0f, 1.0f,
			-0.3f,-0.5f, 0.0f,	0.0f, 0.0f,
			 0.3f,-0.5f, 0.0f,  1.0f, 0.0f,
			 0.3f, 0.5f, 0.0f,  1.0f, 1.0f
		};

		GLuint tileIndices[] = {
			0, 1, 2,
			2, 3, 0
		};

		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ibo);
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo2);
		glGenVertexArrays(1, &m_vao2);

		// Массив буфферов
		glBindVertexArray(m_vao);

		// Буффер вершин
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(tile), tile, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		// Буффер индексов
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tileIndices), tileIndices, GL_STATIC_DRAW);

		glBindVertexArray(0);

		//------------------------------------------------------

		glBindVertexArray(m_vao2);

		// Буффер вершин
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);

		//---------------------------

	/*	m_shader.Create("res/shaders/Base.vert", "res/shaders/Base.frag");*/
		m_shader2.Create("res/shaders/Base.vert", "res/shaders/Base2.frag");

		/*m_vertexOffsetXLocation = glGetUniformLocation(m_shader.GetID(), "uOffsetX");*/

		int imgWidth, imgHeight, imgChannels;
		//unsigned char* imgData = stbi_load("res/textures/box.png", &imgWidth, &imgHeight, &imgChannels, 0);
		//
		//glGenTextures(1, &m_texture1);
		//glBindTexture(GL_TEXTURE_2D, m_texture1);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//if (imgData) {
		//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
		//	glGenerateMipmap(GL_TEXTURE_2D);
		//} else {
		//	std::cout << "Failed to load texture" << std::endl;
		//}
		//stbi_image_free(imgData);
		//glBindTexture(GL_TEXTURE_2D, 0);
		// --------------------------------------------------------------------------------

		unsigned char*  imgData = stbi_load("res/textures/container.jpg", &imgWidth, &imgHeight, &imgChannels, 0);
		glGenTextures(1, &m_texture1);
		glBindTexture(GL_TEXTURE_2D, m_texture1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (imgData) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
			//glGenerateMipmap(GL_TEXTURE_2D);
		} else {
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(imgData);
		//glBindTexture(GL_TEXTURE_2D, 0);

		while (!glfwWindowShouldClose(m_window)) {
			processInput();
			
			glfwPollEvents();

			glClear(GL_COLOR_BUFFER_BIT);

			OnUpdate();

			glfwSwapBuffers(m_window);
		}

		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao2);
		glDeleteBuffers(1, &m_vbo2);
		glDeleteBuffers(1, &m_ibo);
	}

private:
	GLFWwindow* m_window = nullptr;
	const unsigned int m_windowWidth = 1280;
	const unsigned int m_windowHeight = 720;
	const char* m_windowTitle = "GameN";
	GLuint m_vao, m_vbo, m_vao2, m_vbo2, m_ibo;
	Shader m_shader, m_shader2;
	GLint m_vertexOffsetXLocation;
	GLuint m_texture1, m_texture2;
};

int main(int argc, char** argv) {
	Game game;
	game.Init();
	game.Run();
	return 0;
}
