#include <iostream>
#include <fstream>
#include <string>
#include <map>

std::map<std::string, std::string> parse_json(const char* filename) {
    std::ifstream file(filename);
    std::map<std::string, std::string> result;
    std::string line;

    while (std::getline(file, line)) {
        // remove spaces
        line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());

        if (line.empty() || line[0] == '{' || line[0] == '}')
            continue;

        // split on ':'
        size_t colon = line.find(':');
        if (colon == std::string::npos) continue;

        std::string key = line.substr(0, colon);
        std::string value = line.substr(colon + 1);

        // remove quotes and commas
        key.erase(remove(key.begin(), key.end(), '\"'), key.end());
        value.erase(remove(value.begin(), value.end(), '\"'), value.end());
        if (!value.empty() && value.back() == ',') value.pop_back();

        result[key] = value;
    }

    return result;
}

int main() {
    auto config = parse_json("config.json");

    for (auto &kv : config) {
        std::cout << kv.first << " = " << kv.second << "\n";
    }
}
