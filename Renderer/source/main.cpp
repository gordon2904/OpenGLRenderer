/*
* 
* Models downloaded from Morgan McGuire's Computer Graphics Archive https://casual-effects.com/data
* 
*/

#include "utils/logger/Logger.h"
#include "utils/glutils/GLUtils.h"
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"glClasses/GLEntity.h"
#include"glClasses/Material.h"
#include"glClasses/Texture.h"
#include"glClasses/Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLFWwindow* window = nullptr;
const unsigned int INITIAL_SCREEN_WIDTH = 800;
const unsigned int INITIAL_SCREEN_HEIGHT = 600;

const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 100.0f;
const float FOV = 45.0f;

glm::mat4 orthographicProjection = glm::ortho(0.0f, (float)INITIAL_SCREEN_WIDTH, 0.0f, (float)INITIAL_SCREEN_HEIGHT, 0.1f, 100.f);
glm::mat4 perspectiveProjection = glm::perspective(glm::radians(FOV), (float)INITIAL_SCREEN_WIDTH / (float)INITIAL_SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);

float rectangleData[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};
const unsigned int rectangleDataSize = sizeof(rectangleData);

unsigned int rectangleIndices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};
unsigned int rectangleIndicesLength = sizeof(rectangleIndices) / sizeof(unsigned int);

int initializeGLFWwindow()
{
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   window = glfwCreateWindow(INITIAL_SCREEN_WIDTH, INITIAL_SCREEN_HEIGHT, "OpenGL Renderer", nullptr, nullptr);
   if(window == nullptr)
   {
      LOG_ERROR("Failed to create GLFW window");
      glfwTerminate();
      return 0;
   }
   LOG_INFO("initialized GLFW window succesfully");
   glfwMakeContextCurrent(window);
   return 1;
}

int initializeGLAD()
{
   if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
   {
      LOG_ERROR("Failed to initialize GLAD");
      return 0;
   }
   LOG_INFO("initialized GLAD succesfully");
   int maxVertexAttributes;
   glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttributes);
   LOG_INFO("Max number of vertex attributes supported: {0}", maxVertexAttributes);

   return 1;
}

void processInput(GLFWwindow* window)
{
   if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
}

int main(int argc, char** argv)
{
   if(!initializeGLFWwindow()) {
      return -1;
   }   
   if(!initializeGLAD()) {
      return -1;
   }

   glViewport(0, 0, 800, 600);
   glfwSetFramebufferSizeCallback(window, [] (GLFWwindow* window, int width, int height) { 
      orthographicProjection = glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.1f, 100.f);
      perspectiveProjection = glm::perspective(glm::radians(FOV), (float)width / (float)height, NEAR_PLANE, FAR_PLANE);
      glViewport(0, 0, width, height); 
      });

   glm::mat4 model = glm::mat4(1.0f);
   model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

   glm::mat4 view = glm::mat4(1.0f);
   // note that we're translating the scene in the reverse direction of where we want to move
   view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

   std::vector<VertexAttribute> rectangleVertexAttributes = {
      VertexAttribute(3, GL_FLOAT), // position
      VertexAttribute(3, GL_FLOAT), // vertex colour
      VertexAttribute(2, GL_FLOAT)  // tex coord
   };

   std::shared_ptr<Texture> wallTexture = std::make_shared<Texture>("wall.jpg", GLPixelDataFormat::RGB);
   std::shared_ptr<Texture> faceTexture = std::make_shared<Texture>("awesomeface.png", GLPixelDataFormat::RGBA, true);
   std::shared_ptr<Shader> standardShader = std::make_shared<Shader>("standard");
   std::shared_ptr<Material> standardMaterial = std::make_shared<Material>(standardShader);
   standardMaterial->setTexture("texture1", wallTexture, GL_TEXTURE0);
   standardMaterial->setTexture("texture2", faceTexture, GL_TEXTURE0+1);
   std::shared_ptr<GLEntity> glTriangle = std::make_shared<GLEntity>(rectangleData, rectangleDataSize, rectangleVertexAttributes, rectangleIndices, rectangleIndicesLength);
   glTriangle->setMaterial(standardMaterial);
   glTriangle->setUpdateLambda([=] () {
      float timeValue = (float)glfwGetTime();
      standardMaterial->setMat4("model", model);
      standardMaterial->setMat4("view", view);
      standardMaterial->setMat4("projection", perspectiveProjection);
   });
   std::shared_ptr<GLEntity> entities[] = { glTriangle };

   while(!glfwWindowShouldClose(window))
   {
      processInput(window);

      glClearColor(0.0f, 0.3f, 0.9f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      for(auto entity : entities)
      {
         entity->Render();
      }

      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   glfwTerminate();
   return 0;
}