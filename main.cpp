#include"color.h"
#include"ray.h"
#include"vec3.h"

#include<iostream>
color rayColor(const Ray& r)
{
  Vec3 unitDirection = unitVector(r.direction());
  auto a = 0.5*(unitDirection.y() + 1.0);
  return (1-a) * color(1.0,1.0,1.0) + a*color(0.5,0.7,1.0);
}

int main()
{
  auto aspectRatio = 16.0 / 9.0;
  int imageWidth = 400;
  //calculate height based on widht and aspect ratio
  int imageHeight = static_cast<int>(imageWidth / aspectRatio);
  // if image height is less than 1
  imageHeight = (imageHeight < 1) ? 1 : imageHeight;
  
  //Camera stuff
  auto focalLength = 1.0;
  auto viewportHeight = 2.0;
  auto viewportWidth = viewportHeight * (static_cast<double> (imageWidth)/ imageHeight);
  auto cameraCenter = Point3(0,0,0);

  //calc vectors across the horizontal and vertical viewport edges
  auto viewportU = Vec3(viewportWidth, 0, 0);
  auto viewportV = Vec3(0, -viewportWidth, 0);
  // Calculate the horizontal and vertical delta vectors from pixel to pixel.
  auto pixelDeltaU = viewportU / imageWidth;
  auto pixelDeltaV = viewportV / imageHeight;
  // Calculate the location of the upper left pixel.
  auto viewportUpperLeft = cameraCenter - Vec3(0,0,focalLength) - viewportU/2 - viewportV/2;

  auto pixel00Location = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

  //Render

  std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

  for(int j = 0; j < imageHeight; j++)
  {
    std::clog << "\rScanlines: " << (imageHeight - j) << ' ' << std::flush;
    for(int i = 0; i < imageWidth; i++)
    {
      auto pixelCenter = pixel00Location + (i * pixelDeltaU) + (j * pixelDeltaV);
      auto rayDirection = pixelCenter - cameraCenter;
      Ray r(cameraCenter, rayDirection);

      //assigns pixel to color based off ray
      color pixelColor = rayColor(r);
      
      writeColor(std::cout, pixelColor);
    }
  }
  std::clog << "\rDone.                 \n";
}