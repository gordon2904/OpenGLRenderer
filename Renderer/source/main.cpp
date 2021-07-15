/*
* 
* Models downloaded from Morgan McGuire's Computer Graphics Archive https://casual-effects.com/data
* 
*/

#include "utils/logger/Logger.h"
#include "utils/shaders/Shaders.h"
#include "utils/glutils/GLUtils.h"
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"glClasses/GLEntity.h"
#include"glClasses/Shader.h"

GLFWwindow* window = nullptr;
const unsigned int INITIAL_SCREEN_WIDTH = 800;
const unsigned int INITIAL_SCREEN_HEIGHT = 600;

//triangle with vertices in counter clockwise order starting from bottom left
const float triangleVertices[] = {
   -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f,  0.75f, 0.0f
};
const unsigned int triangleVerticesLength = sizeof(triangleVertices) / sizeof(float);

const float rectangleVertices[] = {
  0.5f,  0.5f, 0.0f,  // top right
  0.5f, -0.5f, 0.0f,  // bottom right
 -0.5f, -0.5f, 0.0f,  // bottom left
 -0.5f,  0.5f, 0.0f   // top left 
};
const unsigned int rectangleVerticesLength = sizeof(rectangleVertices) / sizeof(float);
const unsigned int rectangleIndices[] = {
 0, 1, 3,   // first triangle
 1, 2, 3    // second triangle
};
const unsigned int rectangleIndicesLength = sizeof(rectangleIndices) / sizeof(unsigned int);

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

int loadShader(GLuint& shader, const char* source, GLShaderType shaderType, const char* shaderName = nullptr)
{
   shader = glCreateShader(shaderType);
   const bool hasShaderName = shaderName != nullptr;

   glShaderSource(shader, 1, &source, nullptr);
   glCompileShader(shader);

   int  success;
   char infoLog[512];
   glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
   if(!success)
   {
      glGetShaderInfoLog(shader, 512, nullptr, infoLog);
      if(hasShaderName)
      {
         LOG_ERROR("Failed to compile shader: {0}\n"
            "{1}\n"
            "infoLog: {2}",
            shaderName, source, infoLog);
      }
      else
      {
         LOG_ERROR("Failed to compile shader: \n"
            "{0}\n"
            "infoLog: {1}",
            source, infoLog);
      }
      return 0;
   }
   if(hasShaderName)
   {
      LOG_INFO("Succesfully compiled shader: {0}", shaderName);
   }
   else
   {
      LOG_INFO("Succesfully compiled shader");
   }
   return 1;
}

int initializeShaderProgram(GLuint& shaderProgram, const char* vertexShaderSrc, const char* fragShaderSrc)
{
   GLuint vertexShader;
   GLuint fragmentShader;
   if(!loadShader(vertexShader, vertexShaderSrc, GLShaderType::VERTEX_SHADER, "vertex shader"))
   {
      return 0;
   }
   if(!loadShader(fragmentShader, fragShaderSrc, GLShaderType::FRAGMENT_SHADER, "fragment shader"))
   {
      return 0;
   }
   shaderProgram = glCreateProgram();
   glAttachShader(shaderProgram, vertexShader);
   glAttachShader(shaderProgram, fragmentShader);
   glLinkProgram(shaderProgram);


   int  success;
   char infoLog[512];
   glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
   if(!success)
   {
      glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
      LOG_ERROR("Failed to link shader program\n"
         "GLuint: {0}\n"
         "infoLog: {1}",
         shaderProgram, infoLog);
      return 0;
   }
      LOG_INFO("Succesfully linked shader program: {0}", shaderProgram);
   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);
   return 1;
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
   glfwSetFramebufferSizeCallback(window, [] (GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); });

   float data[] = {
      // positions         // colors
       0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
      -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
       0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
   };

   std::vector<VertexAttribute> triangleVertexAttributes = {
      VertexAttribute(3, GL_FLOAT),
      VertexAttribute(3, GL_FLOAT)
   };

   std::vector<VertexAttribute> rectangleVertexAttributes = {
      VertexAttribute(3, GL_FLOAT)
   };

   std::shared_ptr<Shader> standardShader = std::make_shared<Shader>("standard");
   std::shared_ptr<GLEntity> glTriangle = std::make_shared<GLEntity>(data, 3, triangleVertexAttributes);
   glTriangle->setUpdateLambda([=] () {
      // update the uniform color
      float timeValue = (float) glfwGetTime();
      float greenValue = sin(timeValue) / 2.0f + 0.5f;
      float vec4[] = { 0, greenValue, 0, 1 };
      standardShader->setVec4("ourColor", vec4);
      standardShader->setFloat("offset", greenValue);
   });

   std::shared_ptr<GLEntity> glRectangle = std::make_shared<GLEntity>((void*)rectangleVertices, rectangleVerticesLength, rectangleVertexAttributes, (void*)rectangleIndices, rectangleIndicesLength);
   glTriangle->setShaderProgram(standardShader);
   std::shared_ptr<GLEntity> entities[] = { glTriangle, glRectangle };

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