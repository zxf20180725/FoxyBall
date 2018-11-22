import socket
from threading import Thread

from protocol import Protocol, print_hex


class FoxyBall:
    def __init__(self):
        self.client = socket.socket()

    def connect(self, ip, port=45007):
        """
        连接数据库服务器
        :param ip:数据库ip
        :param port:数据库端口
        :return:
        """
        # 与服务器建立连接
        self.client.connect((ip, port))

        # 开始接受服务端消息
        thead = Thread(target=self.msg_handler)
        thead.setDaemon(True)
        thead.start()

    def msg_handler(self):
        """
        处理服务端返回的消息
        """
        while True:
            bytes = self.client.recv(4096)
            print_hex("接收到的数据包：",bytes)
            # 以包长度切割封包
            while True:
                # 读取包长度
                length_pck = int.from_bytes(bytes[:4], byteorder='little')
                # 截取封包
                pck = bytes[4:4 + length_pck]
                # 删除已经读取的字节
                bytes = bytes[4 + length_pck:]
                # 把封包交给处理函数
                self.pck_handler(pck)
                # 如果bytes没数据了，就跳出循环
                if len(bytes) == 0:
                    break

    def pck_handler(self, pck):
        p = Protocol(pck)
        pck_type = p.get_str()
        if pck_type == 'set_result\0':
            print(p.get_int32())

    def send_pck(self, pck):
        self.client.sendall(pck)


if __name__ == '__main__':
    foxyball = FoxyBall()
    foxyball.connect('127.0.0.1')
    p = Protocol()
    p.add_str('set')
    p.add_str('json3')
    p.add_str('{"name":"赵雄飞","age":18}')
    foxyball.send_pck(p.get_pck_has_head())
    input("")
