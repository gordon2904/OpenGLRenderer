#pragma once
#include "Shader.h"
#include "Mesh.h"
#include <memory>
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class Model
{
public:
   Model(const char* path, bool flipTextures = true) : flipTextures(flipTextures), model(glm::mat4(1)), updateLambda([] (glm::mat4&, std::shared_ptr<Shader>, const float&, const float&) {})
   {
      loadModel(path);
   }
   void setModelMat(const glm::mat4 model);
   glm::mat4* getModelMat();
   void setUpdateLambda(std::function<void(glm::mat4&, std::shared_ptr<Shader>, const float&, const float&)> updateLambda);
   glm::vec3 getPosition() const;
   int render(float time, float delta, glm::mat4 view, glm::mat4 projection, std::shared_ptr<Shader> overrideShader = nullptr);
private:
   // model data
   bool flipTextures;
   std::vector<std::shared_ptr<Mesh>> meshes;
   std::string directory; 
   std::vector<MeshTexture> textures_loaded;

   void loadModel(std::string path);
   void processNode(aiNode* node, const aiScene* scene);
   std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
   std::vector<MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
protected:
   std::shared_ptr<Shader> shader;
   bool visible;
   std::function<void(glm::mat4&, std::shared_ptr<Shader>, const float&, const float&)> updateLambda;
   glm::mat4 model;
   void updateModelViewProjection(std::shared_ptr<Shader> shader, glm::mat4& view, glm::mat4& projection);

public:
   void setVisible(const bool& visible)
   {
      this->visible = visible;
   }
   void setShader(std::shared_ptr<Shader> shader)
   {
      this->shader = shader;
   }
   std::shared_ptr<Shader> useShader(const std::shared_ptr<Shader> overrideShader)
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
};