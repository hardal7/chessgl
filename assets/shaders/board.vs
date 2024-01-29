#version 330 core
layout (location = 0) in vec2 aPos;

out vec3 oColor;

void main()
{
  gl_Position = vec4(aPos, 1.0f, 1.0f);
}
