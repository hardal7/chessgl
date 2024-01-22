#version 330 core
layout (location = 0) in vec2 aPos;

out vec3 oColor;

void main()
{
    //Render the board with 0.15 offset
    //gl_Position = vec4(aPos.x<0?aPos.x+0.15f:aPos.x-0.15f, aPos.y<0?aPos.y+0.15f:aPos.y-0.15f, 1.0f, 1.0f);
    gl_Position = vec4(aPos, 1.0f, 1.0f);
}
