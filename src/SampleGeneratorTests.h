#pragma once
//
// Created by matherno on 17/08/17.
//

#include "SampleGenerator.h"
#include "ImageOutput.h"

namespace SampleGeneratorTests{

//  will output the next sample set from the given sample generator through the given ImageOutput
//  puts white pixels on sample points using nearest neighbour filtering
void OutputSamples_UnitSquare(ImageOutput* output, SampleGenerator* sampleGenerator, int width = 300, int height = 300);
void OutputSamples_UnitCircle(ImageOutput* output, SampleGenerator* sampleGenerator, int width = 300, int height = 300);
void OutputSamples_UnitHemisphereXY(ImageOutput* output, SampleGenerator* sampleGenerator, int width = 300, int height = 300);
void OutputSamples_UnitHemisphereXZ(ImageOutput* output, SampleGenerator* sampleGenerator, int width = 300, int height = 300);

}
