#include "pixel_class.h"

Pixel::Pixel() {
    r = 0;
    g = 0;
    b = 0;
}

void Pixel::MoreThanNull() {
    r = std::max(static_cast<double>(0), r);
    g = std::max(static_cast<double>(0), g);
    b = std::max(static_cast<double>(0), b);
}

void Pixel::LessThanOne() {
    r = std::min(static_cast<double>(1), r);
    g = std::min(static_cast<double>(1), g);
    b = std::min(static_cast<double>(1), b);
}

Pixel Pixel::operator+(const Pixel& px1) const {
    Pixel px2;
    px2.r = r + px1.r;
    px2.g = g + px1.g;
    px2.b = b + px1.b;
    return px2;
}

Pixel Pixel::operator-(const Pixel& px1) const {
    Pixel px2;
    px2.r = r - px1.r;
    px2.g = g - px1.g;
    px2.b = b - px1.b;
    return px2;
}

Pixel Pixel::operator*(const double& mlt) const {
    Pixel px2;
    px2.r = r * mlt;
    px2.g = g * mlt;
    px2.b = b * mlt;
    return px2;
}

void Pixel::operator+=(const Pixel& px1) {
    r += px1.r;
    g += px1.g;
    b += px1.b;
}
