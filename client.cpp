#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
// #include "json.hpp" 
#include <string>
#include <cctype>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdlib> 
using namespace std ;

int main(){
   string offset = "10\n" ;
   int k = 10 ;
   int off = atoi(offset.c_str());
   int sock;
   char buffer[1024] = {0};
   struct sockaddr_in serv_addr;

   sock = socket(AF_INET, SOCK_STREAM, 0);
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_port = htons(8080);
   inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
  //  cout<<"yo"<<endl;
   connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  //  cout<<"yo"<<endl;

   send(sock, offset.c_str() , strlen(offset.c_str()), 0);

   bool check = false ;
   std::map<std::string, int> word_freq;
   std::string line, word;
   cout<<"yo"<<endl;
   int count = 0 ;
   while(!check){
      cout<<"yo"<<endl;
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
