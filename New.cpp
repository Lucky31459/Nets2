#include <iostream>
#include <fstream>
#include <string>
#include <map>

int main() {
    std::ifstream f("config.json");
    std::map<std::string,std::string> config;
    std::string key, val, dummy;

    while (f >> dummy) {
        if (dummy[0] == '"') {
            key = dummy.substr(1, dummy.find('"',1)-1);
            f >> dummy; // skip ':'
            f >> val;
            if (val.back() == ',' || val.back() == '}') val.pop_back();
            if (val[0] == '"') val = val.substr(1, val.size()-2);
            config[key] = val;
        }
    }

    for (auto &kv : config)
        std::cout << kv.first << " = " << kv.second << "\n";
}
