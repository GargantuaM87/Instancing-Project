#include <iostream>
#include "../headers/imgui/imgui.h"
#include "../headers/imgui/imgui_internal.h"
#include "../headers/imgui/imgui_impl_glfw.h"
#include "../headers/imgui/imgui_impl_opengl3.h"
#include "glad.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <filesystem>

#include "../headers/shaderClass.h"
#include "../headers/EBO.h"
#include "../headers/VBO.h"
#include "../headers/VAO.h"
#include "../headers/FBO.h"
#include "../headers/texture.h"
#include "../headers/camera.h"
#include "../headers/uniform.h"
#include "../headers/model.h"

#include "../include/stb/stb_image.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"
#include <map>

const unsigned int width = 800;
const unsigned int height = 800;

unsigned int LoadCubeMap(std::vector<std::string> faces)
{
     unsigned int textureID;
     glGenTextures(1, &textureID);
     glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

     int width, height, nrChannels;
     for(unsigned int i = 0; i < faces.size(); i++)
     {
          unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
          if(data)
          {
               glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
               stbi_image_free(data);
          }
          else
          {
               std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
               stbi_image_free(data);
          }
     }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}


void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
     glViewport(0, 0, width, height);
}

int main(int, char **)
{
     float quadVertices[] = {
          // positions     // texCoords
          -1.0f,  1.0f,  0.0f, 1.0f,
          -1.0f, -1.0f,  0.0f, 0.0f,
           1.0f, -1.0f,  1.0f, 0.0f,

          -1.0f,  1.0f,  0.0f, 1.0f,
          1.0f, -1.0f,  1.0f, 0.0f,
          1.0f,  1.0f,  1.0f, 1.0f
     };
     float skyboxVertices[] = {
     // positions          
         -1.0f,  1.0f, -1.0f,
         -1.0f, -1.0f, -1.0f,
          1.0f, -1.0f, -1.0f,
          1.0f, -1.0f, -1.0f,
          1.0f,  1.0f, -1.0f,
         -1.0f,  1.0f, -1.0f,

         -1.0f, -1.0f,  1.0f,
         -1.0f, -1.0f, -1.0f,
         -1.0f,  1.0f, -1.0f,
         -1.0f,  1.0f, -1.0f,
         -1.0f,  1.0f,  1.0f,
         -1.0f, -1.0f,  1.0f,

          1.0f, -1.0f, -1.0f,
          1.0f, -1.0f,  1.0f,
          1.0f,  1.0f,  1.0f,
          1.0f,  1.0f,  1.0f,
          1.0f,  1.0f, -1.0f,
          1.0f, -1.0f, -1.0f,

         -1.0f, -1.0f,  1.0f,
         -1.0f,  1.0f,  1.0f,
          1.0f,  1.0f,  1.0f,
          1.0f,  1.0f,  1.0f,
          1.0f, -1.0f,  1.0f,
         -1.0f, -1.0f,  1.0f,

         -1.0f,  1.0f, -1.0f,
          1.0f,  1.0f, -1.0f,
          1.0f,  1.0f,  1.0f,
          1.0f,  1.0f,  1.0f,
         -1.0f,  1.0f,  1.0f,
         -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
};
     
     GLFWwindow *window;

     if (!glfwInit())
     {
          return -1;
     }
     // Create the window that appears on the screen
     window = glfwCreateWindow(width, height, "Window", NULL, NULL);

     // Tells GLFW to add the window to the current context
     // A context being an object that encapsulates the OpenGL state machine
     glfwMakeContextCurrent(window);

     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
     {
          std::cout << "Couldn't load OpenGL" << std::endl;
          glfwTerminate();
          return -1;
     }
     // shaders
     Shader mainShader("../assets/shaders/lightSource.vert", "../assets/shaders/lightSource.frag"); // Shader program for light sources
     Shader framebufferShader("../assets/shaders/framebuffer.vert", "../assets/shaders/framebuffer.frag");
     Shader skyboxShader("../assets/shaders/skybox.vert", "../assets/shaders/skybox.frag");
     Shader modelShader("../assets/shaders/model.vert", "../assets/shaders/model.frag");
     Shader instanceShader("../assets/shaders/instance.vert", "../assets/shaders/instance.frag");
     // cubemap texture
     unsigned int texID;
     glGenTextures(1, &texID);
     glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
     // models
     Model planet("../assets/planet/planet.obj");
     Model icosphere("../assets/icosphere.obj");
     
     // quad geometry
     VAO quadVAO;
     VBO quadVBO(quadVertices, sizeof(quadVertices));
     quadVAO.Bind();
     quadVAO.LinkAttrib(quadVBO, 0, 2, GL_FLOAT, 4 * sizeof(float), (void*)0);
     quadVAO.LinkAttrib(quadVBO, 1, 2, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));
     quadVAO.Unbind();
     // skybox geometry
     VAO skyboxVAO;
     VBO skyboxVBO(skyboxVertices, sizeof(skyboxVertices));
     skyboxVAO.Bind();
     skyboxVAO.LinkAttrib(skyboxVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
     skyboxVAO.Unbind();
     // create transformation matrices
     unsigned int amount = 10000;
     glm::mat4* modelMatrices;
     modelMatrices = new glm::mat4[amount];
     srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed
     float radius = 100.0f;
     float offset = 25.0f;
     for(unsigned int i = 0; i < amount; i++)
     {
          glm::mat4 model = glm::mat4(1.0f);
          // displace along circle with 'radius' in range [-offset, offset]
          float angle = (float)i / (float)amount * 360.0f;
          float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
          float x = sin(angle) * radius + displacement;
          displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
          float y = displacement * 0.4f;
          displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
          float z = cos(angle) * radius + displacement;
          model = glm::translate(model, glm::vec3(x, y, z));

          // scaling
          float scale = static_cast<float>((rand() % 20) / 50.0 + 0.05);
          model = glm::scale(model, glm::vec3(scale));

          // rotation
          float rotAngle = static_cast<float>((rand() % 360));
          model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

          // add to the list of matrices that we have
          // resonsible for an amount of model matrices with varying displacements, rotations, and scales
          modelMatrices[i] = model;
     }
     VBO buffer(&modelMatrices[0][0][0], amount * sizeof(glm::mat4));

     for(unsigned int i = 0; i < icosphere.meshes.size(); i++)
     {
        unsigned int VAO = icosphere.meshes[i].VAO;
        glBindVertexArray(VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
        // the functions below tell openGL to change these layouts every new instance
        // by default, they'll change every iteration of the vertex shader
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
     }

     // multisampled frame buffer
     FBO fboMSAA;
     fboMSAA.AttatchTextureMSAA(4, width, height);
     fboMSAA.AttatchRenderBufferMSAA(4, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, width, height);
     fboMSAA.CheckStatus(); 
     fboMSAA.Unbind();

     // intermediate frame buffer
     FBO fbo;
     fbo.AttatchTexture(width, height);
     fbo.CheckStatus();
     fbo.Unbind();

     // camera object
     Camera camera(width, height, glm::vec3(0.0f, 2.0f, 100.0f));
     // Setting up the camera's view and projection matrices
     camera.Matrix(45.0f, 0.1f, 200.0f);

     // uniform buffer object
     unsigned int ubo;
     glGenBuffers(1, &ubo);
     glBindBuffer(GL_UNIFORM_BUFFER, ubo);
     glBufferData(GL_UNIFORM_BUFFER, 128, NULL, GL_STATIC_DRAW);
     glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, 2 * sizeof(glm::mat4));
     // filling the buffer with data
     glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera.GetProjMatrix()));
     glBindBuffer(GL_UNIFORM_BUFFER, 0);
     // setting uniform block points
     unsigned int mainS_index = glGetUniformBlockIndex(mainShader.ID, "Matrices");
     glUniformBlockBinding(mainShader.ID, mainS_index, 0);
     unsigned int instanceS_index = glGetUniformBlockIndex(instanceShader.ID, "Matrices");
     glUniformBlockBinding(instanceShader.ID, instanceS_index, 0);
     unsigned int modelS_index = glGetUniformBlockIndex(modelShader.ID, "Matrices");
     glUniformBlockBinding(modelShader.ID, modelS_index, 0);

     glEnable(GL_DEPTH_TEST); // Allows for depth comparison and updates the depth buffer
    // glEnable(GL_BLEND); // enable alpha blending
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

     glm::vec3 lightPos(1.0f);
     glm::vec3 lightDir(1.0f);
     float spinSpeed = 0.01f;
     static int currentItem = 0;
     bool gammaCheck = true;

     float deltaTime = 0.0f;
     float lastFrame = 0.0f;
     
    // cube map texture
     std::vector<std::string> spaceMaps = {
          "../assets/spaceEnv/left.png",
          "../assets/spaceEnv/right.png",
          "../assets/spaceEnv/top.png",
          "../assets/spaceEnv/bottom.png",
          "../assets/spaceEnv/back.png",
          "../assets/spaceEnv/front.png"
     };
     unsigned int cubemapTexture = LoadCubeMap(spaceMaps);

     IMGUI_CHECKVERSION();
     ImGui::CreateContext();
     ImGuiIO &io = ImGui::GetIO();
     (void)io;
     ImGui::StyleColorsDark();
     ImGui_ImplGlfw_InitForOpenGL(window, true);
     ImGui_ImplOpenGL3_Init("#version 330");

     glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

     // Main Render Loop
     while (!glfwWindowShouldClose(window))
     {
          fboMSAA.Bind();
          glEnable(GL_DEPTH_TEST);

          glClearColor(0.0f, 0.0f, 0.15f, 1.0f);
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
          
          
          ImGui_ImplOpenGL3_NewFrame();
          ImGui_ImplGlfw_NewFrame();
          ImGui::NewFrame();

          float crntFrame = glfwGetTime();
          deltaTime = crntFrame - lastFrame;
          lastFrame = crntFrame;

           if (!io.WantCaptureMouse)
               camera.Inputs(window);

          camera.Matrix(45.0f, 0.1f, 200.0f);

          // filling in more data for the uniform buffer object
          glBindBuffer(GL_UNIFORM_BUFFER, ubo);
          glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.GetViewMatrix()));
          glBindBuffer(GL_UNIFORM_BUFFER, 0);

          modelShader.Activate();
          //model shader uniforms
          modelShader.SetToVec3("uViewPos", &camera.Position[0]);
          modelShader.SetToFloat("shininess", 16.0f);
          // directional light
          modelShader.SetToVec3("dirLight.direction", &lightDir[0]);
          modelShader.SetToVec3("dirLight.ambient", &glm::vec3(0.35f)[0]);
          modelShader.SetToVec3("dirLight.diffuse", &glm::vec3(1.0f)[0]);
          modelShader.SetToVec3("dirLight.specular", &glm::vec3(0.08f)[0]);
          // point light
          modelShader.SetToVec3("pointLight.position", &lightPos[0]);
          modelShader.SetToVec3("pointLight.ambient", &glm::vec3(0.8f)[0]);
          modelShader.SetToVec3("pointLight.diffuse", &glm::vec3(1.0f)[0]);
          modelShader.SetToVec3("pointLight.specular", &glm::vec3(1.0f)[0]);
          modelShader.SetToFloat("pointLight.constant", 1.0f);
          modelShader.SetToFloat("pointLight.linear", 0.09f);
          modelShader.SetToFloat("pointLight.quadratic", 0.032f);

          //drawing planet
          glm::mat4 model = glm::mat4(1.0f);
          model = glm::scale(model, glm::vec3(10.0f));
          model = glm::rotate(model, static_cast<float>(glfwGetTime()) * spinSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
          modelShader.Activate();
          modelShader.SetToMat4("model", model);
          planet.Draw(modelShader);

          // drawing icosphere
          instanceShader.Activate();
          instanceShader.SetToVec3("cameraPos", &camera.Position[0]);
          glActiveTexture(GL_TEXTURE0);
          glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
          icosphere.Draw(instanceShader);

          for(unsigned int i = 0; i < icosphere.meshes.size(); i++)
          {
               glBindVertexArray(icosphere.meshes[i].VAO);
               glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(icosphere.meshes[i].indices.size()), GL_UNSIGNED_INT, 0, amount);
               glBindVertexArray(0);
          }
          
          // Sky Box
          glDepthFunc(GL_LEQUAL);
          skyboxShader.Activate();
          glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.GetViewMatrix()));
          glm::mat4 skyboxProj = camera.GetProjMatrix();
          skyboxShader.SetToMat4("projection", skyboxProj);
          skyboxShader.SetToMat4("view", skyboxView);
          skyboxVAO.Bind();
          glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
          glDrawArrays(GL_TRIANGLES, 0, 36);
          glDepthFunc(GL_LESS);
          skyboxVAO.Unbind();

          glBindFramebuffer(GL_READ_FRAMEBUFFER, fboMSAA.ID);
          glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo.ID);
          glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

          fboMSAA.Unbind();
          glDisable(GL_DEPTH_TEST);
          glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
          glClear(GL_COLOR_BUFFER_BIT);

          framebufferShader.Activate();
          framebufferShader.SetToInt("currentKernel", currentItem);
          framebufferShader.SetToInt("gammaCorrection", gammaCheck);
          quadVAO.Bind();
          fbo.BindTexture();
          glDrawArrays(GL_TRIANGLES, 0, 6);
          quadVAO.Unbind();
          
          // GUI STUFF
          ImGui::Begin("OpenGL Settings Panel");
          ImGui::Text("Tweaks");
          ImGui::SliderFloat("Rotation Speed", &spinSpeed, 0.01, 1.0f, "%.3f", 0);
          ImGui::SliderFloat3("Directional Light Direction", &lightDir[0], 0.0f, 50.0f);

          const char* items[] = {"Identity", "Edge-Detection", "Box-Blur", "Guassian-Blur", "Emboss", "Sharpen"};
          
          ImGui::Combo("Post-Processing", &currentItem, items, IM_ARRAYSIZE(items));
          ImGui::Checkbox("Gamma Correction", &gammaCheck);
          
          ImGui::End();

          ImGui::Render();
          ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

          // Swap back buffer with front buffer
          glfwSwapBuffers(window);
          // Makes sure our window is responsive (such as resizing it and moving it)
          glfwPollEvents();
     }

     ImGui_ImplOpenGL3_Shutdown();
     ImGui_ImplGlfw_Shutdown();
     ImGui::DestroyContext();

     quadVAO.Delete();
     quadVBO.Delete();
     fboMSAA.Delete();
     fbo.Delete();
     glDeleteBuffers(1, &ubo);
     
     glfwTerminate();
     return 0;
}
