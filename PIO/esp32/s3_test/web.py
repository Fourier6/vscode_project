import socket

# 创建TCP连接
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_address = ('192.168.31.104', 1234)
server_socket.bind(server_address)
server_socket.listen(1)

while True:
    # 等待客户端连接
    client_socket, client_address = server_socket.accept()

    # 接收客户端数据
    client_data = client_socket.recv(1024)

    # 处理客户端数据
    server_data = b'Hello, client!'

    # 发送响应数据
    client_socket.sendall(server_data)

    # 关闭连接
    client_socket.close()
