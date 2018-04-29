#pragma once
//
// Created by matherno on 10/09/17.
//


#include "Ray.h"

class BoundingBox;
typedef std::shared_ptr<BoundingBox> BoundingBoxPtr;

enum FaceID{
  faceNegY,
  faceNegX,
  faceNegZ,
  facePosX,
  facePosY,
  facePosZ,
};

class BoundingBox {
public:

private:
  Vector3D lowerBound;
  Vector3D upperBound;
  bool gotBounds = false;

public:
  BoundingBox();
  BoundingBox(const Vector3D& lowerBound, const Vector3D& upperBound);

  Vector3D getLowerBound() const { return lowerBound; }
  Vector3D getUpperBound() const { return upperBound; }
  void getSize(float* x, float* y, float* z) const;
  Vector3D getSize() const;
  void setBounds(const Vector3D& lowerBound, const Vector3D& upperBound);
  void expandBounds(float expansion = EPSILON);

  //  will extend the bounds to fit the given point or bounding box
  void addPoint(float x, float y, float z);
  void addPoint(const Vector3D& point);
  void addBoundingBox(const BoundingBoxPtr geometry);

  Vector3D getFaceNormal(FaceID faceID) const;
  bool hitTest(const Ray* ray, float* hitTValue, FaceID* hitFaceID = nullptr) const;
  bool hitTest(const Ray* ray) const;
  bool isPointWithinBounds(const Vector3D& point) const;

  void getCorners(std::vector<Vector3D>* corners) const;
};
