import socket
import time

# 客户端数据
client_data = b'Hello, server!'

while True:
    try:
        # 创建TCP连接
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_address = ('192.168.31.104', 1234)
        client_socket.connect(server_address)

        # 发送数据
        client_socket.sendall(client_data)

        # 接收服务器响应
        response = client_socket.recv(1024)

        # 关闭连接
        client_socket.close()

        # 处理服务器响应
        print(response.decode())

    except:
        # 如果连接失败，则等待一段时间后重新连接
        print('Connection failed. Retrying in 5 seconds...')
        time.sleep(5)
