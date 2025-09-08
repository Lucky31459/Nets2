#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h> 
#include "string.h"
#include <cctype>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <map>
#include <vector>

using namespace std ;
std::map<std::string, std::string> parse_json(const char* filename) {
    std::ifstream file(filename);
    std::map<std::string, std::string> result;
    std::string line;

    while (std::getline(file, line)) {
        // Remove spaces at start/end
        line.erase(0, line.find_first_not_of(" \t\n\r"));
        line.erase(line.find_last_not_of(" \t\n\r") + 1);

        if (line.empty() || line[0]=='{' || line[0]=='}') continue;

        size_t colon = line.find(':');
        if (colon == std::string::npos) continue;

        std::string key = line.substr(0, colon);
        std::string value = line.substr(colon+1);

        // Remove quotes and spaces around key
        key.erase(0, key.find_first_not_of(" \""));
        key.erase(key.find_last_not_of(" \"") + 1);

        // Remove quotes and spaces around value
        value.erase(0, value.find_first_not_of(" \""));
        value.erase(value.find_last_not_of(" \",") + 1); // also remove trailing comma

        result[key] = value;
    }
    return result;
}

// Safe stoi for numeric values
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
    std::string server_ip_a = config["server_ip"];
    std::string input_file = config["filename"];
    // const char* server_ip = server_ip_a.c_str();
    int port = safe_stoi(config["server_port"], 8080);
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    std::ifstream infile(input_file);  
    std::vector<std::string> words;
    std::string line;

    while (std::getline(infile, line)) {
        std::stringstream ss(line);
        std::string token;
        while (std::getline(ss, token, ',')) {  // split by comma
            words.push_back(token);
        }
    }
    infile.close();
    
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // listen on all interfaces
    address.sin_port = htons(port);

    bind(server_fd, (sockaddr*)&address, sizeof(address));
    // // send(client_fd, reply, strlen(reply), 0);

    
    listen(server_fd, 1);

    std::cout << "Server listening on port " << port << "...\n";

    int addrlen = sizeof(address);
    int client_fd = accept(server_fd, (sockaddr*)&address, (socklen_t*)&addrlen);
  
    char buffer[1024] = {0};
    // read(client_fd, buffer, 1024);
    // std::cout << "Received: " << buffer << std::endl;
    // const char* reply = words[0].c_str() ;
    // send(client_fd, reply, strlen(reply), 0);

    int p , k ;
    int len = words.size();
    std::string mystr = "";
    bool last = false ;
    int iter = 0 ;
    std::string word ;
    while (true) {
        // std::cout<<"s"<<iter<<endl;
        if ( iter > 10 ){
            break ;
        }
        iter++;
       
        memset(buffer, 0, sizeof(buffer));
        read(client_fd, buffer, 1024);
        std::stringstream ss(buffer); 
        std::getline(ss, word, ',') ;
        p = safe_stoi(word,1) ;
        std::getline(ss, word, ',') ;
        word.pop_back();
        k = safe_stoi(word, 1) ;
        cout<<p<<" "<<k<<endl;
        for (int i = 0 ; i < k ; i++){
             if ( p+i >= len ){
                mystr = mystr + "EOF\n" ;
                last = true ;
                break ;
             }
             mystr = mystr + word[p+i] + ",";
        }
        if (last){
           send(client_fd, mystr.c_str(), mystr.length(), 0);
            break ;
        }
        mystr.pop_back() ;
        mystr = mystr + "\n";
        send(client_fd, mystr.c_str(), mystr.length(), 0);
        mystr = "";
    }
  
    close(client_fd);
    close(server_fd);
    return 0;
}
































