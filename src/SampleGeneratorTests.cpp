//
// Created by matherno on 17/08/17.
//

#include "SampleGeneratorTests.h"

namespace SampleGeneratorTests{

void OutputSamples_UnitSquare(ImageOutput* output, SampleGenerator* sampleGenerator, int width /*= 300*/, int height /*= 300*/) {
  output->prepare((uint)width, (uint)height);
  for(uint x = 0; x < width; ++x){
    for(uint y = 0; y < height; ++y){
      output->paintPixel(x, y, Vector3D(0.05));
    }
  }

  std::shared_ptr<SampleSet> set = sampleGenerator->getSampleSet(SampleGenerator::unitSquareMap);
  Vector3D sample;
  while(set->nextSample(&sample)){
    sample = sample * Vector3D(width, height);
    output->paintPixel((uint)mathernogl::clampf(std::round(sample.x), 0, width-1), (uint)mathernogl::clampf(std::round(sample.y), 0, height-1), Vector3D(1,1,1));
  }
  output->finalise();
}

void OutputSamples_UnitCircle(ImageOutput* output, SampleGenerator* sampleGenerator, int width /*= 300*/, int height /*= 300*/) {
  output->prepare((uint)width, (uint)height);
  for(uint x = 0; x < width; ++x){
    for(uint y = 0; y < height; ++y){
      output->paintPixel(x, y, Vector3D(0.05));
    }
  }

  std::shared_ptr<SampleSet> set = sampleGenerator->getSampleSet(SampleGenerator::unitCircleMap);
  Vector3D sample;
  while(set->nextSample(&sample)){
    sample += 1.0f;
    sample /= 2.0f;
    sample = sample * Vector3D(width, height);
    output->paintPixel((uint)mathernogl::clampf(std::round(sample.x), 0, width-1), (uint)mathernogl::clampf(std::round(sample.y), 0, height-1), Vector3D(1,1,1));
  }
  output->finalise();
}

void OutputSamples_UnitHemisphereXY(ImageOutput* output, SampleGenerator* sampleGenerator, int width /*= 300*/, int height /*= 300*/) {
  output->prepare((uint)width, (uint)height);
  for(uint x = 0; x < width; ++x){
    for(uint y = 0; y < height; ++y){
      output->paintPixel(x, y, Vector3D(0.05));
    }
  }

  std::shared_ptr<SampleSet> set = sampleGenerator->getSampleSet(SampleGenerator::unitHemisphereMap);
  Vector3D sample;
  while(set->nextSample(&sample)){
    sample += 1.0f;
    sample /= 2.0f;
    sample = sample * Vector3D(width, height);
    output->paintPixel((uint)mathernogl::clampf(std::round(sample.x), 0, width-1), (uint)mathernogl::clampf(std::round(sample.y), 0, height-1), Vector3D(1,1,1));
  }
  output->finalise();
}

void OutputSamples_UnitHemisphereXZ(ImageOutput* output, SampleGenerator* sampleGenerator, int width /*= 300*/, int height /*= 300*/) {
  output->prepare((uint)width, (uint)height);
  for(uint x = 0; x < width; ++x){
    for(uint y = 0; y < height; ++y){
      output->paintPixel(x, y, Vector3D(0.05));
    }
  }

  std::shared_ptr<SampleSet> set = sampleGenerator->getSampleSet(SampleGenerator::unitHemisphereMap);
  Vector3D sample;
  while(set->nextSample(&sample)){
    sample += 1.0f;
    sample /= 2.0f;
    sample = sample * Vector3D(width, height, height);
    output->paintPixel((uint)mathernogl::clampf(std::round(sample.x), 0, width-1), (uint)mathernogl::clampf(height-std::round(sample.z), 0, height-1), Vector3D(1,1,1));
  }
  output->finalise();
}

}

