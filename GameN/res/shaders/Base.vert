#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texPos;
// layout (location = 1) in vec3 color;

out vec3 ourColor;
out vec2 texCoord;

uniform float uOffsetX;

void main() {
  gl_Position = vec4(pos.x + uOffsetX, pos.y, pos.z, 1.0f);
  texCoord = texPos;
  // ourColor = color;
}