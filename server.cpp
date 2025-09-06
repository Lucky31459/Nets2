#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
// #include "json.hpp" 
#include "string.h"
#include <cctype>
#include <fstream>
#include <sstream>
#include <cstdlib>

// #include <stringstream>

using namespace std ;

int main(){
    // std::ifstream file("config.json");
    // if(!file.is_open()){
    //     cout<<"Could not open the file!"<<endl;
    //     return 0;
    // }
    char buffer[1024] = {0};
    // nlohmann::json j;
    // file >> j;

    // string server_ip = 	j["server_ip"] ;
    // int server_port = j["server_port"] ;
    // int k = j["k"] ;
    // int p = j["p"] ;
    // string input_file = j["input_file"] ;
    // int num_clients = j["num_clients"] ;
    
    //SET

    const char* server_ip = 	"127.0.0.1" ;
    int server_port = 8080 ;
    int k = 10 ;
    int p = 2 ;
    string input_file = "./testcase.txt" ;
    int num_clients = 1 ;

    //Initialising a server socket (Node)
    int srvr = socket(AF_INET, SOCK_STREAM, 0);
                //IPv4(/IPv6) , TCP(/UDP) , TCP default protocol
    
    //set options on a socket
    int opt = 1;
    setsockopt(srvr, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    //socket (here server) , levl of the option (here socket level) , option name (here to reuse the address) , option value , length of option value)
    struct sockaddr_in addr ;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(server_port);  
    addr.sin_addr.s_addr = inet_addr(server_ip);  
    bind(srvr, (struct sockaddr*)&addr, sizeof(addr)); 
    
    //LISTENING

    listen(srvr , 1);

    struct sockaddr_in caddress;
    socklen_t caddrlen = sizeof(caddress); 
    int new_socket = accept(srvr, (struct sockaddr *)&caddress, (socklen_t *)&caddrlen);
 
    //Connected
    read(new_socket, buffer, 1024);
  
    // cout<<buffer<<endl;
    int num = atoi((buffer));
    // cout<<num<<endl;
    if( num % k != 0 ){
         cout<<"yo"<<endl;
         return 0 ;
    }
    // cout<<num<<endl;
    std::ifstream infile(input_file);  
    std::string line;
    int word_count = 0;
    string mystr = "";
    int pth = 0 ;
  
    while (std::getline(infile, line)) {
        std::stringstream ss(line);
        std::string word;
        // cout<<"hello"<<endl;
        while (std::getline(ss, word, ',')) {
            // cout<<word_count<<" "<<num+k<<endl;
            if(word_count == num + k){
                std::string fullstr = mystr + "\n";  // add newline
                cout<<fullstr<<endl;
                send(new_socket, fullstr.c_str(), fullstr.length(), 0);

                pth = 0 ;
                mystr = "";
                read(new_socket, buffer, 1024);
                num = atoi(buffer);
                if( num % k != 0 ){
                    cout<<"yo"<<endl;
                    return 0 ;
                }
            }
            if( word_count >= num &&  word_count < num + k ){
                if( pth == p && word_count < num + k - 1){
                    std::string fullstr = mystr + "\n";  // add newline
                    cout<<fullstr<<" "<<word_count<<" "<<num+k<<endl;
                    send(new_socket, fullstr.c_str(), fullstr.length(), 0);
                    cout<<"yo"<<endl;
                    pth = 0 ;
                    mystr = "";
                }
                if( pth < p){
                    mystr = mystr + word + ",";
                    pth++;
                }
                
            }

            word_count++ ;

        }
    }
    if (word_count < num ){
        std::string fullstr = "$$\n"; 
        send(new_socket, fullstr.c_str(), strlen(fullstr.c_str()), 0);
    }else {
        mystr = mystr + "EOF" + "\n" ;
        send(new_socket, mystr.c_str(), strlen(mystr.c_str()), 0);
    }
    
    
    close(new_socket);
    close(srvr);


    return 0 ;

}
