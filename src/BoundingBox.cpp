//
// Created by matherno on 10/09/17.
//

#include "BoundingBox.h"

BoundingBox::BoundingBox() {}

BoundingBox::BoundingBox(const Vector3D& lowerBound, const Vector3D& upperBound) {
  setBounds(lowerBound, upperBound);
}

void BoundingBox::setBounds(const Vector3D& lowerBound, const Vector3D& upperBound) {
  this->lowerBound.x = std::min(lowerBound.x, upperBound.x);
  this->lowerBound.y = std::min(lowerBound.y, upperBound.y);
  this->lowerBound.z = std::min(lowerBound.z, upperBound.z);
  this->upperBound.x = std::max(lowerBound.x, upperBound.x);
  this->upperBound.y = std::max(lowerBound.y, upperBound.y);
  this->upperBound.z = std::max(lowerBound.z, upperBound.z);
  gotBounds = true;
}

void BoundingBox::addBoundingBox(const BoundingBoxPtr boundingBox) {
  ASSERT(boundingBox, "Bounding box that is to be added is null! ")
  if(boundingBox->gotBounds) {
    Vector3D lb = boundingBox->getLowerBound();
    Vector3D ub = boundingBox->getUpperBound();
    if (gotBounds) {
      lb.x = std::min(lowerBound.x, lb.x);
      lb.y = std::min(lowerBound.y, lb.y);
      lb.z = std::min(lowerBound.z, lb.z);
      ub.x = std::max(upperBound.x, ub.x);
      ub.y = std::max(upperBound.y, ub.y);
      ub.z = std::max(upperBound.z, ub.z);
    }
    setBounds(lb, ub);
  }
}

void BoundingBox::addPoint(const Vector3D& point) {
  Vector3D lb, ub;
  if(gotBounds){
    lb.x = std::min(lowerBound.x, point.x);
    lb.y = std::min(lowerBound.y, point.y);
    lb.z = std::min(lowerBound.z, point.z);
    ub.x = std::max(upperBound.x, point.x);
    ub.y = std::max(upperBound.y, point.y);
    ub.z = std::max(upperBound.z, point.z);
  }
  else{
    lb = point;
    ub = point;
  }
  setBounds(lb, ub);
}

void BoundingBox::addPoint(double x, double y, double z) {
  addPoint(Vector3D(x, y, z));
}

Vector3D BoundingBox::getFaceNormal(FaceID faceID) const{
  switch(faceID){
    case faceNegX:
      return Vector3D(-1, 0, 0);
    case faceNegY:
      return Vector3D(0, -1, 0);
    case faceNegZ:
      return Vector3D(0, 0, -1);
    case facePosX:
      return Vector3D(1, 0, 0);
    case facePosY:
      return Vector3D(0, 1, 0);
    case facePosZ:
      return Vector3D(0, 0, 1);
    default:
      ASSERT(false, "Invalid face ID! " + std::to_string(faceID));
      return Vector3D();
  }
}


/*
 *  Based on the algorithm presented by Kevin Suffern, in "Ray Tracing from the Ground Up", p354-361,
 */
