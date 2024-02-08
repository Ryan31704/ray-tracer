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
    int samplesPerPixel = 10;
    int maxDepth = 10; //max number of ray bounces 
    double reflectance = 0.5;

    void render(const hittable&world)
    {
      initialize();
      std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

      for(int j = 0; j < imageHeight; j++)
      {
        std::clog << "\rScanlines: " << (imageHeight - j) << ' ' << std::flush;
        for(int i = 0; i < imageWidth; i++)
        {
          color pixel_color(0,0,0);
          for (int sample = 0; sample < samplesPerPixel; ++sample) {
            ray r = getRay(i, j);
            pixel_color += rayColor(r, maxDepth, world);
          }
          writeColor(std::cout, pixel_color, samplesPerPixel);
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
    ray getRay(int i, int j) const {
    // Get a randomly sampled camera ray for the pixel at location i,j.

    auto pixelCenter = pixel00Location + (i * pixelDeltaU) + (j * pixelDeltaV);
    auto pixelSample = pixelCenter + pixelSample_square();

    auto rayOrigin = cameraCenter;
    auto ray_direction = pixelSample - rayOrigin;

    return ray(rayOrigin, ray_direction);
    }

    vec3 pixelSample_square() const {
    // Returns a random point in the square surrounding a pixel at the origin.
    auto px = -0.5 + randomDouble();
    auto py = -0.5 + randomDouble();
    return (px * pixelDeltaU) + (py * pixelDeltaV);
    }

    color rayColor(const ray& r, int depth, const hittable& world) const
    {
      if(depth <=0)
        return color(0,0,0);

      hit_record record;
      if(world.hit(r, interval(0.001,infinity), record))
      {
        vec3 direction = record.normal + randomUnitVector();
        //more reflectance is more white
        return reflectance * rayColor(ray(record.p, direction), maxDepth,world);
      }

      vec3 unitDirection = unitVector(r.direction());
      auto a = 0.5*(unitDirection.y() + 1.0);
      return (1.0 -a) * color(1.0,1.0,1.0) + a*color(0.5,0.7,1.0);
    }
};

#endif