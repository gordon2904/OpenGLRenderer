#pragma once
#include "glDrawables/GLDrawable.h"
#include "Texture.h"
#include <vector>


struct MeshTexture
{
   std::shared_ptr<Texture> texture;
   std::string type;
   std::string path;
};

struct Vertex
{
   glm::vec3 Position;
   glm::vec3 Normal;
   glm::vec2 TexCoords;
   glm::vec3 Tangent;
   glm::vec3 Bitangent;
};

class Mesh
{
public:
   Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures);
   virtual ~Mesh();
   void Draw(std::shared_ptr<Shader> shader)
   {
      glBindVertexArray(VAO);
      SetTextures(shader);
      glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
   }
protected:
   std::vector<Vertex>        vertices;
   std::vector<unsigned int>  indices;
   std::vector<MeshTexture> textures;
   unsigned int VAO, VBO, EBO;

   void SetTextures(std::shared_ptr<Shader> shader);
   void setupMesh();

public:
};