#include "storage.hpp"
using json = nlohmann::json;

void StorageHandler::storeExpense(float amount, const std::string& category, const std::string& description, const std::string& date) {

    // expense in json format
    json expense = {
        {"amount", amount},
        {"category", category},
        {"description", description},
    };

    // open the file for writing
    std::ifstream input_file("../data.json");
    json data; // to store already existent data from the file

    if (input_file.is_open()) {
        input_file >> data;
        input_file.close();
    } else {
        // initialize an empty json object if the file does not exist
        data = json::object();
    }

    data[date].push_back(expense);

    std::ofstream output_file("../data.json");
    output_file << data.dump(4);
    output_file.close();
}
