#pragma once

#include <fstream>

#include <vector>
#include <string>
#include <cmath>

#include "pixel_class.h"

const double MAX_COLOR = 255;
const int MAX_COLOR_INT = 255;
const int BITMAP_HEADER_SIZE = 14;
const int DIB_HEADER_SIZE = 40;
const size_t DIB_HEADER_WIDTH_INFORMATION_START = 4;
const size_t DIB_HEADER_WIDTH_INFORMATION_END = 7;
const size_t DIB_HEADER_HEIGHT_INFORMATION_START = 8;
const size_t DIB_HEADER_HEIGHT_INFORMATION_END = 11;
const int BMP_SHIFTS[4] = {0, 8, 16, 24};

class Image {
public:
    std::vector<std::vector<Pixel>> image_matrix;

    Image();

    bool OpenBMP(const std::string& path);
    bool WriteBMP(const std::string& path);

    void ImplementFilterMatrix3D(std::vector<std::vector<Pixel>>& image_copy,
                                 const std::vector<std::vector<double>>& filter_matrix);

    void Crop(size_t& height_crop, size_t& width_crop);
    void Greyscale();
    void Negative();
    void Sharpening(Image& image_copy);
    void EdgeDetection(Image& image_copy, double& threshold);
    void GaussianBlur(Image& image_copy, double& sigma);
    void VHSCassette(Image& image_copy);

private:
    size_t width_;
    size_t height_;
};
