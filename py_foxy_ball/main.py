import socket
import time
from threading import Thread
from time import sleep

from protocol import Protocol, print_hex


class FoxyBall:
    def __init__(self):
        self.client = socket.socket()
        # 当前连接状态0 未连接 1 已连接未登录 2 已登录
        self.__state = 0
        # 信号量
        self.__signal = 0
        # 返回值
        self.__return_data = dict()

    def connect(self, ip, port=45007):
        """
        连接数据库服务器
        :param ip:数据库ip
        :param port:数据库端口
        :return:
        """
        # 与服务器建立连接
        try:
            self.client.connect((ip, port))
            # 开始接受服务端消息
            thead = Thread(target=self.__msg_handler)
            thead.setDaemon(True)
            thead.start()
            self.__state = 1
        except:
            self.__state = 0

    def __msg_handler(self):
        """
        处理服务端返回的消息
        """
        while True:
            bytes = self.client.recv(4096)
            print_hex(bytes)
            # 以包长度切割封包
            while True:
                # 读取包长度
                length_pck = int.from_bytes(bytes[:4], byteorder='little')
                # 截取封包
                pck = bytes[4:4 + length_pck]
                # 删除已经读取的字节
                bytes = bytes[4 + length_pck:]
                # 把封包交给处理函数
                self.__pck_handler(pck)
                # 如果bytes没数据了，就跳出循环
                if len(bytes) == 0:
                    break

    def __pck_handler(self, pck):
        p = Protocol(pck)
        pck_type = p.get_str()[:-1]
        if pck_type == 'set_result':
            self.__return_data['protocol'] = pck_type
            self.__return_data['value'] = p.get_int32()
            self.__signal = 0

    def __send_pck(self, pck):
        self.client.sendall(pck)
        self.__signal = 1

    def __wait(self):
        """
        等待服务端返回的数据，同步处理
        :return:
        """
        last_time = time.time()
        while True:
            current_time = time.time()
            # 3秒超时时间
            if current_time - last_time >= 3:
                return False

            sleep(0.001)  # 等待1毫秒

            if self.__signal == 0:
                return True

    def set_k_v(self, k, v):
        """
        在数据库中添加一个kv
        :param k:
        :param v:
        :return:成功1 失败0
        """
        p = Protocol()
        p.add_str("set")
        p.add_str(k)
        p.add_str(v)
        self.__send_pck(p.get_pck_has_head())
        ret = self.__wait()  # 同步
        if ret:
            return self.__return_data['value']
        else:
            raise Exception('等待超时！')


if __name__ == '__main__':
    fb = FoxyBall()
    fb.connect('127.0.0.1')

    while True:
        k = input('请输入key：')
        v = input('请输入value：')
        ret = fb.set_k_v(k, v)
        if ret == 1:
            print('设置成功！')
        else:
            print('设置失败！请检查是否有已存在的key！')
