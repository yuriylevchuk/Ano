#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
// out vec3 FragPos;
out vec3 Normal;
out vec3 Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  //gl_Position = projection * view * model * vec4(aPos, 1.0f);
  // FragPos = vec3(model * vec4(aPos, 1.0f));
  // TODO transfer normal matrix computation to CPU (it's a hard operation)
  // Normal =  mat3(transpose(inverse(model))) * aNormal;
  TexCoords = aTexCoords;
  Normal = mat3(transpose(inverse(model))) * aNormal;
  Position = vec3(model * vec4(aPos, 1.0));
  gl_Position = projection * view * model * vec4(aPos, 1.0);
}
