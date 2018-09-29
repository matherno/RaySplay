//
// Created by matherno on 3/09/17.
//

#include "Mesh.h"

/*
 *  MeshTriangle
 */

class MeshTriangle : public PrimitiveTriangleBase {
public:
  int vertIndexA;
  int vertIndexB;
  int vertIndexC;
  const Mesh* mesh;

  MeshTriangle(const Mesh* mesh) : mesh(mesh) {}

  void set(int vertIndexA, int vertIndexB, int vertIndexC){
    this->vertIndexA = vertIndexA;
    this->vertIndexB = vertIndexB;
    this->vertIndexC = vertIndexC;
  }

  virtual void getCorners(Vector3D* cornerA, Vector3D* cornerB, Vector3D* cornerC) const override {
    *cornerA = mesh->vertices[vertIndexA];
    *cornerB = mesh->vertices[vertIndexB];
    *cornerC = mesh->vertices[vertIndexC];
  }

  virtual Vector3D getNormal(float beta, float gamma) const override {
    Vector3D normal(0, 0, 0);
    if(mesh->smoothShadingEnabled() && mesh->normals.size() > 0) {
      normal += mesh->normals[vertIndexA] * (1.0f - beta - gamma);
      normal += mesh->normals[vertIndexB] * beta;
      normal += mesh->normals[vertIndexC] * gamma;
      normal.makeUniform();
    }
    else {
      normal = mathernogl::crossProduct(mesh->vertices[vertIndexC] - mesh->vertices[vertIndexB], mesh->vertices[vertIndexA] - mesh->vertices[vertIndexB]).getUniform();
    }
    return normal;
  }

  virtual Vector2D getTexCoord(float beta, float gamma) const override {
    Vector2D texCoord(0, 0);
    if (mesh->texCoords.size() > 0) {
      texCoord += mesh->texCoords[vertIndexA] * (1.0f - beta - gamma);
      texCoord += mesh->texCoords[vertIndexB] * beta;
      texCoord += mesh->texCoords[vertIndexC] * gamma;
    }
    return texCoord;
  }
};


/*
 *  Mesh
 */

Mesh::Mesh() : sourceFilePath("") {
}

Mesh::Mesh(const std::string objFilePath, bool justFirstObject) : sourceFilePath(objFilePath) {
  mathernogl::loadObj(objFilePath, &indices, &vertices, &normals, &texCoords, justFirstObject);
  mathernogl::logInfo("Loaded Mesh: '" + objFilePath + "'");
}

bool Mesh::init() {
  if (!isInitialised){
    for(int indicesIndex = 0; indicesIndex + 2 < indices.size(); indicesIndex += 3){
      MeshTriangle* triangle = new MeshTriangle(this);
      triangle->set(indices[indicesIndex], indices[indicesIndex + 1], indices[indicesIndex + 2]);
      triangle->init();
      meshGrid.push_back(GeometryPtr(triangle));
    }
    meshGrid.setMaterial(material);
    meshGrid.init();
  }
  return Geometry::init();
}

void Mesh::constructBoundingBox() {
  meshGrid.constructBoundingBox();
}

bool Mesh::hitTest(const Ray* ray, double* hitTValue, SurfaceInfo* surfaceInfo) const {
  double testTValue;
  SurfaceInfo resultSurfaceInfo;
  if(surfaceInfo ? meshGrid.hitTest(ray, &testTValue, &resultSurfaceInfo) : meshGrid.hitTest(ray, &testTValue)){
    *hitTValue = testTValue;
    if(surfaceInfo)
      *surfaceInfo = resultSurfaceInfo;
    return true;
  }
  return false;
}

bool Mesh::hitTest(const Ray* ray, double* hitTValue) const {
  return hitTest(ray, hitTValue, nullptr);
}

void Mesh::setSmoothShading(bool smoothShading) {
  this->smoothShading = smoothShading;
}

void Mesh::ensureNormalsNormalised() {
  for(Vector3D& normal : normals){
    normal.makeUniform();
  }
}

const BoundingBoxPtr Mesh::getBoundingBox() const {
  return meshGrid.getBoundingBox();
}

string Mesh::constructGLSLHitTest() const
  {
  string glsl;

  glsl += getBoundingBox()->constructGLSLHitTest();

  // this hit test is way too intensive and causes the the compiled shaders to take up way too much memory
  /*
  for(int indicesIndex = 0; indicesIndex + 2 < indices.size(); indicesIndex += 3)
    {
    const int index1 = indices[indicesIndex];
    const int index2 = indices[indicesIndex+1];
    const int index3 = indices[indicesIndex+2];
    const Vector3D& vertex1 = vertices[index1];
    const Vector3D& vertex2 = vertices[index2];
    const Vector3D& vertex3 = vertices[index3];
    Vector3D normal1 = normals[index1];
    Vector3D normal2 = normals[index2];
    Vector3D normal3 = normals[index3];
    if (!smoothShadingEnabled())
      {
      const Vector3D flatShadedNormal = mathernogl::crossProduct(vertex3 - vertex2, vertex1 - vertex2).getUniform();
      normal1 = flatShadedNormal;
      normal2 = flatShadedNormal;
      normal3 = flatShadedNormal;
      }

    string hitTestGLSL = "{"
      "vec3 triNormal;"
      "vec3 cornerA = " + glslVec3(vertex1) + ";"
      "vec3 cornerB = " + glslVec3(vertex2) + ";"
      "vec3 cornerC = " + glslVec3(vertex3) + ";"
      "vec3 normalA = " + glslVec3(normal1) + ";"
      "vec3 normalB = " + glslVec3(normal2) + ";"
      "vec3 normalC = " + glslVec3(normal3) + ";"
      ""
      "float triTValue = hitTestTriangle(rayOrigin, rayDir, cornerA, cornerB, cornerC, normalA, normalB, normalC, triNormal);"
      ""
      "if (triTValue > 0 && (thisTValue < 0 || triTValue < thisTValue))"
      "  {"
      "  thisTValue = triTValue;"
      "  thisNormal = triNormal;"
      "  }"
      "}";

    if (hitTestGLSL.size() + glsl.size() > glsl.max_size())
      {
      ASSERT(false, "We've reached the maximum string size! " + std::to_string(glsl.max_size()))
      return "";
      }

    glsl += hitTestGLSL;
    }
  */

  return glsl;
  }
