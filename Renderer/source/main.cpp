/*
* 
* Models downloaded from Morgan McGuire's Computer Graphics Archive https://casual-effects.com/data
* 
*/

#include "utils/logger/Logger.h"
#include "utils/shaders/Shaders.h"
#include "utils/glenums/GLEnums.h"
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"glClasses/GLEntityTriangle.h"
#include"glClasses/GLEntityPolygon.h"

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
   int maxVertexAttributes;
   glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttributes);
   LOG_INFO("Max number of vertex attributes supported: {0}", maxVertexAttributes);


   GLuint shaderProgram;   
   if(!initializeShaderProgram(shaderProgram, Shaders::uniforms::vertexShader, Shaders::uniforms::fragmentShader))
   {
      return -1;
   }

   GLuint overrideShaderProgram;
   if(!initializeShaderProgram(overrideShaderProgram, Shaders::standard::vertexShader, Shaders::standard::fragmentShader))
   {
      return -1;
   }

   //make some triangles and rectangles
   std::shared_ptr<GLEntityTriangle> glTriangle = std::make_shared<GLEntityTriangle>((float*)triangleVertices, triangleVerticesLength);
   glTriangle->setShaderProgram(shaderProgram);
   glTriangle->setUpdateLambda([=] () 
      {
         unsigned int shaderProgram = glTriangle->getShaderProgram();
         if(shaderProgram == 0)
         {
            LOG_WARN("glTriangle's shaderProgram member variable has not been set");
            return;
         }
         float timeValue = (float)glfwGetTime();
         float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
         int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
         glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
      });
   std::shared_ptr<GLEntityPolygon> glRectangle = std::make_shared<GLEntityPolygon>((float*)rectangleVertices, rectangleVerticesLength, (unsigned int*)rectangleIndices, rectangleIndicesLength);
   std::shared_ptr<GLEntity> entities[] = { glTriangle, glRectangle } ;

   while(!glfwWindowShouldClose(window))
   {
      processInput(window);

      //rendering
      glClearColor(0.0f, 0.3f, 0.9f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      for(auto entity : entities)
      {
         entity->Render();
         //entity->Render(overrideShaderProgram);
      }
      glBindVertexArray(0);

      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   glDeleteProgram(shaderProgram);
   glDeleteProgram(overrideShaderProgram);


   glfwTerminate();
   return 0;
}