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
  
   std::map<std::string,std::string> config = parse_json("config.json");
   std::string server_ip = config["server_ip"];
   int server_port = safe_stoi(config["server_port"], 8080);
    
   int off = safe_stoi(config["p"], 2);
   int k = safe_stoi(config["k"], 10);
    
   int sock;
   
   struct sockaddr_in serv_addr;

   sock = socket(AF_INET, SOCK_STREAM, 0);
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_port = htons(server_port);
   int ipt = inet_pton(AF_INET, server_ip.c_str() , &serv_addr.sin_addr);
   cout<<"C82"<<" "<<"sock, ipt :"<<sock<<","<<ipt<<endl;

   int con = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
   cout<<"C85"<<"con: "<<con<<endl;
   printf("Connecting to %s:%d\n",
       inet_ntoa(serv_addr.sin_addr),   // IP address in dotted form
       ntohs(serv_addr.sin_port));      // Port in host byte order
    
   const char* offset = std::to_string(off).c_str();
   int sn  = send(sock, offset , strlen(offset), 0);
   cout<<"C90"<<"sn: "<<sn<<endl;
   bool check = false ;
   std::map<std::string, int> word_freq;
   std::string line, word;
 
   int count = 0 ;
   int iter = 0;
   while(!check){
       iter++;
       // cout<<iter<<endl;
      
      if ( count > 100 || iter > 1){
          break ;
          cout<<count<<"Hi"<<endl;
      }
      recv(sock, buffer, sizeof(buffer), 0);
      cout<<buffer<<endl;
      std::stringstream ss(buffer);
      if(buffer == "$$"){
        cout<<"yo"<<endl;
        return 0 ;
      }
      while (std::getline(ss, word, ',')){
        if ( count > 100){
          break ;
          cout<<count<<"Hi2"<<endl;
        }
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
