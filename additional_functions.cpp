#include "additional_functions.h"

bool StringToInt(size_t& param, std::string& argument) {
    try {
        param = stoi(argument);

        if (param < 0) {
            throw;
        }
    } catch (...) {
        std::cout << argument << " is not valid parameter for the filter" << std::endl;
        return false;
    }

    return true;
}

bool StringToDouble(double& param, std::string& argument, int flag) {
    try {
        param = stod(argument);

        if (param < 0 || (param > 1 && flag != 1)) {
            throw;
        }
    } catch (...) {
        std::cout << argument << " is not valid parameter for the filter" << std::endl;
        return false;
    }

    return true;
}

double CalculateNumeratorCoefficient(const double& e, double& sigma, int& delta) {
    double coefficient = std::pow(e, -(std::pow(delta, 2) / (2 * std::pow(sigma, 2))));

    return coefficient;
}
