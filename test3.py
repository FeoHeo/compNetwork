import socket

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(("0.0.0.0", 12347))
server_socket.listen(5)  # Listen for connections

print("Waiting for a connection...")
conn, addr = server_socket.accept()  # Blocking call, waits for a client

print(f"Connected by {addr} of type {type(addr)}")
print(addr)

# Communicate using conn
conn.sendall(b"Hello, Client!")
conn.close()
