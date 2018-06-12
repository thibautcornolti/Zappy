import socket
import hashlib


class AIController:
    def __init__(self, client, team):
        self.client = client
        self.team = team
        init_hash = b''
        self.uuid = hashlib.sha1(init_hash).hexdigest()[:12]
        self.ulapid = self.uuid
        if self.client.read_line() != 'WELCOME':
            raise Exception(
                "Zappy client have to be connected to a Zappy server!")
        self.client.write(team)
        r = self.client.read_line()
        if r == 'ko':
            raise Exception("Bad team name")

    def next_lap(self):
        self.ulapid = hashlib.sha256(str.encode(self.ulapid)).hexdigest()[:12]

    def broadcast(self, msg):
        self.client.write("Broadcast %s %s" % (self.ulapid, msg))
        return True if self.client.read_line() == 'ok' else False


class Client:
    def __init__(self, ip, port):
        self.ip = ip
        self.port = port

    def __enter__(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            self.sock.connect((self.ip, self.port))
            return self, None
        except ConnectionRefusedError as e:
            return self, e

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.sock.close()

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
