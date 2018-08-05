#pragma once
//
// Created by matherno on 21/08/17.
//

#include <SampleGenerator.h>
#include "PhongShader.h"

class ReflectionShader : public PhongShader {
private:
  Vector3D mirrorCol;
  bool isPerfectReflection = true;
  std::unique_ptr<SampleGenerator> sampler;
  SampleSetPtr sampleSetPtr;
  float fuzziness = 0;

public:
  ReflectionShader();
  ReflectionShader(const Vector3D& diffuseColour, const Vector3D& specularColour, float specularExp, Vector3D mirrorCol = Vector3D(0.5));
  ReflectionShader(TexturePtr diffuseTexture, const Vector3D& specularColour, float specularExp, Vector3D mirrorCol = Vector3D(0.5));

  Vector3D getMirrorCol() const { return mirrorCol; }
  void setMirrorCol(const Vector3D& mirrorCol);

  /*
   *  Controls the fuzziness of the reflection, 0 being a perfect reflection, 1 being the max fuzziness
   */
  void setFuzziness(float fuzziness);
  float getFuzziness() const { return fuzziness; }

  virtual Vector3D shadeSurface(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) override;

protected:

  /*
   *  Reflects the given uniform vector around the given normal
   *  Uses hemisphere random sampling if not a perfect reflection
   */
  Vector3D calcReflectedLight(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef);

  void initSampler(uint sqrtNumSamples = 2);
};
