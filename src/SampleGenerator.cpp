//
// Created by matherno on 6/08/17.
//

#include "SampleGenerator.h"
#include "RaySplayConst.h"

SampleGenerator::SampleGenerator(int sqrtNumSamples, int numSampleSets)
    : sqrtNumSamples(sqrtNumSamples), numSampleSets(numSampleSets), numSamples(sqrtNumSamples*sqrtNumSamples)  {
  squareMapSamples.reserve((unsigned long)numSamples * numSampleSets);
}

void SampleGenerator::generateUnitCircleSamples() {
  circleMapSamples.clear();
  circleMapSamples.reserve((unsigned long)numSamples * numSampleSets);
  float radius, angle;
  Vector2D sample;
  for(int sampleNum = 0; sampleNum < squareMapSamples.size(); ++sampleNum) {
    sample = squareMapSamples[sampleNum];
    sample = sample * 2.0f - 1.0f;

    if(sample.x > -sample.y){
      if(sample.x > sample.y){
        radius = sample.x;
        angle = sample.x != 0 ? (sample.y / sample.x) : 0;
      }
      else{
        radius = sample.y;
        angle = sample.y != 0 ? (2 - sample.x / sample.y) : 2;
      }
    }
    else{
      if(sample.x < sample.y){
        radius = -sample.x;
        angle = sample.x != 0 ? (4 + sample.y / sample.x) : 4;
      }
      else{
        radius = -sample.y;
        angle = sample.y != 0 ? (6 - sample.x / sample.y) : 6;
      }
    }

    circleMapSamples.emplace_back(radius*cos(angle*M_PI_4), radius*sin(angle*M_PI_4));
  }
}

void SampleGenerator::generateUnitHemisphereSamples(float distributionExp) {
  hemisphereMapSamples.clear();
  hemisphereMapSamples.reserve((unsigned long)numSamples * numSampleSets);

  Vector2D sample;
  for(int sampleNum = 0; sampleNum < squareMapSamples.size(); ++sampleNum) {
    sample = squareMapSamples[sampleNum];
    float cosPhi = (float)cos(2.0*M_PI*sample.x);
    float sinPhi = (float)sin(2.0*M_PI*sample.x);
    float cosTheta = (float)pow(1.0-sample.y, 1.0/(distributionExp+1.0));
    float sinTheta = (float)sqrt(1.0 - cosTheta*cosTheta);
    hemisphereMapSamples.emplace_back(sinTheta*cosPhi, sinTheta*sinPhi, cosTheta);
  }
}

void SampleGenerator::generateUnitSphereSamples()
  {
  sphereMapSamples.clear();
  sphereMapSamples.reserve((unsigned long)numSamples * numSampleSets);
  Vector2D sample;
  for(int sampleNum = 0; sampleNum < squareMapSamples.size(); ++sampleNum) {
    sample = squareMapSamples[sampleNum];
    float theta = (float) (2.0*M_PI*sample.x);
    float phi = (float) acos(2.0*sample.y - 1);
    sphereMapSamples.emplace_back(sinf(theta)*cosf(phi), sinf(theta)*sinf(phi), cosf(theta));
    }
  }

std::shared_ptr<SampleSet> SampleGenerator::getSampleSet(SampleMapType sampleMapType){
  ASSERT(!squareMapSamples.empty(), "Haven't generated any samples for this sample generator! ");

  if(sampleMapType == unitCircleMap){
    ASSERT(!circleMapSamples.empty(), "Haven't generated circle map samples! ");
  }
  else if (sampleMapType == unitHemisphereMap){
    ASSERT(!hemisphereMapSamples.empty(), "Haven't generated hemisphere map samples! ");
  }
  else if (sampleMapType == unitSphereMap){
    ASSERT(!sphereMapSamples.empty(), "Haven't generated sphere map samples! ");
  }

  if(activeSampleSet)
    activeSampleSet->isValid = false;
  activeSampleSet.reset(new SampleSet(this, RAND_INT(0, numSampleSets-1), sampleMapType));
  return activeSampleSet;
}

bool SampleGenerator::getSample(SampleSet* sampleSet, Vector3D* sample, SampleMapType sampleMapType) const {
  ASSERT(sampleSet == activeSampleSet.get(), "This is not the active sample set!");

  int sampleIndex = (sampleSet->setNum * numSamples) + sampleSet->nextSampleNum;
  switch(sampleMapType){
    case unitSquareMap:
      *sample = squareMapSamples[sampleIndex];
      break;
    case unitCircleMap:
      *sample = circleMapSamples[sampleIndex];
      break;
    case unitHemisphereMap:
      *sample = hemisphereMapSamples[sampleIndex];
      break;
    case unitSphereMap:
      *sample = sphereMapSamples[sampleIndex];
      break;
    default:
      ASSERT(false, "What type of sample map is this? " + std::to_string((int)sampleMapType))
      break;
  }
  ++sampleSet->nextSampleNum;
  return true;
}


void ContinousSamplerHelper::initialise(std::shared_ptr<SampleGenerator> generator, SampleGenerator::SampleMapType sampleType)
  {
  this->generator = generator;
  this->sampleType = sampleType;
  currentSampleSet = nullptr;
  }

Vector3D ContinousSamplerHelper::getNextSample()
  {
  if (!currentSampleSet || !currentSampleSet->hasNext())
    currentSampleSet = generator->getSampleSet(sampleType);
  Vector3D sample;
  if (currentSampleSet)
    currentSampleSet->nextSample(&sample);
  return sample;
  }
