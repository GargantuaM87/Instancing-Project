#version 330 core
out vec4 FragColor;

in vec3 vPosition;
in vec3 vNormal;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
    float refractRatio = 1.00 / 1.52;
    vec3 incident = normalize(vPosition - cameraPos);
    vec3 reflection = reflect(incident, normalize(vNormal));
    reflection = vec3(reflection.x, -reflection.y, reflection.z);
    FragColor = vec4(texture(skybox, reflection).rgb, 1.0);
}