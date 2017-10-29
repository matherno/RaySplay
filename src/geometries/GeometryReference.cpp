//
// Created by matherno on 1/10/17.
//

#include "GeometryReference.h"

GeometryReference::GeometryReference(const GeometryPtr& geometry) : geometry(geometry) {}


void GeometryReference::setTransform(const mathernogl::Transform& transform) {
  this->transform = transform;
  this->invTransform = transform.getInverse();
}

bool GeometryReference::init() {
  ASSERT(geometry, "This geometry reference object has no referenced geometry!")
  if(!isInitialised){
    geometry->init();
  }
  return Geometry::init();
}

bool GeometryReference::hitTest(const Ray* ray, float* hitTValue, SurfaceInfo* surfaceInfo) const {
  Vector3D invTransformedOrigin;
  Vector3D invTransformedRayPoint;
  invTransform.transform(ray->origin,                  &invTransformedOrigin);
  invTransform.transform(ray->origin + ray->direction, &invTransformedRayPoint);

  Ray invTransformedRay = Ray::createFromPoints(invTransformedOrigin, invTransformedRayPoint);
  float invTransformedTValue;
  SurfaceInfo testSurfaceInfo;
  if (geometry->hitTest(&invTransformedRay, &invTransformedTValue, &testSurfaceInfo)){
    *hitTValue = invTransformedTValue / invTransformedOrigin.distanceToPoint(invTransformedRayPoint);
    *surfaceInfo = testSurfaceInfo;
    surfaceInfo->position = ray->getPosition(*hitTValue);
    transform.transformNormal(&surfaceInfo->normal);
    if (material)
      surfaceInfo->material = material;
    return true;
  }
  else
    return false;
}

bool GeometryReference::hitTest(const Ray* ray, float* hitTValue) const {
  Vector3D invTransformedOrigin;
  Vector3D invTransformedRayPoint;
  invTransform.transform(ray->origin,                  &invTransformedOrigin);
  invTransform.transform(ray->origin + ray->direction, &invTransformedRayPoint);

  Ray invTransformedRay = Ray::createFromPoints(invTransformedOrigin, invTransformedRayPoint);
  float invTransformedTValue;
  if (geometry->hitTest(&invTransformedRay, &invTransformedTValue)){
    *hitTValue = invTransformedTValue / invTransformedOrigin.distanceToPoint(invTransformedRayPoint);
    return true;
  }
  else
    return false;
}

void GeometryReference::constructBoundingBox() {
  std::vector<Vector3D> bbCorners;
  const BoundingBoxPtr geometryBoundingBox = geometry->getBoundingBox();
  geometryBoundingBox->getCorners(&bbCorners);

  boundingBox = std::make_shared<BoundingBox>();
  for (Vector3D& corner : bbCorners)
    boundingBox->addPoint(transform.transform(corner));
}
