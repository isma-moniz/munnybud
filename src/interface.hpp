#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "json.hpp"
#include "iostream"

int runInterface();

void printResults(nlohmann::json::array_t& results);

#endif