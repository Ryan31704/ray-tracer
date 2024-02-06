#ifndef CAMERA_H
#define CAMERA_H

#include"rtweekend.h"
#include"color.h"
#include"hittable.h"

#include<iostream>

class camera
{
  public:
    double aspectRatio = 1.0;
    int imageWidth = 100;
    void render(const hittable&world)
    {
      initialize();
      std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

      for(int j = 0; j < imageHeight; j++)
      {
        std::clog << "\rScanlines: " << (imageHeight - j) << ' ' << std::flush;
        for(int i = 0; i < imageWidth; i++)
        {
          auto pixelCenter = pixel00Location + (i * pixelDeltaU) + (j * pixelDeltaV);
          auto rayDirection = pixelCenter - cameraCenter;
          ray r(cameraCenter, rayDirection);

          //assigns pixel to color based off ray
          color pixelColor = rayColor(r, world);

          writeColor(std::cout, pixelColor);
        }
      }
      std::clog << "\rDone.                 \n";
    }
  private:
    int imageHeight;   // Rendered image height
    point3 cameraCenter;         // Camera cameraCenter
    point3 pixel00Location;    // Location of pixel 0, 0
    vec3 pixelDeltaU;  // Offset to pixel to the right
    vec3 pixelDeltaV;  // Offset to pixel below
    void initialize()
    {
      imageHeight = static_cast<int>(imageWidth / aspectRatio);
        imageHeight = (imageHeight < 1) ? 1 : imageHeight;

        cameraCenter = point3(0, 0, 0);

        // Determine viewport dimensions.
        auto focal_length = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (static_cast<double>(imageWidth)/imageHeight);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixelDeltaU = viewport_u / imageWidth;
        pixelDeltaV = viewport_v / imageHeight;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left =
            cameraCenter - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
        pixel00Location = viewport_upper_left + 0.5 * (pixelDeltaU + pixelDeltaV);
    }

    color rayColor(const ray& r, const hittable& world) const
    {
      hit_record record;
      if(world.hit(r, interval(0,infinity), record))
      {
        return 0.5 * (record.normal + color(1,1,1));
      }

      vec3 unitDirection = unitVector(r.direction());
      auto a = 0.5*(unitDirection.y() + 1.0);
      return (1.0 -a) * color(1.0,1.0,1.0) + a*color(0.5,0.7,1.0);
    }
};

#endif