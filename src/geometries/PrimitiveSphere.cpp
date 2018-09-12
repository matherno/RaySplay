//
// Created by matherno on 1/08/17.
//

#include <Samplers.h>
#include "PrimitiveSphere.h"

PrimitiveSphere::PrimitiveSphere() : centre(0), radius(1) {
  radiusSquared = radius * radius;
  setupSurfaceSampler();
}

PrimitiveSphere::PrimitiveSphere(const mathernogl::Vector3D& centre, double radius) : centre(centre), radius(radius) {
  radiusSquared = radius * radius;
  setupSurfaceSampler();
}

const mathernogl::Vector3D& PrimitiveSphere::getCentre() const {
  return centre;
}

void PrimitiveSphere::setCentre(const mathernogl::Vector3D& centre) {
  this->centre = centre;
}

void PrimitiveSphere::setCentre(double x, double y, double z) {
  this->centre.x = x;
  this->centre.y = y;
  this->centre.z = z;
}

double PrimitiveSphere::getRadius() const {
  return radius;
}

void PrimitiveSphere::setRadius(double radius) {
  this->radius = radius;

  radiusSquared = radius * radius;
}

double PrimitiveSphere::getRadiusSquared() const {
  return radiusSquared;
}

const double twoPi = 2 * M_PI;

Vector2D PrimitiveSphere::getTextureCoord(const Vector3D& position) const {
  const Vector3D spherePos = (position - centre) / radius;
  Vector2D uv;
  double horizontalAngle = atan2(spherePos.x, spherePos.z);
  if (horizontalAngle < 0.0)
    horizontalAngle += twoPi;
  double verticalAngle = acos(spherePos.y);
  uv.x = (float) (horizontalAngle / twoPi);
  uv.y = (float) (1.0 - (verticalAngle / M_PI));
  return uv;
}


bool PrimitiveSphere::hitTest(const Ray* ray, double* hitTValue, SurfaceInfo* surfaceInfo) const {
  using namespace mathernogl;
  Vector3D originToCentre = ray->origin - centre;
  double a = dotProduct(ray->direction, ray->direction);
  double b = 2.0f * dotProduct(originToCentre, ray->direction);
  double c = dotProduct(originToCentre, originToCentre) - radiusSquared;
  double discriminant = b*b - (4.0f*a*c);

  if(discriminant > 0){
    double sqrtPart = sqrt(discriminant);
    double denominator = 2*a;

    double tValueResult = (-b - sqrtPart)/denominator;
    if(tValueResult > EPSILON){
      *hitTValue = tValueResult;
      if(surfaceInfo){
        surfaceInfo->position = ray->getPosition(tValueResult);
        surfaceInfo->normal = (surfaceInfo->position - centre) / radius;
        surfaceInfo->material = material;
        surfaceInfo->texCoord = getTextureCoord(surfaceInfo->position);
      }
      return true;
    }

    tValueResult = (-b + sqrtPart)/denominator;
    if(tValueResult > EPSILON){
      *hitTValue = tValueResult;
      if(surfaceInfo){
        surfaceInfo->position = ray->getPosition(tValueResult);
        surfaceInfo->normal = (surfaceInfo->position - centre) / radius;
        surfaceInfo->material = material;
        surfaceInfo->texCoord = getTextureCoord(surfaceInfo->position);
      }
      return true;
    }
  }
  return false;
}

bool PrimitiveSphere::hitTest(const Ray* ray, double* hitTValue) const {
  return hitTest(ray, hitTValue, nullptr);
}

void PrimitiveSphere::constructBoundingBox() {
  boundingBox = std::make_shared<BoundingBox>(centre - radius, centre + radius);
}

bool PrimitiveSphere::getSurfaceSample(Vector3D* position, Vector3D* normal)
  {
  *position = samplerHelper.getNextSample() * radius + centre;
  *normal = (*position - centre).getUniform();
  return true;
  }

void PrimitiveSphere::setupSurfaceSampler()
  {
  BlueNoiseSampler* sampler = new BlueNoiseSampler(10);
  sampler->generateUnitSphereSamples();
  samplerHelper.initialise(std::shared_ptr<SampleGenerator>(sampler), SampleGenerator::unitSphereMap);
  }


