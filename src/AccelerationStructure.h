#pragma once
//
// Created by matherno on 13/09/17.
//

#include "geometries/GeometrySet.h"

class AccelerationStructure : public GeometrySet {
public:
  virtual bool init() override {
    if (GeometrySet::init()){
      initialiseStructure();
      return true;
    }
    return false;
  }

protected:
  virtual void initialiseStructure() = 0;
};