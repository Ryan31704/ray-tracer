#ifndef COLOR_H
#define COLOR_H

#include"vec3.h"

#include<iostream>

using color = vec3;

void writeColor(std::ostream &out, color pixelColor)
{
  // Write the translated [0,255] value of each color component.
  out 
  << static_cast<int>(255.99 * pixelColor.x()) << ' '
  << static_cast<int>(255.99 * pixelColor.y()) << ' '
  << static_cast<int>(255.99 * pixelColor.z()) << '\n';
}

#endif