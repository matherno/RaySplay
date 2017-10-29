//
// Created by matherno on 3/08/17.
//

#include "RayTracer.h"


bool RayTracer::traceRay(const Ray* ray, const SceneDef* sceneDef, mathernogl::Vector3D* resultColour) {
  bool gotHit = false;
  if(ray->depth < sceneDef->maxRayDepth) {
    float minTValue, testTValue;
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
        resultColour->set(0.9, 0.0, 0.6);
      }
    }
  }
  return gotHit;
}

bool RayTracer::traceShadowRay(const Ray* ray, const SceneDef* sceneDef, float* tHitValue) {
  float testTValue;
  bool gotHit = false;
  for(GeometryPtr geometry : sceneDef->geometries){
    if(geometry->hitTest(ray, &testTValue)){
      if(!gotHit || testTValue < *tHitValue){
        *tHitValue = testTValue;
        gotHit = true;
      }
    }
  }
  return gotHit;
}
