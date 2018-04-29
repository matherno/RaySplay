//
// Created by matt on 28/04/18.
//

#include <geometries/Mesh.h>
#include <shaders/PhongShader.h>
#include <shaders/LambertianShader.h>
#include "SceneLoader.h"
#include "RaySplayConst.h"

void SceneLoader::loadSceneFromOBJ(const std::string& filePath, AddGeometryFunc addGeometry)
  {
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string error;

  string objDirectory = mathernogl::getDirectoryFromPath(filePath);
  if (!tinyobj::LoadObj(shapes, materials, error, filePath.c_str(), objDirectory.c_str()))
    throw std::runtime_error("Couldn't load obj at '" + filePath + "' ");

  PhongShader* phongShader;
  std::vector<ShaderPtr> shaders;
  for (int matNum = 0; matNum < materials.size(); ++matNum)
    {
    const tinyobj::material_t& material = materials[matNum];
    const Vector3D diffuse(material.diffuse[0], material.diffuse[1], material.diffuse[2]);
    const Vector3D specular(material.specular[0], material.specular[1], material.specular[2]);
    const float specularExp = 1;
    phongShader = new PhongShader(diffuse, specular, specularExp);
    shaders.push_back(ShaderPtr(phongShader));
    mathernogl::logInfo("Material: " + std::to_string(matNum) + ", Diffuse " + diffuse.toString());
    }
  ShaderPtr defaultShader(new LambertianShader(0, 0, 0));

  std::shared_ptr<Mesh> meshObject;
  for(int shapeNum = 0; shapeNum < shapes.size(); ++shapeNum)
    {
    std::vector<uint>& inIndices = shapes[shapeNum].mesh.indices;
    std::vector<float>& inVerts = shapes[shapeNum].mesh.positions;
    std::vector<float>& inNorms = shapes[shapeNum].mesh.normals;
    std::vector<float>& inTexCoords = shapes[shapeNum].mesh.texcoords;
    std::vector<int>& inMaterialIDs = shapes[shapeNum].mesh.material_ids;

    const int materialID = inMaterialIDs[0];
    meshObject.reset(new Mesh());
    meshObject->setSmoothShading(true);
    meshObject->setMaterial(materialID >= 0 && materialID < shaders.size() ? shaders[materialID] : defaultShader);

    //  vertex indices
    for (uint vertexIndex : inIndices)
      {
      meshObject->indices.emplace_back(vertexIndex);
      }

    //  vertex positions
    if (inVerts.size() > 0)
      {
      for (int vertexNum = 0; vertexNum < inVerts.size(); vertexNum += 3)
        {
        meshObject->vertices.emplace_back(inVerts[vertexNum], inVerts[vertexNum + 1], inVerts[vertexNum + 2]);
        }
      }

    //  vertex normals
    if (inNorms.size() > 0)
      {
      for (int vertexNum = 0; vertexNum < inNorms.size(); vertexNum += 3)
        {
        meshObject->normals.emplace_back(inNorms[vertexNum], inNorms[vertexNum + 1], inNorms[vertexNum + 2]);
        }
      }

    // vertex texture coordinates
    if (inTexCoords.size() > 0)
      {
      for (int vertexNum = 0; vertexNum < inTexCoords.size(); vertexNum += 2)
        {
        meshObject->texCoords.emplace_back(inTexCoords[vertexNum], inTexCoords[vertexNum + 1]);
        }
      }

    addGeometry(meshObject);
    }
  }
