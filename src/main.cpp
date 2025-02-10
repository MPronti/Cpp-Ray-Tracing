#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

// 3D Vector class for points and directions
class Vec3 {
public:
    double x, y, z;
    
    Vec3() : x(0), y(0), z(0) {}
    Vec3(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
    
    Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator*(double d) const { return Vec3(x * d, y * d, z * d); }
    Vec3 normalize() const {
        double mg = sqrt(x*x + y*y + z*z);
        return Vec3(x/mg, y/mg, z/mg);
    }
    double dot(const Vec3& v) const { return x*v.x + y*v.y + z*v.z; }
};

// Ray class representing a ray with origin and direction
class Ray {
public:
    Vec3 origin;
    Vec3 direction;
    
    Ray(const Vec3& o, const Vec3& d) : origin(o), direction(d) {}
};

// Sphere class representing a simple sphere in "3D" space
class Sphere {
public:
    Vec3 center;
    double radius;
    Vec3 color;
    
    Sphere(const Vec3& c, double r, const Vec3& col) 
        : center(c), radius(r), color(col) {}
    
    // Check if a ray intersects with the sphere
    bool intersect(const Ray& ray, double& t) const {
        Vec3 oc = ray.origin - center;
        double a = ray.direction.dot(ray.direction);
        double b = 2.0 * oc.dot(ray.direction);
        double c = oc.dot(oc) - radius*radius;
        double discriminant = b*b - 4*a*c;
        
        if (discriminant < 0) return false;
        
        t = (-b - sqrt(discriminant)) / (2.0*a);
        return t > 0;
    }
};

Vec3 trace(const Ray& ray, const vector<Sphere>& spheres) {
    double closest_t = INFINITY;
    const Sphere* hit_sphere = nullptr;
    
    // Find the closest intersection
    for (const auto& sphere : spheres) {
        double t;
        if (sphere.intersect(ray, t) && t < closest_t) {
            closest_t = t;
            hit_sphere = &sphere;
        }
    }
    
    if (hit_sphere) {
        return hit_sphere->color;
    }
    
    // Return background color (sky)
    return Vec3(0.2, 0.3, 0.5);
}

int main() {
    // Image dimensions
    const int width = 400;
    const int height = 400;
    
    // Create a scene with some spheres
    vector<Sphere> spheres;
    spheres.push_back(Sphere(Vec3(0, 0, -5), 2, Vec3(1, 0, 0))); // Red sphere
    spheres.push_back(Sphere(Vec3(2, 0, -6), 2, Vec3(0, 1, 0))); // Green sphere
    spheres.push_back(Sphere(Vec3(-2, 0, -4), 2, Vec3(0, 0, 1))); // Blue sphere
    
    // Camera parameters
    Vec3 camera_pos(0, 0, 0);
    double viewport_height = 2.0;
    double viewport_width = viewport_height * (static_cast<double>(width) / height);
    double focal_length = 1.0;
    
    // Create and open PPM file
    ofstream out("output.ppm");
    out << "P3\n" << width << " " << height << "\n255\n";
    
    // Render loop
    for (int j = height-1; j >= 0; --j) {
        for (int i = 0; i < width; ++i) {
            double u = double(i) / (width-1);
            double v = double(j) / (height-1);
            
            // Calculate ray direction
            Vec3 direction(
                viewport_width * (u - 0.5),
                viewport_height * (v - 0.5),
                -focal_length
            );
            
            Ray ray(camera_pos, direction.normalize());
            Vec3 color = trace(ray, spheres);
            
            // Write color to file
            int r = static_cast<int>(255.999 * color.x);
            int g = static_cast<int>(255.999 * color.y);
            int b = static_cast<int>(255.999 * color.z);
            
            out << r << " " << g << " " << b << "\n";
        }
    }
    
    return 0;
}