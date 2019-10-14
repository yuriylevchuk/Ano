#pragma once

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


class Shader {
public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	void Bind();
	void Unbind();
private:
	GLuint m_shaderProgram;
private:
	std::string ParseShader(const std::string& filePath);
};