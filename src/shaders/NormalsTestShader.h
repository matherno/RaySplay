#pragma once
//
// Created by matherno on 8/11/17.
//

/*
*   Just a way to test normals, by using a surfaces normal as its colour
*/

#include <Shader.h>

class NormalsTestShader : public Shader {
public:
  virtual Vector3D shadeSurface(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) override;
};
