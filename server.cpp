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
// #include <stringstream>

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
    std::map<std::string,std::string> config = parse_json("config_1.json");
    char buffer[1024] = {0};
    std::string server_ip_a = config["server_ip"];
    const char* server_ip = 	server_ip_a.c_str() ;
    std::string input_file = config["input_file"];

    // Numbers
    int server_port = safe_stoi(config["server_port"], 8080);
    int k = safe_stoi(config["k"], 10);
    int p = safe_stoi(config["p"], 2);
    int num_clients = safe_stoi(config["num_clients"], 1);
    
    //SET

    // const char* server_ip = 	"127.0.0.1" ;
    // int server_port = 8080 ;
    // int k = 10 ;
    // int p = 2 ;
    // string input_file = "./input_543.txt" ;
    // int num_clients = 1 ;

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
    cout<<"S106"<<endl;
    int new_socket = accept(srvr, (struct sockaddr *)&caddress, (socklen_t *)&caddrlen);
    cout<<"new_socket:"<<new_socket<<" "<<"S108"<<endl;
    //Connected
    
    read(new_socket, buffer, 1024);
    cout<<"S112"<<endl;
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
        cout<<"hello"<<endl;
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
