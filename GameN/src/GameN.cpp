#pragma once

#include <iostream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>	

#include "shader.h"
#include "camera.h"
#include "Model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 1000;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;	// Время, прошедшее между последним и текущим кадром
GLfloat lastFrame = 0.0f;  	// Время вывода последнего кадра


glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

bool spotLightOn = false;

GLuint lightVao;

class Game {
public:
	Game() {
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

		m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, m_windowTitle, NULL, NULL);
		if (!m_window) {
			std::cout << "Failed to create window: " << m_windowTitle << std::endl;
			glfwTerminate();
		}

		glfwMakeContextCurrent(m_window);

		glfwSwapInterval(1);

		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE);

		glfwSetCursorPosCallback(m_window, mouse_callback);
		glfwSetScrollCallback(m_window, scroll_callback);
		glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize GLAD" << std::endl;
		}

		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void OnUpdate() {
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		


		//lightPos.x = cos(glfwGetTime()) * 2.0f;
		//lightPos.y = sin(glfwGetTime()) * 0.5f;
		//lightPos.z = sin(glfwGetTime()) * 2.0f;
		
	//	m_ObjShader.setVec3("spotLight.position", camera.GetPosition());
	//	m_ObjShader.setVec3("spotLight.direction", camera.GetFront());
	//	m_ObjShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	//	m_ObjShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

	/*	if (spotLightOn) {
			m_ObjShader.setInt("spotLightOn", 1);
		} else {
			m_ObjShader.setInt("spotLightOn", 0);
		}

		m_LightShader.Use();
		m_LightShader.setMat4("view", view);
		m_LightShader.setMat4("projection", projection);

		glBindVertexArray(lightVao);
		for (GLuint i = 0; i < 4; i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.1f));
			m_LightShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}*/
	}

	void Run() {
		Model testModel("res/models/nanosuit/nanosuit.obj");
		//Model testModel("res/models/cottage/cottage_blender.obj");
		m_ObjShader.Create("res/shaders/model.vert", "res/shaders/model.frag");
		m_ObjShader.Use();

		m_ObjShader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		m_ObjShader.setVec3("dirLight.ambient", glm::vec3(0.4f, 0.4f, 0.4f));
		m_ObjShader.setVec3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		m_ObjShader.setVec3("dirLight.specular", glm::vec3(0.1f, 0.1f, 0.1f));

		m_ObjShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		m_ObjShader.setVec3("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		m_ObjShader.setVec3("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		m_ObjShader.setVec3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		m_ObjShader.setFloat("pointLights[0].constant", 1.0f);
		m_ObjShader.setFloat("pointLights[0].linear", 0.09);
		m_ObjShader.setFloat("pointLights[0].quadratic", 0.032);

		m_ObjShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		m_ObjShader.setVec3("pointLights[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		m_ObjShader.setVec3("pointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		m_ObjShader.setVec3("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		m_ObjShader.setFloat("pointLights[1].constant", 1.0f);
		m_ObjShader.setFloat("pointLights[1].linear", 0.09);
		m_ObjShader.setFloat("pointLights[1].quadratic", 0.032);

		while (!glfwWindowShouldClose(m_window)) {
			processInput(m_window);
			
			glfwPollEvents();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			OnUpdate();

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			glm::mat4 view = camera.GetViewMatrix();
			m_ObjShader.setMat4("view", view);

			glm::mat4 projection = glm::mat4(1.0f);
			projection = glm::perspective(glm::radians(camera.GetZoom()), (float)m_windowWidth / (float)m_windowHeight, 0.1f, 100.0f);
			m_ObjShader.setMat4("projection", projection);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
			model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
			//model = glm::rotate(model, glm::radians(-(float)glfwGetTime() * 10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			m_ObjShader.setMat4("model", model);

			glm::vec3 cameraPos = camera.GetPosition();
			m_ObjShader.set3Float("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);


			testModel.Draw(m_ObjShader);


			//model = glm::mat4(1.0f);
			//model = glm::translate(model, glm::vec3(1.7f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
			//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
			//model = glm::rotate(model, glm::radians(-(float)glfwGetTime() * 15.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			//m_ObjShader.setMat4("model", model);

			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			
			//testModel.Draw(m_ObjShader);

			glfwSwapBuffers(m_window);
		}

/*		glDeleteVertexArrays(1, &objVao);
		glDeleteVertexArrays(1, &lightVao);
		glDeleteBuffers(1, &vbo);*/
	}

private:
	GLFWwindow* m_window = nullptr;
	const unsigned int m_windowWidth = SCR_WIDTH;
	const unsigned int m_windowHeight = SCR_HEIGHT;
	const char* m_windowTitle = "GameN";
	Shader m_ObjShader; 
	Shader m_LightShader;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
			spotLightOn = !spotLightOn;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}

int main(int argc, char** argv) {
	Game game;

	game.Init();
	game.Run();

	return 0;
}
