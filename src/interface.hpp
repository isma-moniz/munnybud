#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "transaction.hpp"

#include "json.hpp"
#include <iostream>
#include <vector>

int runInterface();

void printResults(std::vector<Transaction>& results);

#endif