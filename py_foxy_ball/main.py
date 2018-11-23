import socket
import time
from threading import Thread
from time import sleep

from protocol import Protocol, print_hex


class FoxyBall:
    __instance = None

    def __new__(cls, *args, **kwargs):
        """
        单例模式
        """
        if cls.__instance is None:
            cls.__instance = object.__new__(cls)
        return cls.__instance

    def __init__(self):
        if not hasattr(self, 'client'):
            self.client = socket.socket()
        if not hasattr(self, '__state'):
            # 当前连接状态0 未连接 1 已连接未登录 2 已登录
            self.__state = 0
        if not hasattr(self, '__signal'):
            # 信号量
            self.__signal = 0
        if not hasattr(self, '__return_data'):
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
            setattr(self, 'ip', ip)
            setattr(self, 'port', port)
        except:
            self.__state = 0
            self.client.close()

    def __reconnect(self):
        """
        重连
        :return:
        """
        if self.__state == 0 and hasattr(self, 'ip') and hasattr(self, 'port'):
            try:
                self.client.connect((self.ip, self.port))
                # 开始接受服务端消息
                thead = Thread(target=self.__msg_handler)
                thead.setDaemon(True)
                thead.start()
                self.__state = 1
                self.__return_data = dict()
            except:
                self.__state = 0
                self.client.close()

    def __msg_handler(self):
        """
        处理服务端返回的消息
        """
        while True:
            try:
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
            except:
                self.__state = 0
                self.client.close()

    def __pck_handler(self, pck):
        p = Protocol(pck)
        pck_type = p.get_str()[:-1]
        if pck_type == 'set_result':
            self.__return_data['protocol'] = pck_type
            self.__return_data['value'] = p.get_int32()
            self.__signal = 0
        elif pck_type == 'get_result':
            self.__return_data['protocol'] = pck_type
            state = p.get_int32()
            if state == 1:
                s = p.get_str()
                self.__return_data['value'] = [state, s]
            else:
                self.__return_data['value'] = [state, ]
            self.__signal = 0
        elif pck_type == 'del_result':
            self.__return_data['protocol'] = pck_type
            self.__return_data['value'] = p.get_int32()
            self.__signal = 0
        elif pck_type == 'expire_result':
            self.__return_data['protocol'] = pck_type
            self.__return_data['value'] = p.get_int32()
            self.__signal = 0
        elif pck_type == 'save_result':
            self.__return_data['protocol'] = pck_type
            self.__return_data['value'] = p.get_int32()
            self.__signal = 0

    def __send_pck(self, pck):
        if self.__state == 1:
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
                self.__state = 0
                self.client.close()
                raise Exception('等待超时！')

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
        self.__reconnect()
        p = Protocol()
        p.add_str("set")
        p.add_str(k)
        p.add_str(v)
        self.__send_pck(p.get_pck_has_head())
        self.__wait()  # 同步
        return self.__return_data['value']

    def get_v(self, k):
        self.__reconnect()
        p = Protocol()
        p.add_str("get")
        p.add_str(k)
        self.__send_pck(p.get_pck_has_head())
        self.__wait()  # 同步
        if self.__return_data['value'][0] == 0:
            return 0
        return self.__return_data['value'][1][:-1]

    def del_k(self, k):
        self.__reconnect()
        p = Protocol()
        p.add_str("del")
        p.add_str(k)
        self.__send_pck(p.get_pck_has_head())
        self.__wait()  # 同步
        return self.__return_data['value']

    def expire_k(self, k, expire):
        self.__reconnect()
        p = Protocol()
        p.add_str("expire")
        p.add_str(k)
        p.add_int32(expire)
        self.__send_pck(p.get_pck_has_head())
        self.__wait()  # 同步
        return self.__return_data['value']

    def save(self):
        self.__reconnect()
        p = Protocol()
        p.add_str("save")
        self.__send_pck(p.get_pck_has_head())
        self.__wait()  # 同步
        return self.__return_data['value']


if __name__ == '__main__':
    fb = FoxyBall()
    fb.connect('127.0.0.1')

    while True:
        cmd = input('请输入命令：')
        if cmd == 'set':
            k = input('请输入key：')
            v = input('请输入value：')
            ret = fb.set_k_v(k, v)
            if ret == 1:
                print('设置成功！')
            else:
                print('设置失败！请检查是否有已存在的key！')
        elif cmd == 'get':
            k = input('请输入key：')
            ret = fb.get_v(k)
            if ret == 0:
                print('未找到！')
            else:
                print(ret)
        elif cmd == 'del':
            k = input('请输入key：')
            ret = fb.del_k(k)
            if ret == 1:
                print('删除成功！')
            else:
                print('删除失败！请检查是否有已存在的key！')
        elif cmd == 'expire':
            k = input('请输入key：')
            expire = int(input('请输入过期时间：'))
            ret = fb.expire_k(k, expire)
            if ret == 1:
                print('过期时间设置成功！')
            else:
                print('设置失败！请检查key是否存在！')
        elif cmd == 'save':
            ret = fb.save()
            if ret == 1:
                print('持久化成功！')
            else:
                print('持久化失败！')
