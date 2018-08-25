//
// Created by matherno on 1/08/17.
//

#include <shaders/LambertianShader.h>
#include <shaders/PhongShader.h>
#include <lights/PointLight.h>
#include <lights/AmbientOcclusion.h>
#include <lights/AmbientLight.h>
#include <shaders/ReflectionShader.h>
#include <cameras/CameraOrthographic.h>
#include <geometries/Mesh.h>
#include <lights/DirectionLight.h>
#include <geometries/PrimitiveBoxAA.h>
#include <geometries/PrimitiveSphere.h>
#include <geometries/GeometrySet.h>
#include <acceleration_structures/RegularGrid.h>
#include "Scene.h"
#include "RayTracer.h"
#include "AccelerationStructure.h"
#include "SceneLoader.h"
#include <ctime>
#include <chrono>
#include <geometries/GeometryReference.h>
#include <shaders/NormalsTestShader.h>

#define NUM_UPDATES 32

long getTimeMS() {
  using namespace std::chrono;
  milliseconds ms = duration_cast<milliseconds>(
      system_clock::now().time_since_epoch()
  );
  return ms.count();
}

long getTimeUS(){
  using namespace std::chrono;
  microseconds us = duration_cast< microseconds >(
      system_clock::now().time_since_epoch()
  );
  return us.count();
}

Scene::Scene() {
  camera = std::make_shared<CameraOrthographic>();
}

bool Scene::build(string sceneXMLFilePath)
  {
  return SceneLoader::loadSceneFromXML(sceneXMLFilePath, this);
  }

void Scene::render(ImageOutput* imageOutput) {
  long startTime = getTimeMS();
  imageOutput->prepare(viewDef.width, viewDef.height);

  for(GeometryPtr geometry : sceneDef.geometries){
    geometry->init();
  }

  std::unique_ptr<SampleGenerator> pixelSampleGenerator;
  if(viewDef.antiAliasingDegree > 1)
    pixelSampleGenerator = std::make_unique<BlueNoiseSampler>(viewDef.antiAliasingDegree);
  else
    pixelSampleGenerator = std::make_unique<RegularSampler>(1);

  Ray screenRay;
  screenRay.depth = 0;
  Vector2D devicePoint;
  Vector3D samplePos;
  std::shared_ptr<SampleSet> sampleSet;
  Vector3D pixelColour(0);
  Vector3D sampleColour(0);
  uint pixelNum = 0;

  for(uint pixelY = 0; pixelY < viewDef.height; ++pixelY){
    for(uint pixelX = 0; pixelX < viewDef.width; ++pixelX){

      if (!viewDef.draftMode || pixelNum % 4 == 0) {
        pixelColour.set(0, 0, 0);
        sampleSet = pixelSampleGenerator->getSampleSet(SampleGenerator::unitSquareMap);
        while (sampleSet->nextSample(&samplePos)) {
          devicePoint.x = (float) pixelX + samplePos.x;
          devicePoint.y = (float) pixelY + samplePos.y;
          camera->getScreenRay(devicePoint, &viewDef, &screenRay);

          if (!RayTracer::traceRay(&screenRay, &sceneDef, &sampleColour))
            sampleColour = sceneDef.bgColour;
          pixelColour += sampleColour;
        }
      }

      paintPixel(pixelX, pixelY, pixelColour/pixelSampleGenerator->numSamples, imageOutput);
      ++pixelNum;
      if (imageOutput->queryBail())
        return;
    }
    if (pixelY % (int)(viewDef.height / NUM_UPDATES) == 0){
      const float percent = (float)(pixelY*viewDef.width)/(viewDef.width*viewDef.height) * 100;
      mathernogl::logInfo("Generating Image: " + std::to_string(percent) + "% ");
      imageOutput->updateProgress(percent);
    }
  }

  float time = (float)(getTimeMS() - startTime) / 1000;
  if (time < 60)
    mathernogl::logInfo("Finished Image in " + std::to_string(time) + " seconds ");
  else
    mathernogl::logInfo("Finished Image in " + std::to_string(int(time / 60)) + " minutes ");
  imageOutput->finalise();
}

void Scene::paintPixel(uint x, uint y, const Vector3D& colour, ImageOutput* imageOutput) {
  Vector3D finalColour;
  finalColour.x = pow(colour.x, viewDef.invGamma);
  finalColour.y = pow(colour.y, viewDef.invGamma);
  finalColour.z = pow(colour.z, viewDef.invGamma);
  double maxComponent = std::max(finalColour.x, std::max(finalColour.y, finalColour.z));
  if(maxComponent > 1.0){
    finalColour /= maxComponent;
  }
//  finalColour.x = mathernogl::clampd(finalColour.x, 0.0, 1.0);
//  finalColour.y = mathernogl::clampd(finalColour.y, 0.0, 1.0);
//  finalColour.z = mathernogl::clampd(finalColour.z, 0.0, 1.0);
  imageOutput->paintPixel(x, y, finalColour);
}
