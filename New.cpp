#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
#include <cctype>

// Parse flat JSON file into a map
std::map<std::string, std::string> parse_json(const char* filename) {
    std::ifstream file(filename);
    std::map<std::string, std::string> result;
    std::string line;

    while (std::getline(file, line)) {
        // Remove spaces
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

        if (line.empty() || line[0]=='{' || line[0]=='}') continue;

        size_t colon = line.find(':');
        if (colon == std::string::npos) continue;

        std::string key = line.substr(0, colon);
        std::string value = line.substr(colon+1);

        // Remove quotes from key
        key.erase(std::remove(key.begin(), key.end(), '\"'), key.end());

        // Remove quotes from value if present (keep characters for strings like IP or file paths)
        if (!value.empty() && value.front()=='\"') value.erase(value.begin());
        if (!value.empty() && value.back()=='\"') value.pop_back();

        // Remove trailing comma
        if (!value.empty() && value.back()==',') value.pop_back();

        result[key] = value;
    }
    return result;
}

// Safe integer conversion
int safe_stoi(const std::string& s, int default_val=0) {
    try {
        std::string t;
        for (char c : s) if (isdigit(c) || c=='-' || c=='+') t += c;
        if (t.empty()) return default_val;
        return std::stoi(t);
    } catch (...) {
        return default_val;
    }
}

int main() {
    std::map<std::string,std::string> config = parse_json("config.json");

    // Print all key-value pairs
    for (std::map<std::string,std::string>::iterator it=config.begin(); it!=config.end(); ++it)
        std::cout << it->first << " = " << it->second << "\n";

    // Strings: keep as-is
    std::string server_ip = config["server_ip"];
    std::string input_file = config["input_file"];

    // Numbers: safely converted
    int server_port = safe_stoi(config["server_port"], 8080);
    int k = safe_stoi(config["k"], 10);
    int p = safe_stoi(config["p"], 2);
    int num_clients = safe_stoi(config["num_clients"], 1);

    std::cout << "\nServer IP: " << server_ip << "\n";
    std::cout << "Server Port: " << server_port << "\n";
    std::cout << "k: " << k << "\n";
    std::cout << "p: " << p << "\n";
    std::cout << "Input File: " << input_file << "\n";
    std::cout << "Num Clients: " << num_clients << "\n";

    return 0;
}
