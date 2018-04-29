#pragma once
//
// Created by matherno on 3/09/17.
//


#include <acceleration_structures/RegularGrid.h>
#include "Geometry.h"
#include "PrimitiveTriangleBase.h"

class Mesh : public Geometry {
private:
  bool smoothShading = false;
  RegularGrid meshGrid;

public:
  std::vector<Vector3D> vertices;
  std::vector<Vector3D> normals;
  std::vector<Vector2D> texCoords;
  std::vector<int> indices;

  Mesh();
  Mesh(const std::string objFilePath, bool justFirstObject = true);
  virtual ~Mesh(){};

  bool smoothShadingEnabled() const { return smoothShading; }
  void setSmoothShading(bool smoothShading);
  void ensureNormalsNormalised();

  /*
  *  scales the vertices to fit the given size, preserving the shape of the mesh
  */
  void scaleToSize(Vector3D size);

  virtual bool hitTest(const Ray* ray, float* hitTValue, SurfaceInfo* surfaceInfo) const override;
  virtual bool hitTest(const Ray* ray, float* hitTValue) const override;
  virtual bool init() override;

  virtual const BoundingBoxPtr getBoundingBox() const override;

  virtual void constructBoundingBox() override;
};