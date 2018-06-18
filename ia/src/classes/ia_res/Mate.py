# coding = utf-8

import enum
from ia.src.classes.com.HashManager import HashManager


class Roles(enum.Enum):
    QUEEN = 0
    FOOD_SEEKER = 1
    STONE_SEEKER = 2


class Mate(object):

    def __init__(self, uuid):
        self._uuid = uuid
        self.roles = list()

    @property
    def uuid(self):
        self._uuid = HashManager.hash(self._uuid)
        return self._uuid

    @uuid.setter
    def uuid(self, value):
        raise Exception("Can't set uuid directly")

    @property
    def roles(self):
        return self.roles

    @roles.setter
    def roles(self, roles):
        if type(roles) != list():
            raise Exception("Invalid role type")
        self.roles = roles
