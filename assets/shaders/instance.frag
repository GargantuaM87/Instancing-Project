#version 330 core
out vec4 FragColor;

uniform sampler2D text;

in vec2 TexCoords;

void main()
{
    FragColor = texture(text, TexCoords);
}