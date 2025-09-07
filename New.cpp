#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <algorithm>

// Simple JSON parser for key-value pairs (strings and numbers only)
std::map<std::string, std::string> parse_json(const char* filename) {
    std::ifstream file(filename);
    std::map<std::string, std::string> result;

    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << "\n";
        return result;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    // Remove spaces, newlines, braces
    content.erase(std::remove_if(content.begin(), content.end(),
                                 [](unsigned char c) { return std::isspace(c); }),
                  content.end());
    if (!content.empty() && content.front() == '{') content.erase(content.begin());
    if (!content.empty() && content.back() == '}') content.pop_back();

    // Split on commas
    std::stringstream ss(content);
    std::string pair;
    while (std::getline(ss, pair, ',')) {
        auto colon_pos = pair.find(':');
        if (colon_pos == std::string::npos) continue;

        std::string key = pair.substr(0, colon_pos);
        std::string value = pair.substr(colon_pos + 1);

        // Remove quotes from key
        key.erase(std::remove(key.begin(), key.end(), '\"'), key.end());

        // Remove quotes from value if present
        if (!value.empty() && value.front() == '\"') value.erase(value.begin());
        if (!value.empty() && value.back() == '\"') value.pop_back();

        result[key] = value;
    }

    return result;
}

int main() {
    auto config = parse_json("config.json");

    for (const auto& [key, value] : config) {
        std::cout << key << " = " << value << "\n";
    }

    // Example: access values
    if (config.find("server_ip") != config.end()) {
        std::string ip = config["server_ip"];
        std::cout << "Server IP: " << ip << "\n";
    }
    if (config.find("server_port") != config.end()) {
        int port = std::stoi(config["server_port"]);
        std::cout << "Server Port: " << port << "\n";
    }
}
