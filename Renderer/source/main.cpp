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
#include"glClasses/GLMultiEntity.h"
#include"glClasses/Material.h"
#include"glClasses/Texture.h"
#include"glClasses/Shader.h"
#include"glClasses/Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float SinLerp(const float& t);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window); 
int initializeGLFWwindow();
int initializeGLAD();

GLFWwindow* window = nullptr;

//rendering shindig
const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 100.0f;
unsigned int SCREEN_WIDTH = 800;
unsigned int SCREEN_HEIGHT = 600;

float lastX = (float)SCREEN_WIDTH * 0.5f;
float lastY = (float)SCREEN_HEIGHT * 0.5f;
bool firstMouse = false;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//tracks time
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

glm::mat4 orthographicProjection = glm::ortho(0.0f, (float)SCREEN_WIDTH, 0.0f, (float)SCREEN_HEIGHT, 0.1f, 100.f);
glm::mat4 perspectiveProjection = glm::perspective(glm::radians(camera.fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};


// vec3        vec3     vec2
// positions   normals  uvs
float cubeVertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
                                              
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
                                              
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
                                              
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
                                              
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
                                              
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};
const unsigned int cubeDataSize = sizeof(cubeVertices);

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

int main(int argc, char** argv)
{
   if(!initializeGLFWwindow()) {
      return -1;
   }   
   if(!initializeGLAD()) {
      return -1;
   }

   std::vector<VertexAttribute> cubeVertexAttributes = {
      VertexAttribute(3, GL_FLOAT), // position
      VertexAttribute(3, GL_FLOAT), // normals
      VertexAttribute(2, GL_FLOAT)  // tex coord
   };

   std::shared_ptr<Shader> lightShader = std::make_shared<Shader>("light");
   std::shared_ptr<Material> lightMaterial = std::make_shared<Material>(lightShader);
   lightMaterial->use();
   lightMaterial->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
   lightMaterial->setVec3("lightColor", 1.0f, 1.0f, 1.0f);

   glm::mat4 lightModel(1);
   lightModel = glm::translate(lightModel, glm::vec3(1.2f, 1.0f, 2.0f));
   lightModel = glm::scale(lightModel, glm::vec3(0.2f));

   std::shared_ptr<GLEntity> glLight = std::make_shared<GLEntity>(cubeVertices, cubeDataSize, cubeVertexAttributes);
   glLight->setMaterial(lightMaterial);
   glLight->setModelMat(lightModel);


   std::function<void(glm::mat4&, std::shared_ptr<Material>, const float&)> litUpdateLambda = [&] (glm::mat4& model, std::shared_ptr<Material> material, const float& time) {
      glm::vec3 lightColor;
      lightColor.x = SinLerp(time * 2.0f);
      lightColor.y = SinLerp(time * 0.7f);
      lightColor.z = SinLerp(time * 1.3f);

      glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
      glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

      const float clampedT = 0.5f + (sin(time) * 0.5f);

      material->setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
      material->setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
      material->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
      material->setFloat("material.shininess", 32.0f);
      material->setVec3("light.position", glm::vec3((*glLight->getModelMat())[3]));
      material->setVec3("light.ambient", ambientColor);
      material->setVec3("light.diffuse", diffuseColor); // darken diffuse light a bit
      material->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
      material->setMat3("normalMat", glm::mat3(glm::transpose(glm::inverse(model))));
      material->setVec3("viewPos", camera.Position);
   };
   std::shared_ptr<Shader> litShader = std::make_shared<Shader>("lit");
   std::shared_ptr<Material> litMaterial = std::make_shared<Material>(litShader);

   std::shared_ptr<GLEntity> glLitCube = std::make_shared<GLEntity>(cubeVertices, cubeDataSize, cubeVertexAttributes);
   glLitCube->setMaterial(litMaterial);
   glLitCube->setUpdateLambda(litUpdateLambda);

   std::shared_ptr<GLDrawable> entities[] = { glLight, glLitCube };

   const float radius = 10.0f;
   while(!glfwWindowShouldClose(window))
   {
      processInput(window);

      glClearColor(0.0f, 0.3f, 0.9f, 1.0f); 
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      const float time = (float)glfwGetTime();
      deltaTime = time - lastFrame;
      lastFrame = time;

      glm::mat4 view = camera.GetViewMatrix();
      RenderInputs renderInputs =
      {
         nullptr,                //std::shared_ptr<Material> overrideMaterial;
         time,                   //float time;
         view,                   //glm::mat4 view;
         perspectiveProjection   //glm::mat4 projection;
      };

      for(auto entity : entities)
      {
         entity->Render(renderInputs);
      }

      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   glfwTerminate();
   return 0;
}

int initializeGLFWwindow()
{
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Renderer", nullptr, nullptr);
   if(window == nullptr)
   {
      LOG_ERROR("Failed to create GLFW window");
      glfwTerminate();
      return 0;
   }
   LOG_INFO("initialized GLFW window succesfully");
   glfwMakeContextCurrent(window);
   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
   glfwSetCursorPosCallback(window, mouse_callback);
   glfwSetScrollCallback(window, scroll_callback);
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
   glEnable(GL_DEPTH_TEST);
   glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
   return 1;
}

void processInput(GLFWwindow* window)
{
   if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

   const bool isShifting = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
   if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime, isShifting);
   if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime, isShifting);
   if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime, isShifting);
   if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime, isShifting);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
   camera.ProcessMouseScroll((float)yoffset);
   perspectiveProjection = glm::perspective(glm::radians(camera.fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
   float xPosF = (float)xpos;
   float yPosF = (float)ypos;
   if(firstMouse)
   {
      lastX = (float)xPosF;
      lastY = (float)yPosF;
      firstMouse = false;
   }
   float xoffset = xPosF - lastX;
   float yoffset = lastY - yPosF;
   lastX = (float)xPosF;
   lastY = (float)yPosF;
   camera.ProcessMouseMovement(xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
   SCREEN_WIDTH = width;
   SCREEN_HEIGHT = height;
   orthographicProjection = glm::ortho(0.0f, (float)SCREEN_WIDTH, 0.0f, (float)SCREEN_HEIGHT, 0.1f, 100.f);
   perspectiveProjection = glm::perspective(glm::radians(camera.fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);
   glViewport(0, 0, width, height);
}


float SinLerp(const float& t)
{
   return 0.5f + (sin(t) * 0.5f);
}

//OLD USED GLEntities and materials
/*

   std::vector<VertexAttribute> rectangleVertexAttributes = {
   VertexAttribute(3, GL_FLOAT), // position
   VertexAttribute(3, GL_FLOAT), // vertex colour
   VertexAttribute(2, GL_FLOAT)  // tex coord
   };

   std::shared_ptr<Texture> wallTexture = std::make_shared<Texture>("wall.jpg", GLPixelDataFormat::RGB);
   std::shared_ptr<Texture> faceTexture = std::make_shared<Texture>("awesomeface.png", GLPixelDataFormat::RGBA, true);

   //material for rectangle
   std::shared_ptr<Shader> standardShader = std::make_shared<Shader>("standard");
   std::shared_ptr<Material> standardMaterial = std::make_shared<Material>(standardShader);
   standardMaterial->setTexture("texture1", wallTexture, GL_TEXTURE0);
   standardMaterial->setTexture("texture2", faceTexture, GL_TEXTURE0+1);

   //material for cube
   std::shared_ptr<Shader> cubeShader = std::make_shared<Shader>("cube");
   std::shared_ptr<Material> cubeMaterial = std::make_shared<Material>(cubeShader);
   cubeMaterial->setTexture("texture1", wallTexture, GL_TEXTURE0);
   cubeMaterial->setTexture("texture2", faceTexture, GL_TEXTURE0 + 1);


   //update lambda to be used across rectangle and cube
   std::function<void(glm::mat4&, std::shared_ptr<Material>, const float&)> updateLambda = [&](glm::mat4& localPos, std::shared_ptr<Material> material, const float& time) {
      localPos = glm::rotate(localPos, glm::radians(time * -55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
   };

   //update lambda to be used across GLMultiEntity
   std::function<void(glm::mat4&, std::shared_ptr<Material>, const float&, unsigned int)> updateMultiLambda = [&] (glm::mat4& localPos, std::shared_ptr<Material> material, const float& time, unsigned int index) {
      localPos = glm::mat4(1.0f);
      localPos = glm::translate(localPos, cubePositions[index]);
      float angle = 20.0f * index;
      localPos = glm::rotate(localPos, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
   };

   std::shared_ptr<GLEntity> glCube = std::make_shared<GLEntity>(cubeVertices, cubeDataSize, cubeVertexAttributes);
   glCube->setMaterial(cubeMaterial);
   glCube->setUpdateLambda(updateLambda);

   std::shared_ptr<GLMultiEntity> glMultiCube = std::make_shared<GLMultiEntity>(cubeVertices, cubeDataSize, cubeVertexAttributes);
   glMultiCube->setModelCount(10);
   glMultiCube->setMaterial(cubeMaterial);
   glMultiCube->setUpdateLambda(updateMultiLambda);

   std::shared_ptr<GLEntity> glRectangle = std::make_shared<GLEntity>(rectangleData, rectangleDataSize, rectangleVertexAttributes, rectangleIndices, rectangleIndicesLength);
   glRectangle->setMaterial(standardMaterial);
   glRectangle->setUpdateLambda(updateLambda);

*/