#include <iostream>
#include <fstream>
#include "../utils/pixels.hpp"

// that's how i should work with images

int main() {
    std::ofstream file;
    file.open("image.ppm");

    TPixel a(2, 2, 2);
    TPicture pic(300, 200);

    pic[0][1] = a;
    file << pic << std::endl;

    file.close();
    return 0;
}