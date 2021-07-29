#include "Model.h"
#include "../utils/logger/Logger.h"

void Model::loadModel(std::string path)
{
   Assimp::Importer import;
   const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

   if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
   {
      LOG_ERROR("ASSIMP::{0}", import.GetErrorString());
      return;
   }
   directory = path.substr(0, path.find_last_of('/') + 1);

   processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
   // process all the node's meshes (if any)
   for(unsigned int i = 0; i < node->mNumMeshes; i++)
   {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(processMesh(mesh, scene));
   }
   // then do the same for each of its children
   for(unsigned int i = 0; i < node->mNumChildren; i++)
   {
      processNode(node->mChildren[i], scene);
   }
}

std::shared_ptr<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
   std::vector<Vertex> vertices;
   std::vector<unsigned int> indices;
   std::vector<MeshTexture> textures;

   for(unsigned int i = 0; i < mesh->mNumVertices; i++)
   {
      Vertex vertex;
      // process vertex positions, normals and texture coordinates
      vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
      vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
      if(mesh->HasTextureCoords(0))
      {
         vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
      }
      else
         vertex.TexCoords = glm::vec2(0.0f, 0.0f);
      if(mesh->HasTangentsAndBitangents())
      {
         LOG_INFO("has tangent and bitangents");
         vertex.Tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
         vertex.Bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
      }

      vertices.push_back(vertex);
   }
   // process indices
   for(unsigned int i = 0; i < mesh->mNumFaces; i++)
   {
      aiFace face = mesh->mFaces[i];
      for(unsigned int j = 0; j < face.mNumIndices; j++)
         indices.push_back(face.mIndices[j]);
   }
   // process material
   if(mesh->mMaterialIndex >= 0)
   {
      aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

      std::vector<MeshTexture> diffuseMaps = loadMaterialTextures(material,
         aiTextureType_DIFFUSE, "texture_diffuse");
      textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

      std::vector<MeshTexture> specularMaps = loadMaterialTextures(material,
         aiTextureType_SPECULAR, "texture_specular");
      textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

      std::vector<MeshTexture> ambientMaps = loadMaterialTextures(material,
         aiTextureType_AMBIENT, "texture_ambient");
      textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());

      std::vector<MeshTexture> heightMaps = loadMaterialTextures(material,
         aiTextureType_HEIGHT, "texture_height");
      textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

      std::vector<MeshTexture> normalMaps = loadMaterialTextures(material,
         aiTextureType_NORMALS, "texture_normals");
      textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

   }

   return std::make_shared<Mesh>(vertices, indices, textures);
}

std::vector<MeshTexture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
   std::vector<MeshTexture> textures;
   for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
   {
      aiString str;
      mat->GetTexture(type, i, &str);
      bool skip = false;
      for(unsigned int j = 0; j < textures_loaded.size(); j++)
      {
         if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
         {
            textures.push_back(textures_loaded[j]);
            skip = true;
            break;
         }
      }
      if(!skip)
      {   // if texture hasn't been loaded already, load it
         MeshTexture texture;
         const char* path = str.C_Str();
         LOG_INFO("Attempting to create new shared ptr of texture: {0} @ {1}", path, directory);
         texture.texture = std::make_shared<Texture>(path, flipTextures, GLPixelDataFormat::NONE, directory.c_str());
         texture.type = typeName;
         texture.path = path;
         textures.push_back(texture);
         textures_loaded.push_back(texture); // add to loaded textures
      }
   }
   return textures;
}


void Model::setModelMat(const glm::mat4 model)
{
   this->model = model;
}
glm::mat4* Model::getModelMat()
{
   return &model;
}

void Model::updateModelViewProjection(std::shared_ptr<Shader> shader, glm::mat4& view, glm::mat4& projection)
{
   shader->setMat4("model", model);
   shader->setMat4("view", view);
   shader->setMat4("projection", projection);
}

void Model::setUpdateLambda(std::function<void(glm::mat4&, std::shared_ptr<Shader>, const float&, const float&)> updateLambda)
{
   this->updateLambda = updateLambda;
}

glm::vec3 Model::getPosition() const
{
   return glm::vec3(model[3]);
}

int Model::Render(float time, float delta, glm::mat4 view, glm::mat4 projection, std::shared_ptr<Shader> overrideShader)
{
   if(!visible)
   {
      return 0;
   }
   std::shared_ptr<Shader> shader = useShader(overrideShader);
   if(shader == nullptr)
   {
      return 0;
   }
   updateLambda(model, shader, time, delta);
   updateModelViewProjection(shader, view, projection);

   for(std::shared_ptr<Mesh> mesh : meshes)
      mesh->Draw(shader); 

   return 1;
}