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
    const char* server_ip = server_ip_a.c_str();
    int port = safe_stoi(config["server_port"], 8080);
    int off = safe_stoi(config["p"], 2);
    int k = safe_stoi(config["k"], 10);

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, server_ip, &serv_addr.sin_addr);

    connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr));
 
    std::string sendstr = std::to_string(off) + "," + std::to_string(k) + "\n"; 
    send(sock, sendstr.c_str(), sendstr.size(), 0);
    char buffer[1024] = {0};
    // read(sock, buffer, 1024);
    // std::cout << "Server says: " << buffer << std::endl;

    std::map<std::string, int> word_freq;
    std::string line, word;
    int count = 0 ;
    int iter = 0 ;
    bool end = false ;
    int word_count = 0 ;
    while (true) {
        if (iter  > 154 ){
            
            break ;
        }
        iter++;
        
        // memset(buffer, 0, sizeof(buffer));
        int r = read(sock, buffer, sizeof(buffer) - 1);
        if (r > 0) {
            buffer[r] = '\0'; // null terminate
        }
        cout<<iter<<" "<<off<<" "<<r<<endl;
        
        std::stringstream ss(buffer); 
        while (std::getline(ss, word, ',')){
            word_count++ ;
            if ( !word.empty() && word.back() == '\n' ) {
                
                word.pop_back();
            }
            // cout<<endl;
            // cout<<word<<endl;
            // cout<<endl;
            
            if(word == "EOF"){
               cout<<"yo"<<endl;
               end = true ;
              break;
            }
           
            
            word_freq[word]++;  
            count++;
            if(count == k ){
              count = 0 ;
              off = off + k ;
              sendstr = std::to_string(off) + "," + std::to_string(k) + "\n"; 
              send(sock, sendstr.c_str(), sendstr.size(), 0);
            }  
      }
       // if (end){
       //   break ;
       // }  
    }
    
    close(sock);
    std::ofstream outfile("output.txt");

   for (const auto& kv : word_freq) {
       const auto& word = kv.first;
       const auto& counted = kv.second;
       std::cout << word << ", " << counted << "\n";
   }
    return 0;
}



















































