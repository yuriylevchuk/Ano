#pragma once

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glm/glm.hpp>


class Shader {
public:
	Shader();
	~Shader();

	void setInt(const std::string &name, unsigned int value) const;
	void setFloat(const std::string &name, float value) const;
	void set3Float(const std::string &name, float v1, float v2, float v3) const;
	void setVec3(const std::string& name, const glm::vec3& vec) const;
	void setMat4(const std::string &name, const glm::mat4 &mat) const;

	void Create(const std::string &vertexPath, const std::string &fragmentPath);
	void Use();
private:
	GLuint m_shaderProgram;
private:
	std::string ParseShader(const std::string &filePath);
};