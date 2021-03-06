#pragma once
//
// Created by matherno on 6/08/17.
//

#include <mathernogl/MathernoGL.h>
#include <mutex>
#include "RaySplayConst.h"

#define DEFAULT_NUM_SAMPLE_SETS 83
#define DEFAULT_SQRT_NUM_SAMPLES 16

class SampleSet;

typedef std::shared_ptr<SampleSet> SampleSetPtr;

class SampleGenerator {
  friend class SampleSet;

public:
  enum SampleMapType{
    unitSquareMap,
    unitCircleMap,
    unitHemisphereMap,
    unitSphereMap
  };

private:
  SampleSetPtr activeSampleSet;
  std::mutex mutex;

protected:
  std::vector<mathernogl::Vector2D> squareMapSamples;       //  unit square (0,0) -> (1,1), any point inside
  std::vector<mathernogl::Vector2D> circleMapSamples;       //  unit circle, centre (0,0), radius 1, any point inside
  std::vector<mathernogl::Vector3D> hemisphereMapSamples;   //  unit hemisphere => a sphere with centre (0,0,0), radius 1, where z > 0, samples will be on the circumference
  std::vector<mathernogl::Vector3D> sphereMapSamples;       //  unit sphere => a sphere with centre (0,0,0), radius 1, samples will be on the circumference

public:
  const int numSamples;
  const int numSampleSets;
  const int sqrtNumSamples;

  SampleGenerator(int sqrtNumSamples = DEFAULT_SQRT_NUM_SAMPLES, int numSampleSets = DEFAULT_NUM_SAMPLE_SETS);
  virtual ~SampleGenerator(){};

  void generateUnitCircleSamples();
  void generateUnitSphereSamples();
  void generateUnitHemisphereSamples(float distributionExp);

  SampleSetPtr getSampleSet(SampleMapType sampleMapType);
  SampleSetPtr getActiveSampleSet(){ return activeSampleSet; };

protected:
  virtual void generateSampleSets() = 0;
  bool getSample(SampleSet* sampleSet, mathernogl::Vector3D* sample, SampleMapType sampleMapType) const;
};

class SampleSet {
  friend class SampleGenerator;
private:
  const SampleGenerator* generator;
  const int setNum;
  const SampleGenerator::SampleMapType sampleMapType;
  int nextSampleNum = 0;
  bool isValid = true;

  SampleSet(SampleGenerator* generator, int setNum, SampleGenerator::SampleMapType sampleMapType) : generator(generator), setNum(setNum), sampleMapType(sampleMapType) {}

public:
  inline bool nextSample(mathernogl::Vector3D* sample){
    return hasNext() && generator->getSample(this, sample, sampleMapType);
  }

  inline bool hasNext() {
    return isValid && nextSampleNum < generator->numSamples;
  }
};

class ContinousSamplerHelper
  {
private:
  std::shared_ptr<SampleGenerator> generator;
  SampleSetPtr currentSampleSet;
  SampleGenerator::SampleMapType sampleType;
  std::mutex mutex;

public:
  void initialise(std::shared_ptr<SampleGenerator> generator, SampleGenerator::SampleMapType sampleType);
  Vector3D getNextSample();
  };