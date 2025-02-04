#include <iostream>
#include <fstream>

using namespace std;

int main() {
    // Image dimensions
    const int width  = 500;
    const int height = 500;

    // Open a file in write mode
    ofstream file("image.ppm");

    // PPM Header
    file << "P3\n" << width << " " << height << "\n255\n";

    // Write pixel data (gradient effect)
    for( int y = 0; y < height; y++ ) {
        for( int x = 0; x < width; x++ ) {
            int r = (x * 255) / width;   // Gradient from black to red
            int g = (y * 255) / height;  // Gradient from black to green
            int b = 128;                 // Constant blue value
            
            file << r << " " << g << " " << b << " ";
        }
        file << "\n";
    }

    file.close();
    cout << "Image saved as output.ppm\n";
    return 0;
}