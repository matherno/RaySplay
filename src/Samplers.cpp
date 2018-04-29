//
// Created by matherno on 6/08/17.
//

#include "Samplers.h"
#include "RaySplayConst.h"

int shuffleRand(int n) {
  return std::rand() % n;
}

/*
 *  Essentially shuffles the src list, and appends it to the back of the target
 */

void shuffleAndAppend(std::vector<Vector2D>* src, std::vector<Vector2D>* target) {
  std::random_shuffle(src->begin(), src->end(), shuffleRand);
  target->insert(target->end(), src->begin(), src->end());
}

void WhiteNoiseSampler::generateSampleSets() {
  std::vector<Vector2D> samples;
  for(int num = 0; num < numSamples * numSampleSets; ++num){
    samples.emplace_back(RAND_FLOAT(0,1), RAND_FLOAT(0,1));
  }
  shuffleAndAppend(&samples, &squareMapSamples);
}

void BlueNoiseSampler::generateSampleSets() {
  for(int setNum = 0; setNum < numSampleSets; ++setNum){
    std::vector<Vector2D> samples;
    for(int y = 0; y < sqrtNumSamples; ++y){
      for(int x = 0; x < sqrtNumSamples; ++x) {
        samples.emplace_back( (x+RAND_FLOAT(0,1))/sqrtNumSamples, (y+RAND_FLOAT(0,1))/sqrtNumSamples );
      }
    }
    shuffleAndAppend(&samples, &squareMapSamples);
  }
}

void RegularSampler::generateSampleSets() {
  for(int setNum = 0; setNum < numSampleSets; ++setNum){
    std::vector<Vector2D> samples;
    for(int y = 0; y < sqrtNumSamples; ++y){
      for(int x = 0; x < sqrtNumSamples; ++x) {
        samples.emplace_back( (x+0.5f)/sqrtNumSamples, (y+0.5f)/sqrtNumSamples );
      }
    }
    shuffleAndAppend(&samples, &squareMapSamples);
  }
}
