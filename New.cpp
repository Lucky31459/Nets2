#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>

std::map<std::string, std::string> parse_json(const char* filename) {
    std::ifstream file(filename);
    std::map<std::string, std::string> result;
    std::string line;

    while (std::getline(file, line)) {
        // Remove spaces
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

        if (line.empty() || line[0] == '{' || line[0] == '}')
            continue;

        // Split on ':'
        size_t colon = line.find(':');
        if (colon == std::string::npos) continue;

        std::string key = line.substr(0, colon);
        std::string value = line.substr(colon + 1);

        // Remove quotes and trailing commas
        key.erase(std::remove(key.begin(), key.end(), '\"'), key.end());
        value.erase(std::remove(value.begin(), value.end(), '\"'), value.end());
        if (!value.empty() && value.back() == ',') value.pop_back();

        result[key] = value;
    }

    return result;
}

int main() {
    std::map<std::string, std::string> config = parse_json("config.json");

    // Print all key-value pairs
    for (std::map<std::string, std::string>::iterator it = config.begin(); it != config.end(); ++it) {
        std::cout << it->first << " = " << it->second << "\n";
    }

    // Example: Access specific values
    std::string server_ip = config["server_ip"];
    int server_port = std::stoi(config["server_port"]);
    int k = std::stoi(config["k"]);
    int p = std::stoi(config["p"]);
    std::string input_file = config["input_file"];
    int num_clients = std::stoi(config["num_clients"]);

    std::cout << "\nServer IP: " << server_ip << "\n";
    std::cout << "Server Port: " << server_port << "\n";
    std::cout << "k: " << k << "\n";
    std::cout << "p: " << p << "\n";
    std::cout << "Input File: " << input_file << "\n";
    std::cout << "Num Clients: " << num_clients << "\n";

    return 0;
}
