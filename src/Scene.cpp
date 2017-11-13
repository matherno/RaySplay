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
#include <ctime>
#include <chrono>
#include <geometries/GeometryReference.h>
#include <shaders/NormalsTestShader.h>

#ifdef RS_DEBUG
#define NUM_UPDATES 8
#else
#define NUM_UPDATES 32
#endif

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

void Scene::build() {
  mathernogl::resetLog();

  LightSourcePtr ambientLightSource;
#ifdef RS_DEBUG
  ambientLightSource.reset(new AmbientLight(0.02));
#else
  ambientLightSource.reset(new AmbientOcclusion(0.03, 10, 3));
#endif
  sceneDef.lights.push_back(ambientLightSource);

  LightSourcePtr light;

#ifdef SCENE_A
  light.reset(new PointLight(Vector3D(0.72, 0.36, 0.24), Vector3D(0.25, 0.5, -5.5), 10, 1));
#endif

#ifdef SCENE_B
  light.reset(new PointLight(Vector3D(0.6, 0.6, 0.6), Vector3D(-10, 10, 15), 30, 2));
#endif

#ifdef SCENE_C
  light.reset(new DirectionLight(Vector3D(0.4), Vector3D(-1, -0.3, -0.4).getUniform()));
#endif

#ifdef SCENE_D
  light.reset(new DirectionLight(Vector3D(0.4), Vector3D(-1, -0.8, -0.4).getUniform()));
#endif

#ifdef SCENE_E
  light.reset(new PointLight(Vector3D(0.7), Vector3D(5, 10, -12), 25, 3));
  LightSourcePtr light2;
  light2.reset(new PointLight(Vector3D(0.4), Vector3D(-5, 6, -16), 25, 3));
  sceneDef.lights.push_back(light2);
#endif

  sceneDef.lights.push_back(light);

  AccelerationStructure* grid = new RegularGrid();
  sceneDef.geometries.push_back(GeometryPtr(grid));

#ifdef SCENE_D
  GeometryPtr groundBox;
  groundBox.reset(new PrimitiveBoxAA(Vector3D(-30, -10, -50), Vector3D(30, 0, 0)));
  groundBox->setMaterial(std::make_shared<LambertianShader>(0.2, 0.8, 0.1));
  sceneDef.geometries.push_back(groundBox);

  Mesh* palmTree1Mesh = new Mesh("resources/PalmTree1.obj");
  palmTree1Mesh->setSmoothShading(true);
  palmTree1Mesh->ensureNormalsNormalised();
  palmTree1Mesh->setMaterial(std::make_shared<LambertianShader>(0.4, 0.8, 0.2));
  palmTree1Mesh->scaleToSize(Vector3D(1));
  GeometryPtr palmTree1(palmTree1Mesh);

  Mesh* palmTree2Mesh = new Mesh("resources/PalmTree2.obj");
  palmTree2Mesh->setSmoothShading(true);
  palmTree2Mesh->ensureNormalsNormalised();
  palmTree2Mesh->setMaterial(std::make_shared<LambertianShader>(0.4, 0.8, 0.2));
  palmTree2Mesh->scaleToSize(Vector3D(0.8));
  GeometryPtr palmTree2(palmTree2Mesh);

  Mesh* palmTree3Mesh = new Mesh("resources/PalmTree3.obj");
  palmTree3Mesh->setSmoothShading(true);
  palmTree3Mesh->ensureNormalsNormalised();
  palmTree3Mesh->setMaterial(std::make_shared<LambertianShader>(0.4, 0.8, 0.2));
  palmTree3Mesh->scaleToSize(Vector3D(1.2));
  GeometryPtr palmTree3(palmTree3Mesh);

  BlueNoiseSampler treeSampler(5);
  SampleSetPtr sampleSet = treeSampler.getSampleSet(SampleGenerator::unitSquareMap);
  
  typedef mathernogl::RandomGenerator RG;
  Vector3D pos;
  while (sampleSet->nextSample(&pos)){
    float qualifier = RG::randomInt(1, 3);
    GeometryReference* tree;
    if (qualifier == 1) {
      tree = new GeometryReference(palmTree1);
    }
    else if (qualifier == 2) {
      tree = new GeometryReference(palmTree2);
    }
    else {
      tree = new GeometryReference(palmTree3);
    }
    mathernogl::Transform transform;
    transform.scale(RG::randomFloat(0.8, 1.2));
    transform.rotate(0, 1, 0, RG::randomFloat(0, 360));
    transform.translate(pos.x * 14 - 7, 0, -(pos.y * 20 + 8));
    tree->setTransform(transform);
    grid->push_back(GeometryPtr(tree));
  }

#endif

#ifdef SCENE_A
  GeometryPtr object;

  object = std::make_shared<PrimitiveSphere>(Vector3D(0, 0.3, -8), 0.4f);
  object->setMaterial(std::make_shared<LambertianShader>(0.1, 0.4, 0.1));
  grid->push_back(object);

  object = std::make_shared<PrimitiveSphere>(Vector3D(1.0, 0, -6), 0.5f);
  object->setMaterial(std::make_shared<LambertianShader>(0.4, 0.2, 0.9));
  grid->push_back(object);

  PrimitiveBoxAA* box = new PrimitiveBoxAA(Vector3D(-1.05, -0.5, -6), Vector3D(-0.65, 0.0, -7.5));
  object.reset(box);
  object->setMaterial(std::make_shared<LambertianShader>(0.8, 0.8, 0.2));
  grid->push_back(object);

  GeometryPtr plane = std::make_shared<PrimitivePlane>(Vector3D(0.75, -0.5, -6), Vector3D(0, 1, 0).getUniform());
  plane->setMaterial(std::make_shared<LambertianShader>(0.7, 0.1, 0.1));
  sceneDef.geometries.push_back(plane);

  plane = std::make_shared<PrimitivePlane>(Vector3D(0, 0, -10), Vector3D(0, 0, 1).getUniform());
  plane->setMaterial(std::make_shared<LambertianShader>(0.1, 0.1, 0.7));
  sceneDef.geometries.push_back(plane);
#endif


#ifdef SCENE_B
  {   //  ground box
    GeometryPtr groundBox;
    groundBox.reset(new PrimitiveBoxAA(Vector3D(-10, -20, 0), Vector3D(10, 0, 30)));
    groundBox->setMaterial(std::make_shared<LambertianShader>(0.1, 0.5, 0.2));
    grid->push_back(groundBox);
  }

  {   //  blue wall box
    GeometryPtr wallBox;
    wallBox.reset(new PrimitiveBoxAA(Vector3D(-10, 0, -50), Vector3D(10, 20, 0)));
    wallBox->setMaterial(std::make_shared<LambertianShader>(0.1, 0.1, 0.4));
    grid->push_back(wallBox);
  }

  {   //  red wall box
    GeometryPtr wallBox;
    wallBox.reset(new PrimitiveBoxAA(Vector3D(10, 0, 0), Vector3D(20, 20, 40)));
    wallBox->setMaterial(std::make_shared<LambertianShader>(0.4, 0.1, 0.1));
    grid->push_back(wallBox);
  }

  {   //  reflection box
    GeometryPtr reflBox;
    reflBox.reset(new PrimitiveBoxAA(Vector3D(3, 0, 0), Vector3D(6, 5, 16)));
    ReflectionShader* refl = new ReflectionShader(Vector3D(0.2, 0.2, 0.2), Vector3D(0.2, 0.2, 0.2), 1, Vector3D(1));
    refl->setFuzziness(0.15);
    reflBox->setMaterial(ShaderPtr(refl));
    grid->push_back(reflBox);
  }

  {   //  major sphere
    GeometryPtr majSphere;
    majSphere.reset(new PrimitiveSphere(Vector3D(-1, 1.5, 5), 1.5));
    ReflectionShader* refl = new ReflectionShader(Vector3D(0.8, 0, 0), Vector3D(0.3), 10, Vector3D(1, 0, 1));
    refl->setFuzziness(0);
    majSphere->setMaterial(ShaderPtr(refl));
//    majSphere->setMaterial(ShaderPtr(new LambertianShader(resourceManager.loadImageTexture("resources/mercator.jpg"))));
    grid->push_back(majSphere);
  }

  {   //  minor sphere
    GeometryPtr minSphere;
    minSphere.reset(new PrimitiveSphere(Vector3D(-6.3, 2, 8.2), 1.0));
    ReflectionShader* refl = new ReflectionShader(Vector3D(0.7, 0.2, 0), Vector3D(0.3, 0.1, 0.1), 30, Vector3D(0.5));
    refl->setFuzziness(0.8);
    minSphere->setMaterial(ShaderPtr(refl));
//    majSphere->setMaterial(ShaderPtr(new LambertianShader(resourceManager.loadImageTexture("resources/mercator.jpg"))));
    grid->push_back(minSphere);
  }

  {   //  long grey thing
    GeometryPtr flatShiny;
    flatShiny.reset(new PrimitiveBoxAA(Vector3D(0, 0, 8), Vector3D(1, 0.5, 15)));
    flatShiny->setMaterial(std::make_shared<PhongShader>(Vector3D(0.6, 0.3, 0.6), Vector3D(0.9, 0.3, 0.3), 5));
    grid->push_back(flatShiny);
  }

  {   //  cubey
    GeometryPtr cubey;
    cubey.reset(new PrimitiveBoxAA(Vector3D(0, 0, 0), Vector3D(1, 1, 1)));

    {
      mathernogl::Transform transform;
      transform.rotate(0, 1, 0, 30);
      transform.translate(-7, 0, 8);
      GeometryReference* cubeRef = new GeometryReference(cubey);
      cubeRef->setTransform(transform);
      cubeRef->setMaterial(std::make_shared<LambertianShader>(0.05, 0.1, 0.5));
      GeometryPtr instance;
      instance.reset(cubeRef);
      grid->push_back(instance);
    }

    {
      mathernogl::Transform transform;
      transform.rotate(0, 1, 0, 48);
      transform.translate(-5, 0, 10);
      GeometryReference* cubeRef = new GeometryReference(cubey);
      cubeRef->setTransform(transform);
      cubeRef->setMaterial(ShaderPtr(new PhongShader(Vector3D(0.7, 0.3, 0.1), Vector3D(0), 0)));
      GeometryPtr instance;
      instance.reset(cubeRef);
      grid->push_back(instance);
    }

    {
      mathernogl::Transform transform;
      transform.rotate(0, 1, 0, 60);
      transform.translate(-8, 0, 15);
      GeometryReference* cubeRef = new GeometryReference(cubey);
      cubeRef->setTransform(transform);
      cubeRef->setMaterial(std::make_shared<LambertianShader>(0.1, 0.6, 0.4));
      GeometryPtr instance;
      instance.reset(cubeRef);
      grid->push_back(instance);
    }
  }
#endif

#ifdef SCENE_C
  BlueNoiseSampler objectSampler(20);
  std::shared_ptr<SampleSet> sampleSet = objectSampler.getSampleSet(SampleGenerator::unitSquareMap);
  using namespace mathernogl;
  while (sampleSet->hasNext()){
    Vector3D sample;
    sampleSet->nextSample(&sample);

    GeometryPtr object;
    object = std::make_shared<PrimitiveSphere>(Vector3D(sample.x * 40 - 20, sample.y * 20 - 10, RandomGenerator::randomFloat(-20, -40)), 0.5f);
    object->setMaterial(std::make_shared<LambertianShader>(RandomGenerator::randomFloat(0, 0.6), RandomGenerator::randomFloat(0, 0.6), RandomGenerator::randomFloat(0, 0.6)));
    grid->push_back(object);
  }
#endif

#ifdef SCENE_E
//  GeometryPtr groundBox;
//  groundBox.reset(new PrimitiveBoxAA(Vector3D(-30, -10, -100), Vector3D(30, 0, 20)));
//  groundBox->setMaterial(std::make_shared<LambertianShader>(0.2, 0.2, 0.1));
//  sceneDef.geometries.push_back(groundBox);

  {
    Mesh* mesh = new Mesh("resources/Square.obj");
    mesh->setSmoothShading(true);
    mesh->ensureNormalsNormalised();
    ReflectionShader* refl = new ReflectionShader(resourceManager.loadImageTexture("resources/wood_floor.jpg"), Vector3D(1), 250, Vector3D(0.6, 0.5, 0.1));
    refl->setFuzziness(0.25);
    mesh->setMaterial(ShaderPtr(refl));

    GeometryReference* meshRef = new GeometryReference(GeometryPtr(mesh));
    mathernogl::Transform transform;
    transform.rotate(1, 0, 0, -90);
    transform.rotate(0, 1, 0, 36);
    transform.scale(20);
    transform.translate(0, 0, -30);
    meshRef->setTransform(transform);
    grid->push_back(GeometryPtr(meshRef));
  }

  {
    Mesh* mesh = new Mesh("resources/BowlingSet.obj");
    mesh->setSmoothShading(true);
    mesh->ensureNormalsNormalised();
    ReflectionShader* refl = new ReflectionShader(resourceManager.loadImageTexture("resources/bs_diff.jpg"), Vector3D(1), 50, Vector3D(0.1));
    refl->setFuzziness(0.15);
    mesh->setMaterial(ShaderPtr(refl));

    GeometryReference* meshRef = new GeometryReference(GeometryPtr(mesh));
    mathernogl::Transform transform;
    transform.scale(1.2);
    transform.rotate(0, 1, 0, 250);
    transform.translate(0.75, 0, -15);
    meshRef->setTransform(transform);
    grid->push_back(GeometryPtr(meshRef));
  }

//  Mesh* potMesh = new Mesh("resources/PotFernPot.obj");
//  potMesh->setSmoothShading(true);
//  potMesh->ensureNormalsNormalised();
//  potMesh->setMaterial(std::make_shared<PhongShader>(resourceManager.loadImageTexture("resources/PotFernPot.png"), Vector3D(0.2, 0.1, 0.1), 1));
////  potMesh->scaleToSize(Vector3D(5));
//
//  Mesh* leavesMesh = new Mesh("resources/PotFernLeaves.obj");
//  leavesMesh->setSmoothShading(true);
//  leavesMesh->ensureNormalsNormalised();
//  leavesMesh->setMaterial(std::make_shared<PhongShader>(resourceManager.loadImageTexture("resources/PotFernLeaves.png"), Vector3D(0.02, 0.05, 0), 1));
////  leavesMesh->scaleToSize(Vector3D(5));
//
//  Mesh* trunkMesh = new Mesh("resources/PotFernTrunk.obj");
//  trunkMesh->setSmoothShading(true);
//  trunkMesh->ensureNormalsNormalised();
//  trunkMesh->setMaterial(std::make_shared<PhongShader>(resourceManager.loadImageTexture("resources/PotFernTrunk.png"), Vector3D(0.05, 0.01, 0), 1));
////  trunkMesh->scaleToSize(Vector3D(5));
//
//  GeometrySet* potFernMesh = new GeometrySet();
//  potFernMesh->push_back(GeometryPtr(potMesh));
//  potFernMesh->push_back(GeometryPtr(leavesMesh));
//  potFernMesh->push_back(GeometryPtr(trunkMesh));
//  GeometryPtr potFernMeshPtr(potFernMesh);
//
//  {
//    GeometryReference* potFern = new GeometryReference(potFernMeshPtr);
//    mathernogl::Transform transform;
//    transform.scale(0.7);
//    transform.rotate(0, 1, 0, 35);
//    transform.translate(1, 0, -15);
//    potFern->setTransform(transform);
//    grid->push_back(GeometryPtr(potFern));
//  }

#endif

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

  for(uint pixelY = 0; pixelY < viewDef.height; ++pixelY){
    for(uint pixelX = 0; pixelX < viewDef.width; ++pixelX){
//      if (pixelX == 807 && pixelY == 638) {
//        int a = 3;
//        a++;
//      }

      pixelColour.set(0, 0, 0);
      sampleSet = pixelSampleGenerator->getSampleSet(SampleGenerator::unitSquareMap);
      while(sampleSet->nextSample(&samplePos)){
        devicePoint.x = (float)pixelX + samplePos.x;
        devicePoint.y = (float)pixelY + samplePos.y;
        camera->getScreenRay(devicePoint, &viewDef, &screenRay);

        if(!RayTracer::traceRay(&screenRay, &sceneDef, &sampleColour)){
          sampleColour = sceneDef.bgColour;
        }
        pixelColour += sampleColour;
      }
      paintPixel(pixelX, pixelY, pixelColour/pixelSampleGenerator->numSamples, imageOutput);

    }
    if (pixelY % (int)(viewDef.height / NUM_UPDATES) == 0){
      mathernogl::logInfo("Generating Image: " + std::to_string((float)(pixelY*viewDef.width)/(viewDef.width*viewDef.height) * 100) + "% ");
    }
  }

  imageOutput->finalise();
  float time = (float)(getTimeMS() - startTime) / 1000;
  if (time < 60)
    mathernogl::logInfo("Finished Image in " + std::to_string(time) + " seconds ");
  else
    mathernogl::logInfo("Finished Image in " + std::to_string(int(time / 60)) + " minutes ");
}

void Scene::paintPixel(uint x, uint y, const Vector3D& colour, ImageOutput* imageOutput) {
  Vector3D finalColour;
  finalColour.x = (float)pow(colour.x, viewDef.invGamma);
  finalColour.y = (float)pow(colour.y, viewDef.invGamma);
  finalColour.z = (float)pow(colour.z, viewDef.invGamma);
//  float maxComponent = std::max(colour.x, std::max(colour.y, colour.z));
//  if(maxComponent > 1.0){
//    finalColour /= maxComponent;
//  }
  finalColour.x = mathernogl::clampf(finalColour.x, 0.0, 1.0);
  finalColour.y = mathernogl::clampf(finalColour.y, 0.0, 1.0);
  finalColour.z = mathernogl::clampf(finalColour.z, 0.0, 1.0);
  imageOutput->paintPixel(x, y, finalColour);
}

