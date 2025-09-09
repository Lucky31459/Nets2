import socket
import threading
import json
import time

lock = threading.Lock()

def count_words(text):
    return len(text.split())

def handle_client(conn, addr):
    print(f"[NEW CONNECTION] {addr} connected")
    data = conn.recv(4096).decode()
    start_time = time.time()
    with lock:   # enforce one-at-a-time processing
        result = count_words(data)
        time.sleep(0.5)  # simulate processing time
        conn.sendall(str(result).encode())
    end_time = time.time()
    print(f"[DONE] {addr} processed in {end_time - start_time:.3f}s")
    conn.close()

def main():
    with open("config.json") as f:
        config = json.load(f)

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
