//
// Created by matherno on 3/09/17.
//

#include "PrimitiveTriangleBase.h"

/*
 *  Based on the algorithm presented by Kevin Suffern, in "Ray Tracing from the Ground Up", p362-367,
 */
bool PrimitiveTriangleBase::hitTest(const Ray* ray, double* hitTValue, SurfaceInfo* surfaceInfo) const {
  Vector3D cornerA, cornerB, cornerC;
  getCorners(&cornerA, &cornerB, &cornerC);

  double a = cornerA.x - cornerB.x;
  double b = cornerA.x - cornerC.x;
  double c = ray->direction.x;
  double d = cornerA.x - ray->origin.x;
  double e = cornerA.y - cornerB.y;
  double f = cornerA.y - cornerC.y;
  double g = ray->direction.y;
  double h = cornerA.y - ray->origin.y;
  double i = cornerA.z - cornerB.z;
  double j = cornerA.z - cornerC.z;
  double k = ray->direction.z;
  double l = cornerA.z - ray->origin.z;

  double m = f*k - g*j;
  double n = h*k - g*l;
  double p = f*l - h*j;
  double q = g*i - e*k;
  double s = e*j - f*i;

  double invDenominator = 1.0f / (a*m + b*q + c*s);
  double e1 = d*m - b*n - c*p;
  double beta = e1 * invDenominator;

  if(beta < 0)
    return false;

  double r = e*l - h*i;
  double e2 = a*n + d*q + c*r;
  double gamma = e2 * invDenominator;

  if(gamma < 0 || beta + gamma > 1)
    return false;

  double e3 = a*p - b*r + d*s;
  double tValue = e3 * invDenominator;

  if(tValue > EPSILON){
    *hitTValue = tValue;
    if(surfaceInfo){
      surfaceInfo->normal = getNormal(beta, gamma);
      surfaceInfo->texCoord = getTexCoord(beta, gamma);
      surfaceInfo->position = ray->getPosition(tValue);
      surfaceInfo->material = material;
    }
    return true;
  }

  return false;
}

bool PrimitiveTriangleBase::hitTest(const Ray* ray, double* hitTValue) const {
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
