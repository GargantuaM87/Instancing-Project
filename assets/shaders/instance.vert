#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 3) in mat4 instanceMatrix;

out vec3 vPosition;
out vec3 vNormal;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

void main()
{
    vNormal = mat3(transpose(inverse(instanceMatrix))) * aNormal;
    vPosition = vec3(instanceMatrix * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(vPosition, 1.0);
}