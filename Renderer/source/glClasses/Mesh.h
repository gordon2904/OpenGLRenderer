#pragma once
#include "glDrawables/GLDrawable.h"
#include "Textures/Texture.h"
#include <vector>


struct MeshTexture
{
   std::shared_ptr<Texture> texture;
   std::string type;
   std::string path;
};

struct Vertex
{
   glm::vec3 position;
   glm::vec3 normal;
   glm::vec2 texCoords;
   glm::vec3 tangent;
   glm::vec3 bitangent;
};

class Mesh
{
public:
   Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures);
   virtual ~Mesh();
   void draw(std::shared_ptr<Shader> shader)
   {
      glBindVertexArray(VAO);
      setTextures(shader);
      glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
   }
protected:
   std::vector<Vertex>        vertices;
   std::vector<unsigned int>  indices;
   std::vector<MeshTexture> textures;
   unsigned int VAO, VBO, EBO;

   void setTextures(std::shared_ptr<Shader> shader);
   void setupMesh();

public:
};