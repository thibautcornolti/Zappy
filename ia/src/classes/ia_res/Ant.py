# coding = utf-8

from src.classes.com.HashManager import HashManager
from src.classes.ia_res.Vector import Vector


class Ant(object):

    def __init__(self, team):
        self.team = team
        self.map_size = Vector()
        self.current_nbr = 0
        self.lvl = 1
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
