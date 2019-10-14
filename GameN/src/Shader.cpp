#include "Shader.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) : m_shaderProgram(0) {
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const std::string vertexShaderSource = ParseShader(vertexPath);
	const GLchar* srcV = vertexShaderSource.c_str();
	glShaderSource(vertexShader, 1, &srcV, NULL);
	glCompileShader(vertexShader);
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

Shader::~Shader() {

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

void Shader::Bind() {
	glUseProgram(m_shaderProgram);
}

void Shader::Unbind() {
	glUseProgram(0);
}