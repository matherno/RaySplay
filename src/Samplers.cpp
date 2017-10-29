//
// Created by matherno on 6/08/17.
//

#include "Samplers.h"
#include "RaySplayConst.h"

void WhiteNoiseSampler::generateSampleSets() {
  for(int num = 0; num < numSamples * numSampleSets; ++num){
    squareMapSamples.emplace_back(RAND_FLOAT(0,1), RAND_FLOAT(0,1));
  }
}

void BlueNoiseSampler::generateSampleSets() {
  for(int setNum = 0; setNum < numSampleSets; ++setNum){
    for(int y = 0; y < sqrtNumSamples; ++y){
      for(int x = 0; x < sqrtNumSamples; ++x) {
        squareMapSamples.emplace_back( (x+RAND_FLOAT(0,1))/sqrtNumSamples, (y+RAND_FLOAT(0,1))/sqrtNumSamples );
      }
    }
  }
}

void RegularSampler::generateSampleSets() {
  for(int setNum = 0; setNum < numSampleSets; ++setNum){
    for(int y = 0; y < sqrtNumSamples; ++y){
      for(int x = 0; x < sqrtNumSamples; ++x) {
        squareMapSamples.emplace_back( (x+0.5f)/sqrtNumSamples, (y+0.5f)/sqrtNumSamples );
      }
    }
  }
}
