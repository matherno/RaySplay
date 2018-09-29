#pragma once

#include <Shader.h>

/*
*   
*/

class EmissionShader : public Shader
  {
private:
  Vector3D colour;

public:
  EmissionShader(const Vector3D& colour);
  virtual ~EmissionShader() {}

  virtual Vector3D shadeSurface(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) override;
  virtual Vector3D getSimplifiedSurfaceColour() const override;
  virtual bool isEmissive() const override { return true; }
  };
