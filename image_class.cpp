#include "image_class.h"
#include "pixel_class.h"
#include "additional_functions.h"

Image::Image() {
    height_ = 0;
    width_ = 0;

    image_matrix.assign(height_, std::vector<Pixel>(width_));
}

bool Image::OpenBMP(const std::string& path) {
    std::fstream stream;
    stream.open(path, std::ios::in | std::ios::binary);

    if (!stream.is_open()) {
        return false;
    }

    unsigned char bmp_header[BITMAP_HEADER_SIZE];
    stream.read(reinterpret_cast<char*>(bmp_header), BITMAP_HEADER_SIZE);

    if (bmp_header[0] != 'B' || bmp_header[1] != 'M') {
        return false;
    }

    unsigned char dib_header[DIB_HEADER_SIZE];
    stream.read(reinterpret_cast<char*>(dib_header), DIB_HEADER_SIZE);

    for (size_t i = DIB_HEADER_WIDTH_INFORMATION_START; i <= DIB_HEADER_WIDTH_INFORMATION_END; i++) {
        width_ += dib_header[i] << BMP_SHIFTS[i - DIB_HEADER_WIDTH_INFORMATION_START];
    }

    for (size_t i = DIB_HEADER_HEIGHT_INFORMATION_START; i <= DIB_HEADER_HEIGHT_INFORMATION_END; i++) {
        height_ += dib_header[i] << BMP_SHIFTS[i - DIB_HEADER_HEIGHT_INFORMATION_START];
    }

    image_matrix.resize(height_, std::vector<Pixel>(width_));

    const int padding_amount = ((4 - (static_cast<int>(width_) * 3) % 4) + 4) % 4;

    for (int x = static_cast<int>(height_) - 1; x >= 0; x--) {
        for (size_t y = 0; y < width_; y++) {
            unsigned char color[3];
            stream.read(reinterpret_cast<char*>(color), 3);

            image_matrix[x][y].r = static_cast<double>(color[2]) / MAX_COLOR;
            image_matrix[x][y].g = static_cast<double>(color[1]) / MAX_COLOR;
            image_matrix[x][y].b = static_cast<double>(color[0]) / MAX_COLOR;
        }

        stream.ignore(padding_amount);
    }

    stream.close();
    return true;
}

bool Image::WriteBMP(const std::string& path) {
    std::fstream stream;
    stream.open(path, std::ios::out | std::ios::binary);

    if (!stream.is_open()) {
        return false;
    }

    unsigned char bmp_pad[3] = {0, 0, 0};

    const int padding_amount = ((4 - (static_cast<int>(width_) * 3) % 4) + 4) % 4;

    const size_t image_size = BITMAP_HEADER_SIZE + DIB_HEADER_SIZE + width_ * height_ * 3 + padding_amount * width_;

    unsigned char bmp_header[BITMAP_HEADER_SIZE] = {};

    bmp_header[0] = 'B';
    bmp_header[1] = 'M';

    const size_t image_size_information_start = 2;
    const size_t image_size_information_end = 5;
    for (size_t i = image_size_information_start; i <= image_size_information_end; i++) {
        bmp_header[i] = image_size >> BMP_SHIFTS[i - image_size_information_start];
    }

    const size_t offset_size_information = 10;
    bmp_header[offset_size_information] = BITMAP_HEADER_SIZE + DIB_HEADER_SIZE;

    unsigned char dib_header[DIB_HEADER_SIZE] = {};

    dib_header[0] = DIB_HEADER_SIZE;

    for (size_t i = DIB_HEADER_WIDTH_INFORMATION_START; i <= DIB_HEADER_WIDTH_INFORMATION_END; i++) {
        dib_header[i] = width_ >> BMP_SHIFTS[i - DIB_HEADER_WIDTH_INFORMATION_START];
    }

    for (size_t i = DIB_HEADER_HEIGHT_INFORMATION_START; i < DIB_HEADER_HEIGHT_INFORMATION_END; i++) {
        dib_header[i] = height_ >> BMP_SHIFTS[i - DIB_HEADER_HEIGHT_INFORMATION_START];
    }

    const int planes_information = 12;
    const int planes = 1;
    dib_header[planes_information] = planes;

    const int bits_per_pixel_information = 14;
    const int bits_per_pixel = 24;
    dib_header[bits_per_pixel_information] = bits_per_pixel;

    stream.write(reinterpret_cast<char*>(bmp_header), BITMAP_HEADER_SIZE);
    stream.write(reinterpret_cast<char*>(dib_header), DIB_HEADER_SIZE);

    for (int x = static_cast<int>(height_) - 1; x >= 0; x--) {
        for (size_t y = 0; y < width_; y++) {
            unsigned char r =
                static_cast<unsigned char>(std::min(MAX_COLOR_INT, static_cast<int>(image_matrix[x][y].r * MAX_COLOR)));
            unsigned char g =
                static_cast<unsigned char>(std::min(MAX_COLOR_INT, static_cast<int>(image_matrix[x][y].g * MAX_COLOR)));
            unsigned char b =
                static_cast<unsigned char>(std::min(MAX_COLOR_INT, static_cast<int>(image_matrix[x][y].b * MAX_COLOR)));

            unsigned char color[] = {b, g, r};

            stream.write(reinterpret_cast<char*>(color), 3);
        }

        stream.write(reinterpret_cast<char*>(bmp_pad), padding_amount);
    }

    stream.close();
    return true;
}

