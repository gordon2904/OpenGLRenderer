#pragma once

namespace Shaders
{
   namespace standard {
      const char* vertexShader = "#version 330 core\n"
         "layout (location = 0) in vec3 aPos;\n"
         "void main()\n"
         "{\n"
         "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
         "}\0";

      const char* fragmentShader = "#version 330 core\n"
         "out vec4 FragColor;\n"
         "void main()\n"
         "{\n"
         "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
         "}\0";
   }

   namespace vertexColor
   {
      const char* vertexShader = "#version 330 core\n"
         "layout (location = 0) in vec3 aPos; // the position variable has attribute position 0\n"
         "out vec4 vertexColor; // specify a color output to the fragment shader\n"
         "void main()\n"
         "{\n"
         "  gl_Position = vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor\n"
         "  vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // set the output variable to a dark-red color\n"
         "}\0";

      const char* fragmentShader = "#version 330 core\n"
         "out vec4 FragColor;\n"
         "in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)\n"
         "void main()\n"
         "{\n"
         "   FragColor = vertexColor\n"
         "}\0";
   }

   namespace uniforms
   {
      const char* vertexShader = "#version 330 core\n"
         "layout (location = 0) in vec3 aPos; // the position variable has attribute position 0\n"
         "out vec4 vertexColor; // specify a color output to the fragment shader\n"
         "void main()\n"
         "{\n"
         "  gl_Position = vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor\n"
         "  vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // set the output variable to a dark-red color\n"
         "}\0";

      const char* fragmentShader = "#version 330 core\n"
         "out vec4 FragColor;\n"
         "uniform vec4 ourColor; // the input variable from the vertex shader (same name and same type)\n"
         "void main()\n"
         "{\n"
         "   FragColor = ourColor;\n"
         "}\0";
   }
}