//
// Created by matherno on 3/09/17.
//

#include "PrimitiveTriangleBase.h"

/*
 *  Based on the algorithm presented by Kevin Suffern, in "Ray Tracing from the Ground Up", p362-367,
 */
bool PrimitiveTriangleBase::hitTest(const Ray* ray, float* hitTValue, SurfaceInfo* surfaceInfo) const {
  Vector3D cornerA, cornerB, cornerC;
  getCorners(&cornerA, &cornerB, &cornerC);

  float a = cornerA.x - cornerB.x;
  float b = cornerA.x - cornerC.x;
  float c = ray->direction.x;
  float d = cornerA.x - ray->origin.x;
  float e = cornerA.y - cornerB.y;
  float f = cornerA.y - cornerC.y;
  float g = ray->direction.y;
  float h = cornerA.y - ray->origin.y;
  float i = cornerA.z - cornerB.z;
  float j = cornerA.z - cornerC.z;
  float k = ray->direction.z;
  float l = cornerA.z - ray->origin.z;

  float m = f*k - g*j;
  float n = h*k - g*l;
  float p = f*l - h*j;
  float q = g*i - e*k;
  float s = e*j - f*i;

  float invDenominator = 1.0f / (a*m + b*q + c*s);
  float e1 = d*m - b*n - c*p;
  float beta = e1 * invDenominator;

  if(beta < 0)
    return false;

  float r = e*l - h*i;
  float e2 = a*n + d*q + c*r;
  float gamma = e2 * invDenominator;

  if(gamma < 0 || beta + gamma > 1)
    return false;

  float e3 = a*p - b*r + d*s;
  float tValue = e3 * invDenominator;

  if(tValue > EPSILON){
    *hitTValue = tValue;
    if(surfaceInfo){
      surfaceInfo->normal = getNormal(beta, gamma);
      surfaceInfo->position = ray->getPosition(tValue);
      surfaceInfo->material = material;
    }
    return true;
  }

  return false;
}

bool PrimitiveTriangleBase::hitTest(const Ray* ray, float* hitTValue) const {
  return hitTest(ray, hitTValue, nullptr);
}

void PrimitiveTriangleBase::constructBoundingBox() {
  boundingBox = std::make_shared<BoundingBox>();
  Vector3D cornerA, cornerB, cornerC;
  getCorners(&cornerA, &cornerB, &cornerC);
  boundingBox->addPoint(cornerA);
  boundingBox->addPoint(cornerB);
  boundingBox->addPoint(cornerC);
}
