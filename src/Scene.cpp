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
#include <thread>

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

Scene::Scene(uint numThreads /*= 4*/) {
  camera = std::make_shared<CameraOrthographic>();
  this->numThreads = std::max(numThreads, 1u);
}

bool Scene::build(string sceneXMLFilePath)
  {
  return SceneLoader::loadSceneFromXML(sceneXMLFilePath, this);
  }

void Scene::render(ImageOutput* imageOutput)
  {
  /*
   *  Prepare the scene and image output
   */
  mathernogl::logInfo("Starting Image Render...");
  mathernogl::logInfo("Threads: " + std::to_string(numThreads));
  mathernogl::logInfo("Width: " + std::to_string(viewDef.width));
  mathernogl::logInfo("Height: " + std::to_string(viewDef.height));
  mathernogl::logInfo("Samples Per Pixel: " + std::to_string(viewDef.antiAliasingDegree * viewDef.antiAliasingDegree));
  mathernogl::logInfo("Draft Mode: " + std::to_string(viewDef.draftMode));
  long startTime = getTimeMS();
  imageOutput->prepare(viewDef.width, viewDef.height);
  for(GeometryPtr geometry : sceneDef.geometries)
    geometry->init();

  std::mutex queryBailMutex;
  auto queryBailFunc = [&queryBailMutex, imageOutput]()
    {
    queryBailMutex.lock();
    const bool bail = imageOutput->queryBail();
    queryBailMutex.unlock();
    return bail;
    };

  /*
   *  Keep rendering until all the images rows are done
   */
  const uint rowsPerThread = 10;
  uint rowsCompleted = 0;
  while (rowsCompleted < viewDef.height)
    {
    /*
     *  Spin off the threads, each with an allocated set of rows
     */
    std::vector<std::thread> threads;
    std::vector<std::unique_ptr<std::vector<Vector3D>>> threadPixels;
    for (uint threadIdx = 0; threadIdx < numThreads; ++threadIdx)
      {
      const uint rowStart = rowsCompleted + threadIdx * rowsPerThread;
      const uint rowEnd = std::min(rowStart + rowsPerThread, viewDef.height);
      if (rowStart == rowEnd)
        break;
      std::vector<Vector3D>* pixels = new std::vector<Vector3D>();
      threadPixels.emplace_back(pixels);
      threads.emplace_back([this, rowStart, rowEnd, pixels, queryBailFunc]()
        {
        renderPixels(rowStart, rowEnd, pixels, queryBailFunc);
        });
      }

    /*
     *  Wait for each thread to finish, then bail if required
     */
    for (std::thread& thread : threads)
      thread.join();
    if (queryBailFunc())
      return;

    /*
     *  Write the pixels that were just rendered to the image output
     */
    for (auto& pixels : threadPixels)
      {
      const uint rowStart = rowsCompleted;
      const uint rowEnd = std::min(rowStart + rowsPerThread, viewDef.height);
      for (uint pixelY = 0; pixelY < rowsPerThread; ++pixelY)
        {
        for (uint pixelX = 0; pixelX < viewDef.width; ++pixelX)
          {
          const uint index = pixelX + pixelY * viewDef.width;
          if (index < pixels->size())
            paintPixel(pixelX, rowsCompleted + pixelY, pixels->at(index), imageOutput);
          }
        }
      rowsCompleted += rowsPerThread;
      }

    /*
     *  Update progress
     */
    const int numRows = std::min(rowsCompleted, viewDef.height);
    const float percent = (float)(numRows*viewDef.width)/(viewDef.width*viewDef.height) * 100;
    mathernogl::logInfo("Generating Image: " + std::to_string(percent) + "% ");
    imageOutput->updateProgress(percent);
    }

  /*
   *  All finished, finalise the image output
   */
  float time = (float)(getTimeMS() - startTime) / 1000;
  if (time < 4 * 60)
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
  finalColour.x = mathernogl::clampd(finalColour.x, 0.0, 1.0);
  finalColour.y = mathernogl::clampd(finalColour.y, 0.0, 1.0);
  finalColour.z = mathernogl::clampd(finalColour.z, 0.0, 1.0);
  imageOutput->paintPixel(x, y, finalColour);
}

void Scene::renderPixels(uint rowStart, uint rowEnd, std::vector<Vector3D>* pixels, std::function<bool()> queryBailFunc)
  {
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

  for (uint pixelY = rowStart; pixelY < rowEnd; ++pixelY)
    {
    for (uint pixelX = 0; pixelX < viewDef.width; ++pixelX)
      {
      if (!viewDef.draftMode || pixelNum % 4 == 0)
        {
        pixelColour.set(0, 0, 0);
        sampleSet = pixelSampleGenerator->getSampleSet(SampleGenerator::unitSquareMap);
        while (sampleSet->nextSample(&samplePos))
          {
          devicePoint.x = (float) pixelX + (float) samplePos.x;
          devicePoint.y = (float) pixelY + (float) samplePos.y;
          camera->getScreenRay(devicePoint, &viewDef, &screenRay);

          if (!RayTracer::traceRay(&screenRay, &sceneDef, &sampleColour))
            sampleColour = sceneDef.bgColour;
          pixelColour += sampleColour;
          }
        }

      pixels->push_back(pixelColour / pixelSampleGenerator->numSamples);
      if (queryBailFunc())
        return;
      }
    }
  }
