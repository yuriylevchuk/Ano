#include "Primitive.h"
#include "Model.h"

Primitive::Primitive(Primitive_Type type, const std::string texturePath, GLint textureWrapping) {
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	if (type == Primitive_Type::PLANE) {
		glBufferData(GL_ARRAY_BUFFER, sizeof(PLANE_VERTICES), &PLANE_VERTICES, GL_STATIC_DRAW);
		mVerticesCount = sizeof(PLANE_VERTICES) / sizeof(float) / ATTRIBUTES_SIZE;
	}

	if (type == Primitive_Type::CUBE) {
		glBufferData(GL_ARRAY_BUFFER, sizeof(CUBE_VERTICES), &CUBE_VERTICES, GL_STATIC_DRAW);
		mVerticesCount = sizeof(CUBE_VERTICES) / sizeof(float) / ATTRIBUTES_SIZE;
	}

	if (type == Primitive_Type::QUAD) {
		glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD_VERTICES), &QUAD_VERTICES, GL_STATIC_DRAW);
		mVerticesCount = sizeof(QUAD_VERTICES) / sizeof(float) / ATTRIBUTES_SIZE;
	}

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, ATTRIBUTES_SIZE * sizeof(float), (void*)0);
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, ATTRIBUTES_SIZE * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);

	mTexture = TextureFromFile(texturePath.c_str(), "", textureWrapping);
}

Primitive::~Primitive() {
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
}

void Primitive::Draw(Shader shader) {
	glActiveTexture(GL_TEXTURE0);

	shader.setFloat("diffuse_texture1", 0);
	glBindTexture(GL_TEXTURE_2D, mTexture);

	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, mVerticesCount);
	glBindVertexArray(0);
}
