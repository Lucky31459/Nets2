import socket as sk
import select
import time
import json

def parse_config(filename):
    with open(filename, "r") as f:
        return json.load(f)

def count_words(text):
    return len(text.split())

def main():
    config = parse_config("config.json")
    server_ip = config["server_ip"]
    port = config["port"]

    server = sk.socket(sk.AF_INET, sk.SOCK_STREAM)
    server.setsockopt(sk.SOL_SOCKET, sk.SO_REUSEADDR, 1)
    server.bind((server_ip, port))
    server.listen()
    server.setblocking(False)

    print(f"[LISTENING] Server on {server_ip}:{port}")

    clients = []          
    buffers = {}          
    start_times = {}      

    while True:
        readable_server, _, _ = select.select([server], [], [], None)
        for sock in readable_server:
            conn, addr = server.accept()
            conn.setblocking(False)
            clients.append(conn)
            buffers[conn] = b""
            print(f"[NEW CONNECTION] {addr} connected")

        # --- Check for client messages (clients only) ---
        if clients:  # only call select if we have clients
            readable_clients, _, exceptional_clients = select.select(clients, [], clients, None)

            for sock in readable_clients:
                data = sock.recv(4096)
                if data:
                    buffers[sock] += data
                    text = buffers[sock].decode().strip()
                    start_times[sock] = time.time()

                    # process data
                    result = count_words(text)
                    time.sleep(0.5)  # simulate work
                    sock.sendall(str(result).encode())

                    end_time = time.time()
                    print(f"[DONE] {sock.getpeername()} processed in {end_time - start_times[sock]:.3f}s")

                    # cleanup
                    clients.remove(sock)
                    sock.close()
                    buffers.pop(sock, None)
                    start_times.pop(sock, None)
                else:
                    # client disconnected
                    clients.remove(sock)
                    sock.close()
                    buffers.pop(sock, None)
                    start_times.pop(sock, None)

            for sock in exceptional_clients:
                clients.remove(sock)
                sock.close()
                buffers.pop(sock, None)
                start_times.pop(sock, None)

if __name__ == "__main__":
    main()
