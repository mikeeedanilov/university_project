#pragma once

#include <vector>
#include <string>

#include "image_class.h"

struct Parser {
    Image& image;
    int filters_and_arguments_size;
    std::vector<std::string> filters_and_arguments;

    size_t pointer;

    Parser(Image& image, int& argc, char** argv);

    bool ParseArguments();

    bool CallCrop();
    bool CallGreyscale();
    bool CallNegative();
    bool CallSharpening();
    bool CallEdgeDetection();
    bool CallGaussianBlur();
    bool CallVHSCassette();
};

const std::string HELP =
    "This is an image processing program. \n"
    "You can imply 5 filters to your image: \n"
    "-crop x y. Crops to the height of x and width of y from the top left corner. X and y should by natural numbers. \n"
    "-gs. Processes the image in grayscale. \n"
    "-neg. Inverts colors. \n"
    "-sharp. Increases sharpness. \n"
    "-edge x. Converts the image to the black and white tones and highlights edges which are brighter than threshold "
    "(x). X should be real number between 0 and 1. \n"
    "-blur x. Blurs the image with coefficient x. X should be real positive number. \n"
    "-vhs. Creates a VHS cassette effect. \n\n"
    "To start program use specific command rule: \n"
    "{path to the program} {path to the input file} {path to the output file} [-{filter}] [{param1} (or no params)] "
    "[param2] [-{filter}] ...";
