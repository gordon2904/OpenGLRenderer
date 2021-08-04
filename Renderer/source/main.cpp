/*
* 
* Models downloaded from Morgan McGuire's Computer Graphics Archive https://casual-effects.com/data
* 
*/

#include "utils/logger/Logger.h"
#include "utils/glutils/GLUtils.h"
#include<iostream>
#include<sstream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"glClasses/GLEntity.h"
#include"glClasses/GLMultiEntity.h"
#include"glClasses/Material.h"
#include"glClasses/Textures/Texture2D.h"
#include"glClasses/Textures/TextureCube.h"
#include"glClasses/Shader.h"
#include"glClasses/Camera.h"
#include"glClasses/FrameBuffer.h"

#include "glClasses/glLights/GLSpotLight.h"
#include "glClasses/glLights/GLDirectionalLight.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glClasses/Model.h"

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

std::vector<std::shared_ptr<FrameBuffer>> frameBuffers;

glm::mat4 orthographicProjection = glm::ortho(0.0f, (float)SCREEN_WIDTH, 0.0f, (float)SCREEN_HEIGHT, 0.1f, 100.f);
glm::mat4 perspectiveProjection = glm::perspective(glm::radians(camera.getFov()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);

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

float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
};
const unsigned int quadDataSize = sizeof(quadVertices);


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
const unsigned int skyboxDataSize = sizeof(skyboxVertices);

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
   
   std::vector<VertexAttribute> skyboxVertexAttributes = {
      VertexAttribute(3, GL_FLOAT), // position
   };

   std::vector<VertexAttribute> quadVertexAttributes = {
      VertexAttribute(2, GL_FLOAT), // position
      VertexAttribute(2, GL_FLOAT)  // tex coord
   };


   std::shared_ptr<Shader> lightShader = std::make_shared<Shader>("light");
   std::shared_ptr<Material> lightMaterial = std::make_shared<Material>(lightShader);

   glm::mat4 lightModel(1);
   lightModel = glm::scale(lightModel, glm::vec3(0.2f));


   std::vector<std::shared_ptr<GLDirectionalLight>> directionalLights;
   std::vector<std::shared_ptr<GLSpotLight>> spotLights;
   std::vector<std::shared_ptr<GLPointLight>> pointLights;
   glm::vec3 pointLightPositions[] = {
      glm::vec3(0.7f,  0.2f,  2.0f),
      glm::vec3(2.3f, -3.3f, -4.0f),
      glm::vec3(-4.0f,  2.0f, -12.0f),
      glm::vec3(0.0f,  0.0f, -3.0f)
   };   
   glm::vec3 pointLightColours[] = {
      glm::vec3(0.7f,  0.2f,  1.0f),
      glm::vec3(0.2f, 0.0f, 0.8f),
      glm::vec3(0.2f, 0.9f, 0.0f),
      glm::vec3(1.0f, 0.2f, 0.0f),
   };
   for(int i = 0; i < 4; ++i)
   {
      const std::shared_ptr<GLPointLight> pointLight = std::make_shared<GLPointLight>(cubeVertices, cubeDataSize, cubeVertexAttributes);
      pointLight->setMaterial(lightMaterial);
      pointLight->setModelMat(glm::translate(lightModel, pointLightPositions[i]));
      pointLight->setAmbientColour(pointLightColours[i] * 0.1f);
      pointLight->setDiffuseColour(pointLightColours[i]);
      pointLight->setSpecularColour(pointLightColours[i]);
      pointLights.push_back(pointLight);
   }

   //std::shared_ptr<GLEntity> glLight = std::make_shared<GLEntity>(cubeVertices, cubeDataSize, cubeVertexAttributes);
   //glLight->setMaterial(lightMaterial);
   //glLight->setModelMat(lightModel);


   std::function<void(glm::mat4&, std::shared_ptr<Shader>, const float&, const float& delta)> litUpdateLambda = [&] (glm::mat4& model, std::shared_ptr<Shader> shader, const float& time, const float& delta) {

      //update point lights
      shader->setInt("pointLightsLength", pointLights.size());
      for(int i = 0; i < pointLights.size(); ++i)
      {
         const std::shared_ptr<GLPointLight> light = pointLights[i];
         std::stringstream stringStream;
         stringStream << "pointLights[" << i << "]";
         shader->setVec3(stringStream.str() + ".position", light->getPosition());
         shader->setFloat(stringStream.str() + ".constant", light->getConstant());
         shader->setFloat(stringStream.str() + ".linear", light->getLinear());
         shader->setFloat(stringStream.str() + ".quadratic", light->getQuadratic());
         shader->setVec3(stringStream.str() + ".ambient", light->getAmbientColour());
         shader->setVec3(stringStream.str() + ".diffuse", light->getDiffuseColour());
         shader->setVec3(stringStream.str() + ".specular", light->getSpecularColour());
      }

      //update direction lights
      shader->setInt("dirLightsLength", 0);
      for(int i = 0; i < 0; ++i)
      {
         const std::shared_ptr<GLDirectionalLight> light = directionalLights[i];
         std::stringstream stringStream;
         stringStream << "dirLights[" << i << "]";
         shader->setVec3(stringStream.str() + ".direction", light->getDirection());
         shader->setVec3(stringStream.str() + ".ambient", light->getAmbientColour());
         shader->setVec3(stringStream.str() + ".diffuse", light->getDiffuseColour());
         shader->setVec3(stringStream.str() + ".specular", light->getSpecularColour());
      }

      //update spot lights
      shader->setInt("spotLightsLength", 0);
      for(int i = 0; i < 0; ++i)
      {
         const std::shared_ptr<GLSpotLight> light = spotLights[i];
         std::stringstream stringStream;
         stringStream << "spotlights[" << i << "]";
         shader->setVec3(stringStream.str() + ".position", light->getPosition());
         shader->setVec3(stringStream.str() + ".direction", light->getDirection());
         shader->setFloat(stringStream.str() + ".cutOff", light->getCutOff());
         shader->setFloat(stringStream.str() + ".outerCutOff", light->getOuterCutOff());
         shader->setFloat(stringStream.str() + ".constant", light->getConstant());
         shader->setFloat(stringStream.str() + ".linear", light->getLinear());
         shader->setFloat(stringStream.str() + ".quadratic", light->getQuadratic());
         shader->setVec3(stringStream.str() + ".ambient", light->getAmbientColour());
         shader->setVec3(stringStream.str() + ".diffuse", light->getDiffuseColour());
         shader->setVec3(stringStream.str() + ".specular", light->getSpecularColour());
      }

      float angle = 20.0f;
      //model = glm::rotate(model, glm::radians(deltaTime * angle), glm::vec3(1.0f, 0.3f, 0.5f));

      shader->setFloat("time", time);
      shader->setFloat("material.shininess", 32.0f);
      shader->setMat3("normalMat", glm::mat3(glm::transpose(glm::inverse(model))));
      shader->setVec3("viewPos", camera.getPosition());
   };

   //std::shared_ptr<Shader> litShader = std::make_shared<Shader>("lit");
   //std::shared_ptr<Material> litMaterial = std::make_shared<Material>(litShader); 
   //litMaterial->getShader()->use();
   //litMaterial->setTexture("material.diffuse", boxDiffuseTex, GL_TEXTURE0);
   //litMaterial->setTexture("material.specular", boxSpecularTex, GL_TEXTURE0 + 1);
   //litMaterial->setTexture("material.emission", boxEmissiveTex, GL_TEXTURE0 + 2);

   //std::shared_ptr<GLMultiEntity> glLitCube = std::make_shared <GLMultiEntity> (cubeVertices, cubeDataSize, cubeVertexAttributes);
   //glLitCube->setModelCount(10);
   //glLitCube->setMaterial(litMaterial);
   //glLitCube->setUpdateLambda(litUpdateLambda);

   //std::shared_ptr<GLLight> lights[] = { pointLights[0] };
   //std::shared_ptr<GLDrawable> entities[] = { glLight, glLitCube };


   const char* modelPath = "assets/backpack/backpack.obj";
   //const char* modelPath = "assets/sponza/sponza.obj";
   std::shared_ptr<Shader> debugShader = std::make_shared<Shader>("debug");
   std::shared_ptr<Shader> unlitShader = std::make_shared<Shader>("lit-model");
   std::shared_ptr<Model> model = std::make_shared<Model>(modelPath);
   model->setUpdateLambda(litUpdateLambda);
   model->setShader(unlitShader);

   std::shared_ptr<Shader> screenShader = std::make_shared<Shader>("post-processing/standard");
   std::shared_ptr<Material> screenMaterial = std::make_shared<Material>(screenShader);
   std::shared_ptr<FrameBuffer> frameBuffer = std::make_shared<FrameBuffer>(SCREEN_WIDTH, SCREEN_HEIGHT, true);
   frameBuffers.push_back(frameBuffer);
   screenMaterial->setTexture("screenTexture", frameBuffer->getTexture());
   std::shared_ptr<GLEntity> screenQuad = std::make_shared<GLEntity>(quadVertices, quadDataSize, quadVertexAttributes);
   screenQuad->setMaterial(screenMaterial);

   std::string faces[]
   {
      "skybox/right.jpg",
      "skybox/left.jpg",
      "skybox/top.jpg",
      "skybox/bottom.jpg",
      "skybox/front.jpg",
      "skybox/back.jpg"
   };

   std::shared_ptr<Texture> skyboxTexture = std::make_shared<TextureCube>(faces);
   std::shared_ptr<Shader> skyboxShader = std::make_shared<Shader>("skybox");
   std::shared_ptr<Material> skyboxMaterial = std::make_shared<Material>(skyboxShader);
   skyboxMaterial->setTexture("skybox", skyboxTexture);
   std::shared_ptr<GLEntity> skybox = std::make_shared<GLEntity>(skyboxVertices, skyboxDataSize, skyboxVertexAttributes);
   skybox->setMaterial(skyboxMaterial);

   const float radius = 10.0f;

   // draw as wireframe
   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   while(!glfwWindowShouldClose(window))
   {
      processInput(window);

      const float time = (float)glfwGetTime();
      deltaTime = time - lastFrame;
      lastFrame = time;

      glm::mat4 view = camera.GetViewMatrix();
      glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
      RenderInputs renderInputs =
      {
         nullptr,                //std::shared_ptr<Material> overrideMaterial;
         time,                   //float time;
         view,             //glm::mat4 view;
         perspectiveProjection   //glm::mat4 projection;
      };

      frameBuffer->bindFrameBuffer();
      //glClearColor(0.0f, 0.3f, 0.9f, 1.0f);


      glEnable(GL_DEPTH_TEST);
      glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
      glStencilMask(0x00); //stencil mask off by default

      //update lights
      //for(auto light : pointLights)
      //{
      //   light->Render(renderInputs);
      //}

      ////render scene
      //for(auto entity : entities)
      //{
      //   entity->Render(renderInputs);
      //}

      model->setShader(unlitShader); 
      glStencilFunc(GL_ALWAYS, 1, 0xFF);
      glStencilMask(0xFF);
      glm::mat4 mat(1);
      model->setModelMat(mat);
      model->render(time, deltaTime, view, perspectiveProjection);

      glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
      glStencilMask(0x00);
      glDisable(GL_DEPTH_TEST);
      mat = glm::scale(mat, glm::vec3(1 / 1.1));
      model->setModelMat(mat);
      model->setShader(debugShader);
      model->render(time, deltaTime, view, perspectiveProjection);

      glStencilMask(0xFF);
      glStencilFunc(GL_ALWAYS, 1, 0xFF);
      glEnable(GL_DEPTH_TEST);


      renderInputs.view = skyboxView;
      skybox->render(renderInputs);

      frameBuffer->unbindFrameBuffer();

      glDepthFunc(GL_LEQUAL);
      glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      //disable depth test to write directly to the screen and disregard recorded depth data
      glDisable(GL_DEPTH_TEST);
      //draw quad to screen using the frame buffer texture
      screenQuad->render(renderInputs);

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
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_STENCIL_TEST);
   glEnable(GL_CULL_FACE);
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
   perspectiveProjection = glm::perspective(glm::radians(camera.getFov()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);
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
   for(auto& frameBuffer : frameBuffers)
   {
      frameBuffer->resize(width, height);
   }
   orthographicProjection = glm::ortho(0.0f, (float)SCREEN_WIDTH, 0.0f, (float)SCREEN_HEIGHT, 0.1f, 100.f);
   perspectiveProjection = glm::perspective(glm::radians(camera.getFov()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);
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

   std::shared_ptr<Texture> wallTexture = std::make_shared<Texture2D>("wall.jpg", GLPixelDataFormat::RGB);
   std::shared_ptr<Texture> faceTexture = std::make_shared<Texture2D>("awesomeface.png", GLPixelDataFormat::RGBA, true);

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