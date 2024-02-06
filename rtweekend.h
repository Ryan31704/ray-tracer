#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include<cmath>
#include<cstdlib>
#include<limits>
#include<memory>
#include<random>

//usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

//constanst
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

//utility functions

inline double degreesToRadians(double degrees)
{
  return degrees * pi/ 180.0;
}
inline double randomDouble()
{
  // Returns a random real in [0,1)
  static std::uniform_real_distribution<double> distribution(0.0, 1.0);
  static std::mt19937 generator;
  return distribution(generator);
}

inline double randomDouble(double min, double max)
{
  //returns a random real in [min,max)
  return min + (max - min)*randomDouble();
}

//common headers

#include"interval.h"
#include "ray.h"
#include"vec3.h"

#endif