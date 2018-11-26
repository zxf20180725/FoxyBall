class Protocol:
    """
    规定：
        数据包头部占4字节

        整型占4字节

        字符串长度位占4字节

        字符串不定长

    """

    def __init__(self, bs=None):
        """
        如果bs为None则代表需要创建一个数据包
        否则代表需要解析一个数据包
        """
        if bs:
            self.bs = bytearray(bs)
        else:
            self.bs = bytearray(0)

    def get_int32(self):
        try:
            ret = self.bs[:4]
            self.bs = self.bs[4:]
            return int.from_bytes(ret, byteorder='little')
        except:
            raise Exception("数据异常！")

    def get_str(self):
        try:
            # 拿到字符串字节长度(字符串长度位4字节)
            length = int.from_bytes(self.bs[:4], byteorder='little')
            # 再拿字符串
            ret = self.bs[4:length + 4]
            # 删掉取出来的部分
            self.bs = self.bs[4 + length:]
            return ret.decode(encoding='utf8')
        except:
            raise Exception("数据异常！")

    def add_int32(self, val):
        bytes_val = bytearray(val.to_bytes(4, byteorder='little'))
        self.bs += bytes_val

    def add_str(self, val):
        bytes_val = bytearray(val.encode(encoding='utf8'))
        end_flag = bytearray(1)
        end_flag[0] = 0x00
        bytes_val += end_flag

        bytes_length = bytearray(len(bytes_val).to_bytes(4, byteorder='little'))
        self.bs += (bytes_length + bytes_val)

    def get_pck_not_head(self):
        return self.bs

    def get_pck_has_head(self):
        data_start = bytearray(4)
        data_start[0] = 0xAB
        data_start[1] = 0xCD
        data_start[2] = 0xEF
        data_start[3] = 0xAB

        bytes_pck_length = bytearray(len(self.bs).to_bytes(4, byteorder='little'))
        return data_start + bytes_pck_length + self.bs


def print_hex(bytes):
    l = [hex(int(i)) for i in bytes]
    print(" ".join(l))


p = Protocol()
p.add_str("赵雄飞")

print_hex(p.get_pck_not_head())
