from .HashManager import HashManager


class AIController:
    def __init__(self, client, team):
        self.client = client
        self.team = team
        self.uuid = HashManager.generate()
        self.ulapid = self.uuid
        if self.client.read_line() != 'WELCOME':
            raise Exception(
                "Zappy client have to be connected to a Zappy server!")
        self.client.write(team)
        r = self.client.read_line()
        if r == 'ko':
            raise Exception("Bad team name")

    def next_lap(self):
        self.ulapid = HashManager.hash(self.ulapid)

    def broadcast(self, msg):
        self.client.write("Broadcast %s %s" % (self.ulapid, msg))
        return True if self.client.read_line() == 'ok' else False
