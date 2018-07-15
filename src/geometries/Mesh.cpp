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

bool Mesh::hitTest(const Ray* ray, float* hitTValue, SurfaceInfo* surfaceInfo) const {
  float testTValue;
  SurfaceInfo resultSurfaceInfo;
  if(surfaceInfo ? meshGrid.hitTest(ray, &testTValue, &resultSurfaceInfo) : meshGrid.hitTest(ray, &testTValue)){
    *hitTValue = testTValue;
    if(surfaceInfo)
      *surfaceInfo = resultSurfaceInfo;
    return true;
  }
  return false;
}

bool Mesh::hitTest(const Ray* ray, float* hitTValue) const {
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
