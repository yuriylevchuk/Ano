#pragma once

#include<iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Application {
public:
	Application();
	~Application();

	void Init(const unsigned int windowWidth, const unsigned int windowHeight, const char* windowTitle);
	void Run();

private:
	GLFWwindow* m_window;
private:
	void OnUpdate();
};