bool BoundingBox::hitTest(const Ray* ray, double* hitTValue, FaceID* hitFaceID /*= nullptr */) const {
  double xTValueMin, yTValueMin, zTValueMin;
  double xTValueMax, yTValueMax, zTValueMax;

  double invRayDirX = 1.0f / ray->direction.x;
  if(invRayDirX >= 0){
    xTValueMin = (lowerBound.x - ray->origin.x) * invRayDirX;
    xTValueMax = (upperBound.x - ray->origin.x) * invRayDirX;
  }
  else {
    xTValueMin = (upperBound.x - ray->origin.x) * invRayDirX;
    xTValueMax = (lowerBound.x - ray->origin.x) * invRayDirX;
  }

  double invRayDirY = 1.0f / ray->direction.y;
  if(invRayDirY >= 0){
    yTValueMin = (lowerBound.y - ray->origin.y) * invRayDirY;
    yTValueMax = (upperBound.y - ray->origin.y) * invRayDirY;
  }
  else {
    yTValueMin = (upperBound.y - ray->origin.y) * invRayDirY;
    yTValueMax = (lowerBound.y - ray->origin.y) * invRayDirY;
  }

  double invRayDirZ = 1.0f / ray->direction.z;
  if(invRayDirZ >= 0){
    zTValueMin = (lowerBound.z - ray->origin.z) * invRayDirZ;
    zTValueMax = (upperBound.z - ray->origin.z) * invRayDirZ;
  }
  else {
    zTValueMin = (upperBound.z - ray->origin.z) * invRayDirZ;
    zTValueMax = (lowerBound.z - ray->origin.z) * invRayDirZ;
  }


  double tValueIn;
  FaceID hitFaceIn;
  if(xTValueMin > yTValueMin){
    tValueIn = xTValueMin;
    hitFaceIn = invRayDirX >= 0.0 ? faceNegX : facePosX;
  }
  else{
    tValueIn = yTValueMin;
    hitFaceIn = invRayDirY >= 0.0 ? faceNegY : facePosY;
  }
  if(zTValueMin > tValueIn){
    tValueIn = zTValueMin;
    hitFaceIn = invRayDirZ >= 0.0 ? faceNegZ : facePosZ;
  }

  double tValueOut;
  FaceID hitFaceOut;
  if(xTValueMax < yTValueMax){
    tValueOut = xTValueMax;
    hitFaceOut = invRayDirX >= 0.0 ? facePosX : faceNegX;
  }
  else{
    tValueOut = yTValueMax;
    hitFaceOut = invRayDirY >= 0.0 ? facePosY : faceNegY;
  }
  if(zTValueMax < tValueOut){
    tValueOut = zTValueMax;
    hitFaceOut = invRayDirZ >= 0.0 ? facePosZ : faceNegZ;
  }

  if(tValueIn < tValueOut && tValueOut > 0){
    if(tValueIn > 0){
      //  hit from outside the box
      *hitTValue = tValueIn;
      if(hitFaceID)
        *hitFaceID = hitFaceIn;
    }
    else{
      //  hit from inside the box
      *hitTValue = tValueOut;
      if(hitFaceID)
        *hitFaceID = hitFaceOut;
    }
    return true;
  }
  else
    return false;
}

bool BoundingBox::hitTest(const Ray* ray) const {
  double tValue;
  return hitTest(ray, &tValue);
}

void BoundingBox::expandBounds(float expansion /*= EPSILON*/) {
  if(gotBounds){
    lowerBound -= expansion;
    upperBound += expansion;
  }
}

void BoundingBox::getSize(double* x, double* y, double* z) const {
  Vector3D size;
  size = getSize();
  *x = size.x;
  *y = size.y;
  *z = size.z;
}

Vector3D BoundingBox::getSize() const {
  Vector3D size;
  size.x = (float)fabs(upperBound.x - lowerBound.x);
  size.y = (float)fabs(upperBound.y - lowerBound.y);
  size.z = (float)fabs(upperBound.z - lowerBound.z);
  return size;
}

bool BoundingBox::isPointWithinBounds(const Vector3D& point) const {
  return point.x > lowerBound.x && point.y > lowerBound.y && point.z > lowerBound.z &&
      point.x < upperBound.x && point.y < upperBound.y && point.z < upperBound.z;
}

void BoundingBox::getCorners(std::vector<Vector3D>* corners) const {
  const Vector3D& lb = lowerBound;
  const Vector3D& ub = upperBound;
  corners->emplace_back(Vector3D(lb.x, lb.y, lb.z));
  corners->emplace_back(Vector3D(ub.x, lb.y, lb.z));
  corners->emplace_back(Vector3D(lb.x, ub.y, lb.z));
  corners->emplace_back(Vector3D(ub.x, ub.y, lb.z));
  corners->emplace_back(Vector3D(lb.x, lb.y, ub.z));
  corners->emplace_back(Vector3D(ub.x, lb.y, ub.z));
  corners->emplace_back(Vector3D(lb.x, ub.y, ub.z));
  corners->emplace_back(Vector3D(ub.x, ub.y, ub.z));
}

