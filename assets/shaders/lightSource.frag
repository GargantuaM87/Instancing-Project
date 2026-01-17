#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main() 
{
   float refractRatio = 1.00 / 1.52;
   vec3 incident = normalize(Position - cameraPos);
   //vec3 reflection = reflect(incident, normalize(Normal));
   vec3 reflection = refract(incident, normalize(Normal), refractRatio);
   reflection = vec3(reflection.x, -reflection.y, reflection.z);
   FragColor = vec4(texture(skybox, reflection).rgb, 1.0);
}