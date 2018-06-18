# coding = utf-8

import enum
from ia.src.classes.com.HashManager import HashManager


class Ressources(enum.Enum):
    """
    To implement fully
    """
    Food = 0
    minerai_a = 0
    minerai_b = 0
    minerai_c = 0

class GameException(Exception):

    def __init__(self, value):
        self.value = value

    def __str__(self):
        return repr("GameException : " + self.value)


class Controller(object):

    def __init__(self):
        self._uuid = HashManager.generate()

    @property
    def uuid(self):
        save = self.uuid
        self._uuid = HashManager.hash(self._uuid)
        return save

    @uuid.setter
    def uuid(self, value):
        raise Exception("Can't set uuid directly")

    def broadcast(self, msg):
        raise NotImplementedError("Broadcast")

    def look(self):
        raise NotImplementedError("Look")

    def left(self):
        raise NotImplementedError("Left")

    def right(self):
        raise NotImplementedError("Right")

    def move(self):
        raise NotImplementedError("Move")

    def take(self, type):
        if type(type) != Ressources:
            raise Exception("Invalid type")
        raise NotImplementedError("Take")

    def let(self, type):
        if type(type) != Ressources:
            raise Exception("Invalid type")
        raise NotImplementedError("Let")

    def inventory(self):
        raise NotImplementedError("Inventory")

    def fork(self):
        raise NotImplementedError("Fork")

    def incantation(self):
        raise NotImplementedError("Incantation")

    def checkEndGame(self, args):
        for elem in args:
            if elem == "dead":
                raise GameException("You died")


controller = Controller()
