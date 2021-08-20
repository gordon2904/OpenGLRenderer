#pragma once
#include "Shader.h"
#include "Mesh.h"
#include <memory>
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Textures/TextureCube.h"


class Model
{
public:
   Model(const char* path, bool flipTextures = true) : flipTextures(flipTextures), model(glm::mat4(1)), updateLambda([] (glm::mat4&, const Shader*, const float&, const float&) {})
   {
      loadModel(path);
   }
   void setModelMat(const glm::mat4 model);
   glm::mat4* getModelMat();
   void setUpdateLambda(std::function<void(glm::mat4&, const Shader*, const float&, const float&)> updateLambda);
   glm::vec3 getPosition() const;
   int render(float time, float delta, glm::mat4 view, glm::mat4 projection, const Shader* overrideShader = nullptr);
private:
   // model data
   bool flipTextures;
   std::vector<std::shared_ptr<Mesh>> meshes;
   std::string directory; 
   std::vector<MeshTexture> textures_loaded;

   void loadModel(std::string path);
   void processNode(aiNode* node, const aiScene* scene);
   void processMesh(aiMesh* mesh, const aiScene* scene);
   std::vector<MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
protected:
   TextureCube* cubemap;
   Shader* shader;
   bool visible;
   std::function<void(glm::mat4&, const Shader*, const float&, const float&)> updateLambda;
   glm::mat4 model;
   void updateModelViewProjection(const Shader* shader, glm::mat4& view, glm::mat4& projection);

public:
   void setVisible(const bool& visible)
   {
      this->visible = visible;
   }
   void setShader(Shader* shader)
   {
      this->shader = shader;
   }
   const Shader* useShader(const Shader* overrideShader)
   {
      if(overrideShader != nullptr)
      {
         overrideShader->use();
         return overrideShader;
      }
      else if(shader != nullptr)
      {
         shader->use();
         return shader;
      }
      return nullptr;
   }

   void setCubeMap(TextureCube* cubemap)
   {
      this->cubemap = cubemap;
   }
};