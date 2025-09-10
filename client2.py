
import socket
import time

def parse_config(filename):
    config = {}
    with open(filename, "r") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("{") or line.startswith("}"):
                continue
            if ":" in line:
                key, value = line.split(":", 1)
                key = key.strip().strip('"')
                value = value.strip().strip(",").strip('"')
                if value.isdigit():
                    value = int(value)
                config[key] = value
    return config

config = parse_config("config.json")
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
