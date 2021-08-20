#pragma once
#include "glDrawables/GLDrawable.h"
#include "Textures/TextureCube.h"
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
   Mesh();
   void loadMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures);
   virtual ~Mesh();
   void draw(const Shader* shader, TextureCube* skybox)
   {
      glBindVertexArray(VAO);
      setTextures(shader, skybox);
      glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
   }
protected:
   std::vector<Vertex>        vertices;
   std::vector<unsigned int>  indices;
   std::vector<MeshTexture> textures;
   unsigned int VAO, VBO, EBO;

   void setTextures(const Shader* shader, TextureCube* skybox);
   void setupMesh();

public:
};