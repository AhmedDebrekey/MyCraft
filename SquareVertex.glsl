#version 330 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in float aLightLevel;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 v_TexCoord;

out float LightLevel;

void main()
{
   gl_Position = projection * view * model * aPos;
   v_TexCoord = TexCoord;
   LightLevel = aLightLevel;
}