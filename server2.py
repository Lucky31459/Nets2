import socket as sk
import time 
import json

if __name__ == "__main__" :
  with open("config.json", "r") as jsonfile:
    config = json.load(jsonfile)
    
  ipaddr = config["server_ip"]
  port = config["port"]
  n = config["num_clients"]
  iter = config["num_iterations"]
  words_file = config["filename"]
  words = []
  with open(words_file, "r") as f:
      for line in f:
          line = line.strip()
          if not line:
              continue
          words.extend(line.split(","))
        
  srvr = sk.socket( sk.AF_INET, sk.SOCK_STREAM )
  srvr.setsockopt( sk.SOL_SOCKET, sk.SO_REUSEADDR, 1)
  srvr.bind((ipaddr, port))
  srvr.setblocking(False)
  srvr.listen()  
  clients_array = []          
  num = 0
  
  while num < n :
    new_connections_array, _, _ = select.select([srvr], [], [])
    for new_connection in new_connections_array :
      skt, _ = srvr.accept()
      skt.setblocking(False)
      clients.append(skt)
      num+=1   
      if ( num >= n):
        break
  
  while True :
    recieved_messages_array,_,_ = select.select(clients_array, [],[])
    if ( len(recieved_messages_array) > 0  ):
      for sock in recieved_messages_array:
        data = sock.recv(4096).decode()
        if data:
          parts = data.split(",")
          p, k = int(parts[0]), int(parts[1])
                response = ""
          
          for i in range(k):
                    if p + i >= len(words):
                        response += "EOF\n"
                        break
                    response += words[p + i] + ","
          if not response.endswith("EOF\n"):
                    response = response.rstrip(",") + "\n"
          sock.sendall(response.encode())
          
    else:
      break
    
      
  
