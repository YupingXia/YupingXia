#服务器端程序
import socket
import sys

#创建对象
serversocket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)

#获取主机名
host = socket.gethostname()
port = 9999

#绑定端口号
serversocket.bind((host,port))
#设置最大连接数目
serversocket.listen(5)

#建立连接

while True:
    clientscoket,addr = serversocket.accept()
    print('连接地址为'f'{addr}')
    msg = '欢迎访问我的服务器' + "\r\n"
    clientscoket.send(msg.encode('utf-8'))
    clientscoket.close()