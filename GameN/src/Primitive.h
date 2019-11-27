#pragma once

#include <iostream>

#include "shader.h"

enum class Primitive_Type {
	PLANE,
	CUBE,
	QUAD
};

const unsigned int ATTRIBUTES_SIZE = 5;

const float CUBE_VERTICES[] = {
	// Back face
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
	// Front face
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
	// Left face
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
	// Right face
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
	// Bottom face
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
	// Top face
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left   
};

const float PLANE_VERTICES[] = {
	 // positions         // texture coords
	 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 0.0f,
	-5.0f, -0.5f,  5.0f,  0.0f, 2.0f,

	 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	 5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
	-5.0f, -0.5f, -5.0f,  2.0f, 2.0f
};

const float QUAD_VERTICES[] = {
	// positions         // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
};

class Primitive
{
public:
	Primitive(Primitive_Type type, const std::string texturePath, GLint textureWrapping);
	~Primitive();

	void Draw(Shader shader);
private:
	unsigned int mVAO, mVBO, mTexture, mVerticesCount;
};