#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <string>
#include <cctype>
#include <fstream>
#include <sstream>

#include <map>
#include <algorithm>

#include <cstdlib> 
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

int main(){

   char buffer[1024] = {0};
  
   std::map<std::string,std::string> config = parse_json("config_1.json");
   std::string server_ip = config["server_ip"];
   int server_port = safe_stoi(config["server_port"], 8080);
    
   int off = safe_stoi(config["p"], 2);
   int k = safe_stoi(config["k"], 10);
    
   int sock;
   
   struct sockaddr_in serv_addr;

   sock = socket(AF_INET, SOCK_STREAM, 0);
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_port = htons(8080);
   inet_pton(AF_INET, server_ip.c_str() , &serv_addr.sin_addr);
  //  cout<<"yo"<<endl;
   connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  //  cout<<"yo"<<endl;

   send(sock, off.c_str() , strlen(off.c_str()), 0);

   bool check = false ;
   std::map<std::string, int> word_freq;
   std::string line, word;
   cout<<"yo"<<endl;
   int count = 0 ;
   while(!check){
      cout<<"yo"<<endl;
       cout<<count<<endl;
      recv(sock, buffer, sizeof(buffer), 0);
      std::stringstream ss(buffer);
      if(buffer == "$$"){
        cout<<"yo"<<endl;
        return 0 ;
      }
      while (std::getline(ss, word, ',')){
        cout<<word<<endl;
   
        if(word == "EOF"){
          cout<<"EOF"<<endl;
          break;
        }
        word_freq[word]++;  
        count++;
        if(count == k ){
          count = 0 ;
          int new_off = off + k ;
          off = new_off ;
          string new_offset = to_string(new_off)+"\n";
          send(sock, new_offset.c_str() , strlen(new_offset.c_str()), 0);
        }  
      }

   }
  //  cout<<"yo"<<endl;
   close(sock);
   std::ofstream outfile("output.txt");

   for (const auto& kv : word_freq) {
       const auto& word = kv.first;
       const auto& counted = kv.second;
       outfile << word << ", " << counted << "\n";
   }


  
   return 0;
 }
