#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <algorithm>

std::map<std::string, std::string> parse_json(const std::string &filename) {
    std::ifstream file(filename);
    std::map<std::string, std::string> result;
    std::string line;

    while (std::getline(file, line)) {
        // remove spaces
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

        if (line.empty() || line[0] == '{' || line[0] == '}')
            continue;

        // split by ':'
        size_t colon = line.find(':');
        if (colon == std::string::npos) continue;

        std::string key = line.substr(0, colon);
        std::string value = line.substr(colon + 1);

        // remove quotes and commas
        auto clean = [](std::string s) {
            if (!s.empty() && s.front() == '"') s.erase(0, 1);
            if (!s.empty() && s.back() == '"') s.pop_back();
            if (!s.empty() && s.back() == ',') s.pop_back();
            return s;
        };

        key = clean(key);
        value = clean(value);

        result[key] = value;
    }

    return result;
}

int main() {
    auto config = parse_json("config.json");

    std::cout << "server_ip = " << config["server_ip"] << "\n";
    std::cout << "server_port = " << config["server_port"] << "\n";
    std::cout << "k = " << config["k"] << "\n";
    std::cout << "p = " << config["p"] << "\n";
    std::cout << "input_file = " << config["input_file"] << "\n";
    std::cout << "num_clients = " << config["num_clients"] << "\n";

    return 0;
}
