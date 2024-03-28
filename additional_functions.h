#pragma once

#include <iostream>

#include <string>
#include <cmath>

bool StringToInt(size_t& param, std::string& argument);
bool StringToDouble(double& param, std::string& argument, int flag);
double CalculateNumeratorCoefficient(const double& e, double& sigma, int& delta);
