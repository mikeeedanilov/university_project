#include <iostream>

#include <vector>
#include <string>

#include "image_class.h"
#include "parser_class.h"

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << HELP << std::endl;
        return 0;
    }

    std::string input_file = argv[1];
    std::string output_file = argv[2];

    Image image;
    if (!image.OpenBMP(input_file)) {
        std::cout << "File can not be opened: wrong path or file is not .bmp" << std::endl;
        return 0;
    };

    Parser parser = Parser(image, argc, argv);
    if (!parser.ParseArguments()) {
        std::cout << "Filter parameters are invalid" << std::endl;
        return 0;
    }

    if (!image.WriteBMP(output_file)) {
        std::cout << "File can not be written: wrong path or internal problem" << std::endl;
    };

    return 0;
}
