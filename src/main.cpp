#include <iostream>
#include "../headers/imgui/imgui.h"
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
               glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
     
     float octahedronVertices[] = {
          //top-north-west
          0.0,  1.0,  0.0,
          -1.0,  0.0,  0.0,
          0.0,  0.0,  1.0,

          //top-south-west
          0.0,  1.0,  0.0,
          0.0,  0.0, -1.0,
          -1.0,  0.0,  0.0,

          //top-south-east
          0.0,  1.0,  0.0,
          1.0,  0.0,  0.0,
          0.0,  0.0, -1.0,

          //bottom-north-east
          0.0, -1.0,  0.0,
          1.0,  0.0,  0.0,
          0.0,  0.0,  1.0,

          //bottom-north-west
          0.0, -1.0,  0.0,
          0.0,  0.0,  1.0,
          -1.0,  0.0,  0.0,

          //bottom-south-west
          0.0, -1.0,  0.0,
          -1.0,  0.0,  0.0,
          0.0,  0.0, -1.0,

          //bottom-south-east
          0.0, -1.0,  0.0,
          0.0,  0.0, -1.0,
          1.0,  0.0,  0.0,
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
     Shader simpleShader("../assets/shaders/simple.vert", "../assets/shaders/simple.frag");
     Shader skyboxShader("../assets/shaders/skybox.vert", "../assets/shaders/skybox.frag");
     Shader modelShader("../assets/shaders/model.vert", "../assets/shaders/model.frag");
     Shader pointShader("../assets/shaders/points.vert", "../assets/shaders/points.frag");
     Shader instanceShader("../assets/shaders/instance.vert", "../assets/shaders/instance.frag");

     pointShader.LinkGeometry("../assets/shaders/points.geom");
     // textures
     TextureUnit cubeTexture("../container.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
     TextureUnit floorTexture("../marble.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
     // cubemap texture
     unsigned int texID;
     glGenTextures(1, &texID);
     glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
     // models
     Model asteroids("../assets/rock/rock.obj");
     
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
     // octrahedron geometry
     VAO octaVAO;
     VBO octaVBO(octahedronVertices, sizeof(octahedronVertices));
     octaVAO.Bind();
     octaVAO.LinkAttrib(octaVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
     octaVAO.Unbind();
     // create transformation matrices
     unsigned int amount = 1000;
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
          float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
          model = glm::scale(model, glm::vec3(scale));

          // rotation
          float rotAngle = static_cast<float>((rand() % 360));
          model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

          // add to the list of matrices that we have
          modelMatrices[i] = model;
     }

     VBO buffer(&modelMatrices[0][0][0], amount * sizeof(glm::mat4));

     for(unsigned int i = 0; i < asteroids.meshes.size(); i++)
     {
        unsigned int VAO = asteroids.meshes[i].VAO;
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

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
     }

     // frame buffer
     unsigned int fbo;
     glGenFramebuffers(1, &fbo);
     glBindFramebuffer(GL_FRAMEBUFFER, fbo);

     // texture for frame buffer
     unsigned int texture;
     glGenTextures(1, &texture);
     glBindTexture(GL_TEXTURE_2D, texture);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 800, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
     glBindTexture(GL_TEXTURE_2D, 0); 
     // render buffer
     unsigned int rbo;
     glGenRenderbuffers(1, &rbo);
     glBindRenderbuffer(GL_RENDERBUFFER, rbo);
     glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 800);
     glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
     glBindRenderbuffer(GL_RENDERBUFFER, 0);

     if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
          std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
     glBindFramebuffer(GL_FRAMEBUFFER, 0);
     // camera object
     Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
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

     glEnable(GL_DEPTH_TEST); // Allows for depth comparison and updates the depth buffer
    // glEnable(GL_BLEND); // enable alpha blending
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

     glm::vec3 lightPos(0.5f, 0.5f, 0.0f);

     float deltaTime = 0.0f;
     float lastFrame = 0.0f;
     
     // cubemap textures
     std::vector<std::string> cubemaps = {
          "../assets/skybox/left.jpg",
          "../assets/skybox/right.jpg",
          "../assets/skybox/bottom.jpg",
          "../assets/skybox/top.jpg",
          "../assets/skybox/back.jpg",
          "../assets/skybox/front.jpg"
     };

     std::vector<std::string> stylizedMaps = {
          "../assets/stylizedSkybox/nx.png",
          "../assets/stylizedSkybox/px.png",
           "../assets/stylizedSkybox/ny.png",
           "../assets/stylizedSkybox/py.png",
          "../assets/stylizedSkybox/nz.png",
          "../assets/stylizedSkybox/pz.png"
     };

     std::vector<std::string> yokohamaMaps = {
          "../assets/Yokohama3/negx.jpg",
          "../assets/Yokohama3/posx.jpg",
          "../assets/Yokohama3/negy.jpg",
          "../assets/Yokohama3/posy.jpg",
          "../assets/Yokohama3/negz.jpg",
          "../assets/Yokohama3/posz.jpg"
     };
     unsigned int cubemapTexture = LoadCubeMap(yokohamaMaps);

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
          glBindFramebuffer(GL_FRAMEBUFFER, fbo);
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

          // drawing octahedron
          glm::mat4 model = glm::mat4(1.0f);
          instanceShader.Activate();
          instanceShader.SetToMat4("model", model);
          octaVAO.Bind();
          glDrawArrays(GL_TRIANGLES, 0, 24);
          octaVAO.Unbind();

          // drawing asteroids
          /*for(unsigned int i = 0; i < asteroids.meshes.size(); i++)
          {
               glBindVertexArray(asteroids.meshes[i].VAO);
               glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(asteroids.meshes[i].indices.size()), GL_UNSIGNED_INT, 0, amount);
               glBindVertexArray(0);
          }*/
          
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

          glBindFramebuffer(GL_FRAMEBUFFER, 0);
          glDisable(GL_DEPTH_TEST);
          glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
          glClear(GL_COLOR_BUFFER_BIT);

          simpleShader.Activate();
          quadVAO.Bind();
          glBindTexture(GL_TEXTURE_2D, texture);
          glDrawArrays(GL_TRIANGLES, 0, 6);
          quadVAO.Unbind();
          
          // GUI STUFF
          ImGui::Begin("OpenGL Settings Panel");
          ImGui::Text("Tweaks");
     
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
     glDeleteRenderbuffers(1, &rbo);
     glDeleteFramebuffers(1, &fbo);
     glDeleteBuffers(1, &ubo);
     
     glfwTerminate();
     return 0;
}
