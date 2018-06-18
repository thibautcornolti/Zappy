# coding = utf-8

from ia.src.classes.com.HashManager import HashManager

print("ANT")

class Vector(object):

    def __init__(self, x=0, y=None):
        if y is None:
            y = x
        self.x = x
        self.y = y


class Ant(object):

    def __init__(self, team):
        self.team = team
        self.map_size = Vector()
        self.current_nbr = 0
        self.lvl = 0
        self.look = []
        self.inventory = {}
        self._uuid = HashManager.generate()

    @property
    def uuid(self):
        save = self.uuid
        self._uuid = HashManager.hash(self._uuid)
        return save

    @uuid.setter
    def uuid(self, value):
        raise Exception("Can't set uuid directly")


ant = Ant(None)


def init_ant(team):
    ant.team = team
    return ant
