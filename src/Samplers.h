#pragma once
//
// Created by matherno on 6/08/17.
//

#include "SampleGenerator.h"

class WhiteNoiseSampler : public SampleGenerator {
public:
  WhiteNoiseSampler(int sqrtNumSamples, int numSampleSets = DEFAULT_NUM_SAMPLE_SETS) : SampleGenerator(sqrtNumSamples, numSampleSets) {
    generateSampleSets();
  }

protected:
  void generateSampleSets() override;
};

class BlueNoiseSampler : public SampleGenerator {
public:
  BlueNoiseSampler(int sqrtNumSamples, int numSampleSets = DEFAULT_NUM_SAMPLE_SETS) : SampleGenerator(sqrtNumSamples, numSampleSets) {
    generateSampleSets();
  }

protected:
  void generateSampleSets() override;
};

class RegularSampler : public SampleGenerator {
public:
  RegularSampler(int sqrtNumSamples, int numSampleSets = DEFAULT_NUM_SAMPLE_SETS) : SampleGenerator(sqrtNumSamples, numSampleSets) {
    generateSampleSets();
  }

protected:
  void generateSampleSets() override;
};