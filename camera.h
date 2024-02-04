#ifndef CAMERA_H
#define CAMERA_H

#include "utils.h"

#include "color.h"
#include "hittable.h"

#include <iostream>

class camera {
    public:
        // Public parameters here
        double aspect_ratio = 1.0;  // Ratio of image width over height
        int img_width = 100;        // Rendered image width in pixel count
        int samples_per_pixel = 10;

        void render(const hittable& world) {
            init();

            std::cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";

            for (int j=0; j < img_height; j++) {
                std::clog << "\rScanlines remaining: " << (img_height - j) << ' ' << std::flush;
                for (int i = 0; i < img_width; i++) {
                    color pixel_color(0,0,0);
                    for (int sample = 0; sample < samples_per_pixel; sample++) {
                        ray r = get_ray(i, j);
                        pixel_color += ray_color(r, world);
                    }
                    write_color(std::cout, pixel_color, samples_per_pixel);
                }
            }

            std::clog << "\rDone.                 \n";
        }

    private:
        // Private camera variables here
        int img_height;         // Rendered image height
        point3 center;          // Camera center
        point3 pixel_00_loc;    // Location of pixel 0,0
        vec3 pixel_delta_u;     // Offset to pixel to the right
        vec3 pixel_delta_v;     // Offset to pixel below

        void init() {
            img_height = static_cast<int>(img_width / aspect_ratio);
            img_height = (img_height < 1 ? 1 : img_height);

            center = point3(0, 0, 0);

            // Determine viewport dimensions
            double focal_length = 1.0;
            double viewport_height = 2.0;
            double viewport_width = viewport_height * (static_cast<double>(img_width)/img_height);

            // Calculate the vectors across the horizontal and down the vertical viewport edges. auto viewport_u = vec3(viewport_width, 0, 0);
            vec3 viewport_u = vec3(viewport_width, 0, 0);
            vec3 viewport_v = vec3(0, -viewport_height, 0);

            // Calculate the horizontal and vertical delta vectors from pixel to pixel
            pixel_delta_u = viewport_u / img_width;
            pixel_delta_v = viewport_v / img_height;

            // Calculate the location of the upper left pixel.
            point3 viewport_upper_left = center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
            pixel_00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
        }

        ray get_ray(int i, int j) const {
            // Get a randomly samples camera ray for the pixel ar location i, j
            point3 pixel_center = pixel_00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto pixel_sample = pixel_center + pixel_sample_square();

            vec3 ray_origin = center;
            vec3 ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        vec3 pixel_sample_square() const {
            // Returns a random point in the square surrounding a pixel at the origin
            auto px = -0.5 + random_double();
            auto py = -0.5 + random_double();

            return (px * pixel_delta_u) + (py * pixel_delta_v);
        }

        color ray_color(const ray& r, const hittable& world) const {
            hit_record rec;

            if (world.hit(r, interval(0, infinity), rec)) {
                return 0.5 * (rec.normal + color(1,1,1));
            }

            // Linear Interpolation depending on the ray Y coordinate
            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5 * (unit_direction.y() + 1.0);
            return (1.0-a) * color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
        }
};

#endif
