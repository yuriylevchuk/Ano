#pragma once

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


class Shader {
public:
	Shader();
	~Shader();

	GLuint GetID() const;

	void Use();
	void Create(const std::string& vertexPath, const std::string& fragmentPath);
private:
	GLuint m_shaderProgram;
private:
	std::string ParseShader(const std::string& filePath);
};