void Image::ImplementFilterMatrix3D(std::vector<std::vector<Pixel>>& image_copy_matrix,
                                    const std::vector<std::vector<double>>& filter_matrix) {

    for (int x = 0; x < height_; x++) {
        for (int y = 0; y < width_; y++) {
            image_matrix[x][y] = Pixel(0, 0, 0);

            for (int d1 = -1; d1 <= 1; d1++) {
                for (int d2 = -1; d2 <= 1; d2++) {
                    size_t x0 = std::min(static_cast<int>(height_ - 1), std::max(0, x - d1));
                    size_t y0 = std::min(static_cast<int>(width_ - 1), std::max(0, y - d2));

                    image_matrix[x][y] += image_copy_matrix[x0][y0] * filter_matrix[1 - d1][1 - d2];
                }
            }
            image_matrix[x][y].MoreThanNull();
            image_matrix[x][y].LessThanOne();
        }
    }
}

void Image::Crop(size_t& height_crop, size_t& width_crop) {
    image_matrix.resize(std::min(height_, height_crop), std::vector<Pixel>(std::min(width_, width_crop)));

    height_ = std::min(height_, height_crop);
    width_ = std::min(width_, width_crop);
}

void Image::Greyscale() {
    const double coefficients[3] = {0.299, 0.587, 0.114};
    for (size_t i = 0; i < height_; i++) {
        for (size_t j = 0; j < width_; j++) {
            image_matrix[i][j].r = coefficients[0] * image_matrix[i][j].r + coefficients[1] * image_matrix[i][j].g +
                                   coefficients[2] * image_matrix[i][j].b;
            image_matrix[i][j].g = image_matrix[i][j].r;
            image_matrix[i][j].b = image_matrix[i][j].r;
        }
    }
}

void Image::Negative() {
    for (size_t i = 0; i < height_; i++) {
        for (size_t j = 0; j < width_; j++) {
            image_matrix[i][j] = Pixel(1, 1, 1) - image_matrix[i][j];
        }
    }
}

void Image::Sharpening(Image& image_copy) {
    const std::vector<std::vector<double>> sharp_matrix = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};

    ImplementFilterMatrix3D(image_copy.image_matrix, sharp_matrix);
}

void Image::EdgeDetection(Image& image_copy, double& threshold) {
    image_copy.Greyscale();

    const std::vector<std::vector<double>> edge_matrix = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
    ImplementFilterMatrix3D(image_copy.image_matrix, edge_matrix);

    for (size_t x = 0; x < height_; x++) {
        for (size_t y = 0; y < width_; y++) {
            if (image_matrix[x][y].r > threshold) {
                image_matrix[x][y] = Pixel(1, 1, 1);
            } else {
                image_matrix[x][y] = Pixel(0, 0, 0);
            }
        }
    }
}

void Image::GaussianBlur(Image& image_copy, double& sigma) {
    const int limit = static_cast<int>(3 * sigma + 1);
    const double pi = 3.141592653589;
    const double e = 2.718281828459;
    const double denominator_coefficient = 1 / (2 * pi * std::pow(sigma, 2));

    double numerator_coefficients[2 * limit + 1];

    for (int delta = -limit; delta <= limit; delta++) {
        numerator_coefficients[delta + limit] = CalculateNumeratorCoefficient(e, sigma, delta);
    }

    for (int x = 0; x < height_; x++) {
        for (int y = 0; y < width_; y++) {
            image_matrix[x][y] = Pixel();
            for (int y1 = y - limit; y1 < y + limit + 1; y1++) {
                image_matrix[x][y] +=
                    image_copy.image_matrix[x][std::min(static_cast<int>(width_) - 1, std::max(0, y1))] *
                    numerator_coefficients[y1 - y + limit];
            }
        }
    }

    for (int x = 0; x < height_; x++) {
        for (int y = 0; y < width_; y++) {
            image_copy.image_matrix[x][y] = image_matrix[x][y];
        }
    }

    for (int y = 0; y < width_; y++) {
        for (int x = 0; x < height_; x++) {
            image_matrix[x][y] = Pixel();
            for (int x1 = x - limit; x1 < x + limit + 1; x1++) {
                image_matrix[x][y] +=
                    image_copy.image_matrix[std::min(static_cast<int>(height_) - 1, std::max(0, x1))][y] *
                    numerator_coefficients[x1 - x + limit];
            }
            image_matrix[x][y] = image_matrix[x][y] * denominator_coefficient;
            image_matrix[x][y].LessThanOne();
        }
    }
}

void Image::VHSCassette(Image& image_copy) {
    const std::vector<std::vector<double>> vhs_matrix = {{-1, -2, -3}, {-4, -8, -6}, {7, 8, 9}};

    ImplementFilterMatrix3D(image_copy.image_matrix, vhs_matrix);

    const double vhs_coefficient = 0.25;
    for (size_t x = 0; x < height_; x++) {
        for (size_t y = 0; y < width_; y++) {
            image_matrix[x][y] =
                image_copy.image_matrix[x][y] * (1 - vhs_coefficient) + image_matrix[x][y] * vhs_coefficient;
            image_matrix[x][y].LessThanOne();
        }
    }
}