string BoundingBox::constructGLSLHitTest() const
  {
  return ""
    ""
    "  int faceNegY = 0;"
    "  int faceNegX = 1;"
    "  int faceNegZ = 2;"
    "  int facePosX = 3;"
    "  int facePosY = 4;"
    "  int facePosZ = 5;"
    "  int hitFace = -1;"
    ""
    ""
    "  vec3 lowerBound = vec3(" + std::to_string(lowerBound.x) + ", " + std::to_string(lowerBound.y) + ", " + std::to_string(lowerBound.z) + ");"
    "  vec3 upperBound = vec3(" + std::to_string(upperBound.x) + ", " + std::to_string(upperBound.y) + ", " + std::to_string(upperBound.z) + ");"
    ""
    "  float xTValueMin, yTValueMin, zTValueMin;\n"
    "  float xTValueMax, yTValueMax, zTValueMax;\n"
    "\n"
    "  float invRayDirX = 1.0f / rayDir.x;\n"
    "  if(invRayDirX >= 0){\n"
    "    xTValueMin = (lowerBound.x - rayOrigin.x) * invRayDirX;\n"
    "    xTValueMax = (upperBound.x - rayOrigin.x) * invRayDirX;\n"
    "  }\n"
    "  else {\n"
    "    xTValueMin = (upperBound.x - rayOrigin.x) * invRayDirX;\n"
    "    xTValueMax = (lowerBound.x - rayOrigin.x) * invRayDirX;\n"
    "  }\n"
    "\n"
    "  float invRayDirY = 1.0f / rayDir.y;\n"
    "  if(invRayDirY >= 0){\n"
    "    yTValueMin = (lowerBound.y - rayOrigin.y) * invRayDirY;\n"
    "    yTValueMax = (upperBound.y - rayOrigin.y) * invRayDirY;\n"
    "  }\n"
    "  else {\n"
    "    yTValueMin = (upperBound.y - rayOrigin.y) * invRayDirY;\n"
    "    yTValueMax = (lowerBound.y - rayOrigin.y) * invRayDirY;\n"
    "  }\n"
    "\n"
    "  float invRayDirZ = 1.0f / rayDir.z;\n"
    "  if(invRayDirZ >= 0){\n"
    "    zTValueMin = (lowerBound.z - rayOrigin.z) * invRayDirZ;\n"
    "    zTValueMax = (upperBound.z - rayOrigin.z) * invRayDirZ;\n"
    "  }\n"
    "  else {\n"
    "    zTValueMin = (upperBound.z - rayOrigin.z) * invRayDirZ;\n"
    "    zTValueMax = (lowerBound.z - rayOrigin.z) * invRayDirZ;\n"
    "  }\n"
    "  "
    "  "
    "  float tValueIn;\n"
    "  int hitFaceIn;\n"
    "  if(xTValueMin > yTValueMin){\n"
    "    tValueIn = xTValueMin;\n"
    "    hitFaceIn = invRayDirX >= 0.0 ? faceNegX : facePosX;\n"
    "  }\n"
    "  else{\n"
    "    tValueIn = yTValueMin;\n"
    "    hitFaceIn = invRayDirY >= 0.0 ? faceNegY : facePosY;\n"
    "  }\n"
    "  if(zTValueMin > tValueIn){\n"
    "    tValueIn = zTValueMin;\n"
    "    hitFaceIn = invRayDirZ >= 0.0 ? faceNegZ : facePosZ;\n"
    "  }\n"
    "\n"
    "  float tValueOut;\n"
    "  int hitFaceOut;\n"
    "  if(xTValueMax < yTValueMax){\n"
    "    tValueOut = xTValueMax;\n"
    "    hitFaceOut = invRayDirX >= 0.0 ? facePosX : faceNegX;\n"
    "  }\n"
    "  else{\n"
    "    tValueOut = yTValueMax;\n"
    "    hitFaceOut = invRayDirY >= 0.0 ? facePosY : faceNegY;\n"
    "  }\n"
    "  if(zTValueMax < tValueOut){\n"
    "    tValueOut = zTValueMax;\n"
    "    hitFaceOut = invRayDirZ >= 0.0 ? facePosZ : faceNegZ;\n"
    "  }\n"
    "\n"
    "  if(tValueIn < tValueOut && tValueOut > EPSILON){\n"
    "    if(tValueIn > 0){\n"
    "      //  hit from outside the box\n"
    "      thisTValue = tValueIn;\n"
    "      hitFace = hitFaceIn;\n"
    "    }\n"
    "    else{\n"
    "      //  hit from inside the box\n"
    "      thisTValue = tValueOut;\n"
    "      hitFace = hitFaceOut;\n"
    "    }\n"
    ""
    "if (hitFace == faceNegX)\n"
    "      thisNormal = vec3(-1, 0, 0);\n"
    "if (hitFace == faceNegY)\n"
    "      thisNormal = vec3(0, -1, 0);\n"
    "if (hitFace == faceNegZ)"
    "      thisNormal = vec3(0, 0, -1);\n"
    "if (hitFace == facePosX)"
    "      thisNormal = vec3(1, 0, 0);\n"
    "if (hitFace == facePosY)"
    "      thisNormal = vec3(0, 1, 0);\n"
    "if (hitFace == facePosZ)"
    "      thisNormal = vec3(0, 0, 1);\n"
    "  }";
  }



