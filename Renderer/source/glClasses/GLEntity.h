#ifndef GLENTITY_H
#define GLENTITY_H
class GLEntity
{
protected:
   unsigned int vao;
   unsigned int vbo;
   bool mVisible;
   GLEntity();
public:
   ~GLEntity();
   int virtual Render(const unsigned int &shaderProgram);
   void setVisible(const bool& visible);
};
#endif