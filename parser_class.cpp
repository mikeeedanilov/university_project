#include "image_class.h"
#include "parser_class.h"
#include "additional_functions.h"

Parser::Parser(Image& image, int& argc, char** argv) : image(image) {
    filters_and_arguments_size = argc - 3;
    pointer = 0;
    for (int i = 3; i < argc; i++) {
        filters_and_arguments.push_back(static_cast<std::string>(argv[i]));
    }
}

bool Parser::ParseArguments() {
    while (pointer < filters_and_arguments_size) {

        if (filters_and_arguments[pointer] == "-crop") {
            if (!CallCrop()) {
                return false;
            }
        } else if (filters_and_arguments[pointer] == "-gs") {
            if (!CallGreyscale()) {
                return false;
            }
        } else if (filters_and_arguments[pointer] == "-neg") {
            if (!CallNegative()) {
                return false;
            }
        } else if (filters_and_arguments[pointer] == "-sharp") {
            if (!CallSharpening()) {
                return false;
            }
        } else if (filters_and_arguments[pointer] == "-edge") {
            if (!CallEdgeDetection()) {
                return false;
            }
        } else if (filters_and_arguments[pointer] == "-blur") {
            if (!CallGaussianBlur()) {
                return false;
            }
        } else if (filters_and_arguments[pointer] == "-vhs") {
            if (!CallVHSCassette()) {
                return false;
            }
        } else {
            return false;
        }
        pointer++;
    }

    return true;
}

bool Parser::CallCrop() {
    size_t height_crop = 0;
    size_t width_crop = 0;
    if (pointer + 2 >= filters_and_arguments_size || !StringToInt(height_crop, filters_and_arguments[pointer + 1]) ||
        !StringToInt(width_crop, filters_and_arguments[pointer + 2])) {
        return false;
    }

    image.Crop(height_crop, width_crop);

    pointer += 2;
    return true;
}

bool Parser::CallGreyscale() {
    image.Greyscale();

    return true;
}

bool Parser::CallNegative() {
    image.Negative();

    return true;
}

bool Parser::CallSharpening() {
    Image image_copy = image;
    image.Sharpening(image_copy);

    return true;
}

bool Parser::CallEdgeDetection() {
    double threshold = 0;

    if (pointer + 1 >= filters_and_arguments_size ||
        !StringToDouble(threshold, filters_and_arguments[pointer + 1], 0)) {
        return false;
    }

    Image image_copy = image;
    image.EdgeDetection(image_copy, threshold);

    pointer += 1;
    return true;
}

bool Parser::CallGaussianBlur() {
    double sigma = 0;

    if (pointer + 1 >= filters_and_arguments_size || !StringToDouble(sigma, filters_and_arguments[pointer + 1], 1)) {
        return false;
    }

    Image image_copy = image;
    image.GaussianBlur(image_copy, sigma);

    pointer += 1;
    return true;
}

bool Parser::CallVHSCassette() {
    Image image_copy = image;
    image.VHSCassette(image_copy);

    return true;
}
