# coding = utf-8


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
