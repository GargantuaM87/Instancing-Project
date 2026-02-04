#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform int currentKernel;
uniform int gammaCorrection;

const float offset = 1.0 / 300.0;
float gamma = 2.2;


void main() {
  vec2 offsets[9] = vec2[](
        vec2(-offset, offset),
        vec2(0.0f, offset),
        vec2(offset, offset),
        vec2(-offset, 0.0f),
        vec2(0.0f, 0.0f),
        vec2(offset, 0.0f),
        vec2(-offset, -offset),
        vec2(0.0f, -offset),
        vec2(offset, -offset)
   );

   float kernel[9];
   if(currentKernel == 0) { // identity kernel
    kernel = float[] (
     0, 0, 0,
     0, 1, 0,
     0, 0, 0
     );
   }
   else if(currentKernel == 1) { // edge detection kernel
    kernel = float[] (
     -1, -1, -1,
     -1, 8, -1,
     -1, -1, -1
     );
   }
   else if(currentKernel == 2) {  // box blur kernel
     kernel = float[] (
     1.0/9, 1.0/9, 1.0/9,
     1.0/9, 1.0/9, 1.0/9,
     1.0/9, 1.0/9, 1.0/9
     );
   } 
   else if(currentKernel == 3) { // guassian blur kernel
     kernel = float[] (
     1.0/16, 2.0/16, 1.0/16,
     2.0/16, 4.0/16, 2.0/16,
     1.0/16, 2.0/16, 1.0/16
     );
   }
   else if(currentKernel == 4) { // emboss kernel
     kernel = float[] (
     -2, -1, 0,
     -1,  1, 1,
      0,  1, 2
     );
   }
    else if(currentKernel == 5) { // sharpen kernel
     kernel = float[] (
      0, -1, 0,
     -1,  5, -1,
      0, -1, 0
     );
   }

   vec3 sampleTex[9];
   for(int i = 0; i < 9; i++)
   {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
   }
   vec3 col = vec3(0.0);
   for(int i = 0; i < 9; i++)
   {
        col += sampleTex[i] * kernel[i];
   }

   if(gammaCorrection == 0)
    gamma = 2.2;
   else 
    gamma = 1.0;
   FragColor = vec4(pow(col.rgb, vec3(gamma)), 1.0);
   //FragColor = vec4(col, 1.0);
}