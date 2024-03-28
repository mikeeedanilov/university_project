#pragma once

#include <vector>

struct Pixel {
    double r;
    double g;
    double b;

    Pixel();
    Pixel(double r, double g, double b) : r(r), g(g), b(b){};

    void MoreThanNull();
    void LessThanOne();

    Pixel operator+(const Pixel& px1) const;
    Pixel operator-(const Pixel& px1) const;
    Pixel operator*(const double& mlt) const;
    void operator+=(const Pixel& px1);
};
