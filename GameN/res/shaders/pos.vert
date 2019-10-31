#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  gl_Position = projection * view * model * vec4(pos, 1.0f);
  FragPos = vec3(model * vec4(pos, 1.0f));
  // TODO transfer normal matrix computation to CPU (it's a hard operation)
  Normal =  mat3(transpose(inverse(model))) * normal;
}