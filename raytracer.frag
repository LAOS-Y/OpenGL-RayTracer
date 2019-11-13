#version 430 core
out vec4 FragColor;

in vec2 TexCoord;

// texture sampler
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	FragColor = (
                 texture(texture0, TexCoord)
                 + texture(texture1, TexCoord)
                 + texture(texture2, TexCoord)
                 ) / 3;
}