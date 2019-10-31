#pragma once

#include "Shader.h"

Shader::Shader() : m_shaderProgram(0) {
}

Shader::~Shader() {
}

void Shader::setInt(const std::string &name, unsigned int value) const {
	glUniform1i(glGetUniformLocation(m_shaderProgram, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
	glUniform1f(glGetUniformLocation(m_shaderProgram, name.c_str()), value);
}

void Shader::set3Float(const std::string &name, float v1, float v2, float v3) const {
	glUniform3f(glGetUniformLocation(m_shaderProgram, name.c_str()), v1, v2, v3);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &vec) const {
	glUniform3fv(glGetUniformLocation(m_shaderProgram, name.c_str()), 1, &vec[0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
	glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::Create(const std::string &vertexPath, const std::string &fragmentPath) {
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const std::string vertexShaderSource = ParseShader(vertexPath);
	const GLchar* srcV = vertexShaderSource.c_str();
	glShaderSource(vertexShader, 1, &srcV, NULL);
	glCompileShader(vertexShader); //TODO create CompileShader method
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const std::string fragmentShaderSource = ParseShader(fragmentPath);
	const GLchar* srcF = fragmentShaderSource.c_str();
	glShaderSource(fragmentShader, 1, &srcF, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, vertexShader);
	glAttachShader(m_shaderProgram, fragmentShader);
	glLinkProgram(m_shaderProgram);
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

std::string Shader::ParseShader(const std::string& filePath) {
	std::string shaderCode;
	std::ifstream fs;
	std::stringstream ss;
	fs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		fs.open(filePath);
		ss << fs.rdbuf();
		fs.close();
		shaderCode = ss.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	return shaderCode;
}

void Shader::Use() {
	glUseProgram(m_shaderProgram);
}