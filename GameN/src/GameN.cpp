#pragma once

#include <iostream>
#include <filesystem>
#include <map>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>	

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Primitive.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
unsigned int loadCubemap(std::vector<std::string> faces);

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


std::vector<glm::vec3> objectPositions;

bool spotLightOn = false;

GLuint lightVao;

Shader objShader, strokeShader, screenShader, skyboxShader, normalShader;

float screenQuad[] = {
	// positions   // texture coords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
};

std::vector<std::string> faces {
		"res/textures/skybox/right.jpg",
		"res/textures/skybox/left.jpg",
		"res/textures/skybox/top.jpg",
		"res/textures/skybox/bottom.jpg",
		"res/textures/skybox/front.jpg",
		"res/textures/skybox/back.jpg"
};

float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

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
		glfwWindowHint(GLFW_SAMPLES, 4);

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

		//glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_CULL_FACE);
		//glEnable(GL_STENCIL_TEST);
		//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void UpdateDeltaTime() {
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}

	void Run() {
		unsigned int screenVAO, screenVBO;
		glGenVertexArrays(1, &screenVAO);
		glGenBuffers(1, &screenVBO);

		glBindVertexArray(screenVAO);
		glBindBuffer(GL_ARRAY_BUFFER, screenVBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuad), &screenQuad, GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		// vertex texture coords
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		glBindVertexArray(0);
		
		//framebuffer 
		unsigned int framebuffer;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		unsigned int texColorBuffer;
		glGenTextures(1, &texColorBuffer);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_windowWidth, m_windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_windowWidth, m_windowHeight);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);



		unsigned int skyboxVAO, skyboxVBO;
		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);

		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glBindVertexArray(0);
		
		
		unsigned int cubemapTexture = loadCubemap(faces);

		Model testModel("res/models/nanosuit/nanosuit.obj");

		//objectPositions.push_back(glm::vec3(-0.98f, 2.0f, 0.5f));
		//objectPositions.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
		//objectPositions.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
		//objectPositions.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
		//objectPositions.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

		//Primitive cube(Primitive_Type::CUBE, "res/textures/box.png", GL_REPEAT);
		//Primitive plane(Primitive_Type::PLANE, "res/textures/concrete.jpg", GL_REPEAT);
		//Primitive quadGrass(Primitive_Type::QUAD, "res/textures/grass.png", GL_CLAMP_TO_EDGE);
		//Primitive quadWindow(Primitive_Type::QUAD, "res/textures/blending_transparent_window.png", GL_CLAMP_TO_EDGE);
		objShader.Create("res/shaders/model.vert", "res/shaders/model.frag", ""); 
		normalShader.Create("res/shaders/show_normals.vert", "res/shaders/single_color.frag", "res/shaders/show_normals.geom");
		//screenShader.Create("res/shaders/pos_texpos.vert", "res/shaders/screen_tex.frag", "");
		skyboxShader.Create("res/shaders/skybox.vert", "res/shaders/skybox.frag", "");
		//strokeShader.Create("res/shaders/model.vert", "res/shaders/single_color.frag");

		//m_ObjShader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		//m_ObjShader.setVec3("dirLight.ambient", glm::vec3(0.4f, 0.4f, 0.4f));
		//m_ObjShader.setVec3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		//m_ObjShader.setVec3("dirLight.specular", glm::vec3(0.1f, 0.1f, 0.1f));

		//m_ObjShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		//m_ObjShader.setVec3("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		//m_ObjShader.setVec3("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		//m_ObjShader.setVec3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		//m_ObjShader.setFloat("pointLights[0].constant", 1.0f);
		//m_ObjShader.setFloat("pointLights[0].linear", 0.09);
		//m_ObjShader.setFloat("pointLights[0].quadratic", 0.032);

		//m_ObjShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		//m_ObjShader.setVec3("pointLights[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		//m_ObjShader.setVec3("pointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		//m_ObjShader.setVec3("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		//m_ObjShader.setFloat("pointLights[1].constant", 1.0f);
		//m_ObjShader.setFloat("pointLights[1].linear", 0.09);
		//m_ObjShader.setFloat("pointLights[1].quadratic", 0.032);

		while (!glfwWindowShouldClose(m_window)) {
			processInput(m_window);
			
			glfwPollEvents();
			//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glEnable(GL_DEPTH_TEST);

			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			
			UpdateDeltaTime();


			//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), (float)m_windowWidth / (float)m_windowHeight, 0.1f, 100.0f);


			//strokeShader.Use();
			//strokeShader.setMat4("view", view);
			//strokeShader.setMat4("projection", projection);
			glm::mat4 view = camera.GetViewMatrix();


			objShader.Use();
			objShader.setMat4("view", view);
			objShader.setMat4("projection", projection);

			//glStencilMask(0x00);

			//objShader.setMat4("model", model);
			//plane.Draw(objShader);

			//glStencilFunc(GL_ALWAYS, 1, 0xFF);
			//glStencilMask(0xFF);
			//glEnable(GL_CULL_FACE);
			//model = glm::translate(model, glm::vec3(-1.0f, 0.002f, -0.3f));
			//objShader.setMat4("model", model);
			//cube.Draw(objShader);

			//model = glm::mat4(1.0f);
			//model = glm::translate(model, glm::vec3(2.0f, 0.002f, 1.0f));
			//objShader.setMat4("model", model);
			//cube.Draw(objShader);


			model = glm::mat4(1.0f);
			//model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
			//model = glm::translate(model, glm::vec3(-6.5f, 3.35f, -1.4f));
			//model = glm::rotate(model, glm::radians(-(float)glfwGetTime() * 15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			//objShader.setVec3("cameraPos", camera.GetPosition());
			objShader.setMat4("model", model);

			glActiveTexture(GL_TEXTURE3);
			objShader.setInt("skybox", 3);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
			glActiveTexture(GL_TEXTURE0);
			
			testModel.Draw(objShader);

			//normalShader.Use();
			//normalShader.setMat4("view", view);
			//normalShader.setMat4("projection", projection);
			//normalShader.setMat4("model", model);
			//testModel.Draw(normalShader);
			//model = glm::mat4(1.0f);
			//model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
			//model = glm::translate(model, glm::vec3(13.5f, 3.35f, 7.0f));
			//objShader.setMat4("model", model);
			//testModel.Draw(objShader);

			skyboxShader.Use();

			view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
			
			skyboxShader.setMat4("view", view);
			skyboxShader.setMat4("projection", projection);


			glDepthFunc(GL_LEQUAL);
			glBindVertexArray(skyboxVAO);
			//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glDepthFunc(GL_LESS);
			//glDisable(GL_CULL_FACE);

			//glBindFramebuffer(GL_FRAMEBUFFER, 0);
			//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			//glClear(GL_COLOR_BUFFER_BIT);

			//screenShader.Use();
			//glBindVertexArray(screenVAO);
			//glDisable(GL_DEPTH_TEST);
			//glBindTexture(GL_TEXTURE_2D, texColorBuffer);
			//glDrawArrays(GL_TRIANGLES, 0, 6);

			//glEnable(GL_BLEND);

			//std::map<float, glm::vec3> sortedPositions;
			//for (unsigned int i = 0; i < objectPositions.size(); i++) {
			//	float distance = glm::length(camera.GetPosition() - objectPositions[i]);
			//	sortedPositions[distance] = objectPositions[i];
			//}

			//for (std::map<float, glm::vec3>::reverse_iterator it = sortedPositions.rbegin(); it != sortedPositions.rend(); ++it) {
			//	model = glm::mat4(1.0f);
			//	model = glm::translate(model, it->second);
			//	objShader.setMat4("model", model);
			//	quadWindow.Draw(objShader);
			//}

			//glDisable(GL_BLEND);


			
			//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			//glStencilMask(0x00);
			//glDisable(GL_DEPTH_TEST);

			//strokeShader.Use();
			//float scale = 1.05;

			//model = glm::mat4(1.0f);
			//model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
			//model = glm::scale(model, glm::vec3(scale, scale, scale));
			//strokeShader.setMat4("model", model);
			//cube.Draw(strokeShader);

			//model = glm::mat4(1.0f);
			//model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
			//model = glm::scale(model, glm::vec3(scale, scale, scale));
			//strokeShader.setMat4("model", model);
			//cube.Draw(strokeShader);

			//glStencilMask(0xFF);
			//glEnable(GL_DEPTH_TEST);

			//model = glm::mat4(1.0f);

			//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
			//model = glm::rotate(model, glm::radians(-(float)glfwGetTime() * 10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			//objShader.setMat4("model", model);
			//glm::vec3 cameraPos = camera.GetPosition();
			//objShader.set3Float("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
			//cube.Draw(objShader);

			//testModel.Draw(m_ObjShader);
			//model = glm::mat4(1.0f);
			//model = glm::translate(model, glm::vec3(1.7f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
			//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
			//model = glm::rotate(model, glm::radians(-(float)glfwGetTime() * 15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			//m_ObjShader.setMat4("model", model);
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			//testModel.Draw(m_ObjShader);

			glfwSwapBuffers(m_window);
		}

		glDeleteFramebuffers(1, &framebuffer);
		glDeleteVertexArrays(1, &screenVAO);
		glDeleteBuffers(1, &screenVBO);
	}

private:
	GLFWwindow* m_window = nullptr;
	const unsigned int m_windowWidth = SCR_WIDTH;
	const unsigned int m_windowHeight = SCR_HEIGHT;
	const char* m_windowTitle = "GameN";
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

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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

unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}


int main(int argc, char** argv) {
	Game game;

	game.Init();
	game.Run();

	return 0;
}
