//
// Created by matherno on 3/08/17.
//

#include "RayTracer.h"


bool RayTracer::traceRay(const Ray* ray, const SceneDef* sceneDef, mathernogl::Vector3D* resultColour, bool* hitBackground) {
  bool gotHit = false;
  if (hitBackground)
    *hitBackground = false;
  if(ray->depth < sceneDef->maxRayDepth) {
    double minTValue, testTValue;
    SurfaceInfo finalSurfaceInfo, testSurfaceInfo;

    for (GeometryPtr geometry : sceneDef->geometries) {
      if (geometry->hitTest(ray, &testTValue, &testSurfaceInfo)) {
        if (!gotHit || testTValue < minTValue) {
          minTValue = testTValue;
          finalSurfaceInfo = testSurfaceInfo;
          gotHit = true;
        }
      }
    }

    if (gotHit && resultColour) {
      if (finalSurfaceInfo.material) {
        *resultColour = finalSurfaceInfo.material->shadeSurface(ray, &finalSurfaceInfo, sceneDef);
      }
      else {
        *resultColour = ERROR_COLOUR;
      }
    }

    if (hitBackground)
      *hitBackground = !gotHit;
  }
  return gotHit;
}

bool RayTracer::traceShadowRay(const Ray* ray, const SceneDef* sceneDef, double* tHitValue) {
  double testTValue;
  bool gotHit = false;
  for(GeometryPtr geometry : sceneDef->geometries){
    if(geometry->canCastShadows() && geometry->hitTest(ray, &testTValue)){
      if(!gotHit || testTValue < *tHitValue){
        *tHitValue = testTValue;
        gotHit = true;
      }
    }
  }
  return gotHit;
}
