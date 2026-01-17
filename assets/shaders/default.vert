#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
 // Uniforms are inputted from the CPU to all parallel threads on the GPU
uniform mat4 model;
uniform mat4 camMatrix;

void main()
{
    gl_Position = camMatrix * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));

    // Inversing matrices is a costly operation for shaders since they have to be done on each vertex of our scene
    // It'd be more efficient to calculate this normal matrix on the CPU and send it to the shaders via a uniform before drawing
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
}