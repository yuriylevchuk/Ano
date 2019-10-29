#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

GLfloat m_lastX = 600, m_lastY = 500;
GLfloat m_yaw = -90.0f;
GLfloat m_pitch = 0.0f;
bool m_firstMouse = true;
GLfloat m_fov = 45.0f;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

class Game {
public:
	Game() : m_ibo(0), m_vao(0), m_vbo(0), m_texture1(0), m_texture2(0), m_mixingRatio(0.2f) {
	}

	~Game() {
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void Init() {

		if (!glfwInit()) {
			std::cout << "Failed to initialize GLFW" << std::endl;
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

		glfwSwapInterval(1);

		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glfwSetCursorPosCallback(m_window, mouse_callback);
		glfwSetScrollCallback(m_window, scroll_callback);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
		}

		glViewport(0, 0, m_windowWidth, m_windowHeight);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void processInput()
	{
		if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(m_window, true);

		GLfloat cameraSpeed = 2.5f * m_deltaTime;
		if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
			m_cameraPos += cameraSpeed * m_cameraFront;
		}

		if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
			m_cameraPos -= cameraSpeed * m_cameraFront;
		}

		if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
			m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
		}

		if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
			m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
		}
	}

	glm::mat4 LookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up) {
		glm::vec3 cameraDirection = glm::normalize(pos - target);
		glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
		glm::mat4 translator = glm::mat4(1.0f);
		glm::mat4 rotator = glm::mat4(1.0f);
		translator[3][0] = -pos.x;
		translator[3][1] = -pos.y;
		translator[3][2] = -pos.z;
		rotator[0][0] = cameraRight.x;
		rotator[1][0] = cameraRight.y;
		rotator[2][0] = cameraRight.z;
		rotator[0][1] = cameraUp.x;
		rotator[1][1] = cameraUp.y;
		rotator[2][1] = cameraUp.z;
		rotator[0][2] = cameraDirection.x;
		rotator[1][2] = cameraDirection.y;
		rotator[2][2] = cameraDirection.z;
		return rotator * translator;
	}

	void OnUpdate() {
		GLfloat currentFrame = glfwGetTime();
		m_deltaTime = currentFrame - m_lastFrame;
		m_lastFrame = currentFrame;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_texture2);


		// Camera
		glm::vec3 front;
		front.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
		front.y = sin(glm::radians(m_pitch));
		front.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
		m_cameraFront = glm::normalize(front);

		glm::mat4 view = glm::mat4(1.0f);
		view = LookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(m_fov), (float) m_windowWidth / (float) m_windowHeight, 0.1f, 100.0f);

		GLuint modelLoc = glGetUniformLocation(m_shader.GetID(), "model");
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GLuint viewLoc = glGetUniformLocation(m_shader.GetID(), "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		GLuint projectionLoc = glGetUniformLocation(m_shader.GetID(), "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glUniform1f(glGetUniformLocation(m_shader.GetID(), "mixingRatio"), m_mixingRatio);

		glBindVertexArray(m_vao);

		for (GLuint i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, m_cubePositions[i]);
			GLfloat angle = 20.0f * ( i + 1 ) * (GLfloat)glfwGetTime();
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0 , 36);

		glBindVertexArray(0);
	}

	void Run() {
		GLfloat tile[] = {
			// positions		//texture coords
			-0.5f, 0.5f, 0.0f,  0.0f, 1.0f,
			-0.5f,-0.5f, 0.0f,	0.0f, 0.0f,
			 0.5f,-0.5f, 0.0f,  1.0f, 0.0f,
			 0.5f, 0.5f, 0.0f,  1.0f, 1.0f
		};

		GLfloat cube[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		GLuint tileIndices[] = {
			0, 1, 2,
			2, 3, 0
		};

		glGenBuffers(1, &m_vbo);
		//glGenBuffers(1, &m_ibo);
		glGenVertexArrays(1, &m_vao);

		// Массив буфферов
		glBindVertexArray(m_vao);

		// Буффер вершин
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

		// position attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// texture coords attributes
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		// Буффер индексов
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tileIndices), tileIndices, GL_STATIC_DRAW);

		glBindVertexArray(0);

		//------------------------------------------------------

		m_shader.Create("res/shaders/Base.vert", "res/shaders/Base2.frag");

		int imgWidth, imgHeight, imgChannels;
		unsigned char* imgData = stbi_load("res/textures/box.png", &imgWidth, &imgHeight, &imgChannels, 0);
		
		glGenTextures(1, &m_texture1);
		glBindTexture(GL_TEXTURE_2D, m_texture1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (imgData) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
			glGenerateMipmap(GL_TEXTURE_2D);
		} else {
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(imgData);

		// --------------------------------------------------------------------------------
		stbi_set_flip_vertically_on_load(true);
		imgData = stbi_load("res/textures/skull2.jpg", &imgWidth, &imgHeight, &imgChannels, 0);
		glGenTextures(1, &m_texture2);
		glBindTexture(GL_TEXTURE_2D, m_texture2);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (imgData) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
			glGenerateMipmap(GL_TEXTURE_2D);
		} else {
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(imgData);

		m_shader.Use();


		glUniform1i(glGetUniformLocation(m_shader.GetID(), "ourTexture1"), 0);
		glUniform1i(glGetUniformLocation(m_shader.GetID(), "ourTexture2"), 1);

		while (!glfwWindowShouldClose(m_window)) {
			processInput();
			
			glfwPollEvents();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			OnUpdate();

			glfwSwapBuffers(m_window);
		}

		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		//glDeleteBuffers(1, &m_ibo);
	}

private:
	GLFWwindow* m_window = nullptr;
	const unsigned int m_windowWidth = 1200;
	const unsigned int m_windowHeight = 1000;
	const char* m_windowTitle = "GameN";
	GLuint m_vao, m_vbo, m_ibo;
	Shader m_shader;
	GLuint m_texture1, m_texture2;
	GLfloat m_mixingRatio;

	glm::vec3 m_cubePositions[10] = {
		  glm::vec3(0.0f,  0.0f,  0.0f),
		  glm::vec3(2.0f,  5.0f, -15.0f),
		  glm::vec3(-1.5f, -2.2f, -2.5f),
		  glm::vec3(-3.8f, -2.0f, -12.3f),
		  glm::vec3(2.4f, -0.4f, -3.5f),
		  glm::vec3(-1.7f,  3.0f, -7.5f),
		  glm::vec3(1.3f, -2.0f, -2.5f),
		  glm::vec3(1.5f,  2.0f, -2.5f),
		  glm::vec3(1.5f,  0.2f, -1.5f),
		  glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	GLfloat m_deltaTime = 0.0f;	// Время, прошедшее между последним и текущим кадром
	GLfloat m_lastFrame = 0.0f;  	// Время вывода последнего кадра
};

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (m_firstMouse)
	{
		m_lastX = xpos;
		m_lastY = ypos;
		m_firstMouse = false;
	}

	GLfloat xoffset = xpos - m_lastX;
	GLfloat yoffset = m_lastY - ypos;
	m_lastX = xpos;
	m_lastY = ypos;
	GLfloat sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	m_yaw += xoffset;
	m_pitch += yoffset;

	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	if (m_pitch < -89.0f)
		m_pitch = -89.0f;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (m_fov >= 1.0f && m_fov <= 45.0f)
		m_fov -= yoffset;
	if (m_fov <= 1.0f)
		m_fov = 1.0f;
	if (m_fov >= 45.0f)
		m_fov = 45.0f;
}

int main(int argc, char** argv) {
	Game game;
	game.Init();
	game.Run();
	return 0;
}
