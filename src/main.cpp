#include "include/common.hpp"
#include "include/my_exception.hpp"
#include "include/color.hpp"
#include "include/vec3.hpp"
#include "include/ray.hpp"
#include "include/hittable.hpp"
#include "include/hittable_list.hpp"
#include "include/sphere.hpp"
#include <concepts>

double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = center - r.origin();
    auto a = dot(r.direction(), r.direction());
    auto b = -2.0 * dot(r.direction(), oc);
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    
    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-b - std::sqrt(discriminant) ) / (2.0*a);
    }
}

color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1,1,1));
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}

// TODO: refactor to have output stream be like tiny-raytracer
// auto render() {

// Function to create output file stream

/**
 * @brief Create a output fstream object
 * 
 * @param argc 
 * @param argv 
 * @param image_width 
 * @param image_height 
 * @return std::ofstream 
 */
std::ofstream create_output_fstream(int &argc, char* argv[], int image_width = 256, int image_height = 256) {
    // Check if the user provided a path to the PPM file
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <output_file_path>\n";
        throw_line("incorrect command line arguments");
    }

    std::ofstream output_file{argv[1]};
    if (!output_file) {
        std::cerr << "file could not be opened for writing!\n";
        throw_line("file could not be opened for writing");
    }
    
    output_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    return output_file;
}

int main(int argc, char* argv[]) {
    
    // Image

    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // Calculate the image height, and ensure that it's at least 1.
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // World

    hittable_list world;

    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    // Camera

    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width)/image_height);
    auto camera_center = point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left = camera_center
                             - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    std::ofstream output_file = create_output_fstream(argc, argv,image_width,image_height);
    

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r, world);
            write_color(output_file, pixel_color);
        }
    }
    std::clog << "\rDone.                 \n";
}
