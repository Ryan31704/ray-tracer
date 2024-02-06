#ifndef COLOR_H
#define COLOR_H

#include"vec3.h"

#include<iostream>

using color = vec3;

void writeColor(std::ostream &out, color pixelColor, int samplesPerPixel)
{
  auto r = pixelColor.x();
  auto g = pixelColor.y();
  auto b = pixelColor.z();

  auto scale = 1.0 / samplesPerPixel;
  r *= scale;
  g *= scale;
  b *= scale;
  // Write the translated [0,255] value of each color component.
  static const interval intensity(0.000,0.999);
  out 
  << static_cast<int>(255.99 * intensity.clamp(r)) << ' '
  << static_cast<int>(255.99 * intensity.clamp(g)) << ' '
  << static_cast<int>(255.99 * intensity.clamp(b)) << '\n';
}

#endif