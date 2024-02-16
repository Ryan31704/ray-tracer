#ifndef CAMERA_H
#define CAMERA_H

#include"rtweekend.h"

#include"color.h"
#include"hittable.h"
#include"material.h"

#include<iostream>

class camera
{
  public:
    double aspectRatio = 1.0;
    int imageWidth = 100;
    int samplesPerPixel = 10;
    int maxDepth = 10; //max number of ray bounces 
    double vFov = 90;
    point3 lookfrom = point3(0,0,-1);  // Point camera is looking from
    point3 lookat   = point3(0,0,0);   // Point camera is looking at
    vec3   vup      = vec3(0,1,0);     // Camera-relative "up" direction

    double defocusAngle = 0;  // Variation angle of rays through each pixel
    double focusDist = 10;    // Distance from camera lookfrom point to plane of perfect focus

    void render(const hittable&world)
    {
      initialize();
      std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

// multi threading babyyyyyy
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
    vec3 u, v, w; //camera frame basis vectors
    vec3 defocus_disk_u;  // Defocus disk horizontal radius
    vec3 defocus_disk_v;  // Defocus disk vertical radius

    void initialize()
    {
      imageHeight = static_cast<int>(imageWidth / aspectRatio);
        imageHeight = (imageHeight < 1) ? 1 : imageHeight;

        cameraCenter = lookfrom;

        // Determine viewport dimensions.
        auto theta = degreesToRadians(vFov);
        auto h = tan(theta/2);
        auto viewport_height = 2 * h * focusDist;
        auto viewport_width = viewport_height * (static_cast<double>(imageWidth)/imageHeight);

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
        w = unitVector(lookfrom - lookat);
        u = unitVector(cross(vup, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        vec3 viewport_u = viewport_width * u;    // Vector across viewport horizontal edge
        vec3 viewport_v = viewport_height * -v;  // Vector down viewport vertical edge

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixelDeltaU = viewport_u / imageWidth;
        pixelDeltaV = viewport_v / imageHeight;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = cameraCenter - (focusDist * w) - viewport_u/2 - viewport_v/2;

        pixel00Location = viewport_upper_left + 0.5 * (pixelDeltaU + pixelDeltaV);

        // Calculate the camera defocus disk basis vectors.
        auto defocus_radius = focusDist * tan(degreesToRadians(defocusAngle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }
    ray getRay(int i, int j) const {
    // Get a randomly sampled camera ray for the pixel at location i,j.

    auto pixelCenter = pixel00Location + (i * pixelDeltaU) + (j * pixelDeltaV);
    auto pixelSample = pixelCenter + pixelSample_square();

    auto ray_origin = (defocusAngle <= 0) ? cameraCenter : defocusDiskSample();

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

    point3 defocusDiskSample() const {
      // Returns a random point in the camera defocus disk.
      auto p = randomInUnitDisk();
      return cameraCenter + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    color rayColor(const ray& r, int depth, const hittable& world) const
    {
      if(depth <=0)
        return color(0,0,0);

      hit_record record;
      if(world.hit(r, interval(0.001,infinity), record))
      {
        ray scattered;
        color attenuation;
        if(record.mat->scatter(r, record, attenuation, scattered))
          return attenuation * rayColor(scattered, depth-1, world);
        return color(0,0,0);
      }

      vec3 unitDirection = unitVector(r.direction());
      auto a = 0.5*(unitDirection.y() + 1.0);
      return (1.0 -a) * color(1.0,1.0,1.0) + a*color(0.5,0.7,1.0);
    }
};

#endif