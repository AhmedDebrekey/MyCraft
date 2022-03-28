#version 330 core

out vec4 FragColor;

uniform sampler2D u_Texture;

in vec2 v_TexCoord;
in float LightLevel;

void main()
{
	FragColor = texture(u_Texture, v_TexCoord) * LightLevel;
}