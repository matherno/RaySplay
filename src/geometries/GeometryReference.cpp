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

bool GeometryReference::hitTest(const Ray* ray, double* hitTValue, SurfaceInfo* surfaceInfo) const {
  Vector3D invTransformedOrigin;
  Vector3D invTransformedRayPoint;
  invTransform.transform(ray->origin,                  &invTransformedOrigin);
  invTransform.transform(ray->origin + ray->direction, &invTransformedRayPoint);

  Ray invTransformedRay = Ray::createFromPoints(invTransformedOrigin, invTransformedRayPoint);
  double invTransformedTValue;
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

bool GeometryReference::hitTest(const Ray* ray, double* hitTValue) const {
  Vector3D invTransformedOrigin;
  Vector3D invTransformedRayPoint;
  invTransform.transform(ray->origin,                  &invTransformedOrigin);
  invTransform.transform(ray->origin + ray->direction, &invTransformedRayPoint);

  Ray invTransformedRay = Ray::createFromPoints(invTransformedOrigin, invTransformedRayPoint);
  double invTransformedTValue;
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

string GeometryReference::constructGLSLHitTest() const
  {
  const mathernogl::Matrix4* invMatrix = invTransform.getTransformMatrix();
  const string stringInvTransformMatrix = "mat4("
                                 + std::to_string(invMatrix->getAt(0, 0)) + ", "
                                 + std::to_string(invMatrix->getAt(1, 0)) + ", "
                                 + std::to_string(invMatrix->getAt(2, 0)) + ", "
                                 + std::to_string(invMatrix->getAt(3, 0)) + ", "
                                 + std::to_string(invMatrix->getAt(0, 1)) + ", "
                                 + std::to_string(invMatrix->getAt(1, 1)) + ", "
                                 + std::to_string(invMatrix->getAt(2, 1)) + ", "
                                 + std::to_string(invMatrix->getAt(3, 1)) + ", "
                                 + std::to_string(invMatrix->getAt(0, 2)) + ", "
                                 + std::to_string(invMatrix->getAt(1, 2)) + ", "
                                 + std::to_string(invMatrix->getAt(2, 2)) + ", "
                                 + std::to_string(invMatrix->getAt(3, 2)) + ", "
                                 + std::to_string(invMatrix->getAt(0, 3)) + ", "
                                 + std::to_string(invMatrix->getAt(1, 3)) + ", "
                                 + std::to_string(invMatrix->getAt(2, 3)) + ", "
                                 + std::to_string(invMatrix->getAt(3, 3)) + ")";

  const mathernogl::Matrix4* matrix = invTransform.getTransformMatrix();
  const string stringNormalTransformMatrix = "mat3("
                                       + std::to_string(matrix->getAt(0, 0)) + ", "
                                       + std::to_string(matrix->getAt(1, 0)) + ", "
                                       + std::to_string(matrix->getAt(2, 0)) + ", "
                                       + std::to_string(matrix->getAt(0, 1)) + ", "
                                       + std::to_string(matrix->getAt(1, 1)) + ", "
                                       + std::to_string(matrix->getAt(2, 1)) + ", "
                                       + std::to_string(matrix->getAt(0, 2)) + ", "
                                       + std::to_string(matrix->getAt(1, 2)) + ", "
                                       + std::to_string(matrix->getAt(2, 2)) + ")";


  string glsl = ""
    "mat3 normalTransformMatrix = " + stringNormalTransformMatrix + ";"
    "mat4 transformMatrixInv = " + stringInvTransformMatrix + ";"
    "vec3 untransformedRayOrigin = rayOrigin;"
    "vec3 untransformedRayDir = rayDir;"
    ""
    "vec3 rayEnd = (vec4(rayOrigin + rayDir, 1) * transformMatrixInv).xyz;"
    "rayOrigin = (vec4(rayOrigin, 1) * transformMatrixInv).xyz;"
    "rayDir = normalize(rayEnd - rayOrigin);"
    "";

  glsl += geometry->constructGLSLHitTest();

  glsl += ""
    ""
    "thisTValue = thisTValue / length(rayEnd - rayOrigin);"
    "thisNormal = normalize(thisNormal * normalTransformMatrix);"
    ""
    "rayOrigin = untransformedRayOrigin;"
    "rayDir = untransformedRayDir;";

  return glsl;
  }

ShaderPtr GeometryReference::getMaterial() const
  {
  return material ? material : geometry->getMaterial();
  }
