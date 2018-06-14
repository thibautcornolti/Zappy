import socket
import select


class Client:

    def __init__(self, ip, port):
        self.port = port
        self.ip = ip

    def __enter__(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            self.sock.connect((self.ip, self.port))
            return self, None
        except ConnectionRefusedError as e:
            return self, e

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.sock.close()

    def poll(self, timeout):
        raise NotImplementedError("Poll")

    def write(self, msg):
        self.sock.sendall(str.encode(msg + "\n"))

    def read_line(self):
        res = b''
        c = b''
        while c != b'\n':
            res += c
            c = self.sock.recv(1)
        print("Receive: %s" % bytes.decode(res))
        return bytes.decode(res)
