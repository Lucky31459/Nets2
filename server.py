import socket
import threading
import time

lock = threading.Lock()

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

def count_words(text):
    return len(text.split())

def handle_client(conn, addr):
    print(f"[NEW CONNECTION] {addr} connected")
    data = conn.recv(4096).decode()
    start_time = time.time()
    with lock:  # enforce sequential processing
        result = count_words(data)
        time.sleep(0.5)  # simulate processing time
        conn.sendall(str(result).encode())
    end_time = time.time()
    print(f"[DONE] {addr} processed in {end_time - start_time:.3f}s")
    conn.close()

def main():
    config = parse_config("config.json")
    server_ip = config["server_ip"]
    port = config["port"]

    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((server_ip, port))
    server.listen()

    print(f"[LISTENING] Server on {server_ip}:{port}")

    while True:
        conn, addr = server.accept()
        thread = threading.Thread(target=handle_client, args=(conn, addr))
        thread.start()

if __name__ == "__main__":
    main()
