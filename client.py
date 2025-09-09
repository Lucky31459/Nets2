import socket
import time
import json

with open("config.json") as f:
    config = json.load(f)

server_ip = config["server_ip"]
port = config["port"]

def run_client(text):
    start = time.time()
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((server_ip, port))
    sock.sendall(text.encode())
    result = sock.recv(4096).decode()
    sock.close()
    end = time.time()
    print(f"Result: {result}, Time: {end-start:.3f}s")
    return end - start

if __name__ == "__main__":
    run_client("word counting example with several words")
