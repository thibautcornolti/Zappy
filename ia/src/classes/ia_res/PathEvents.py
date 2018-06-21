# coding = utf-8

from src.classes.com.Controller import controller, CmdCost
from src.classes.ia_res.Vector import Vector


class PointEvent(object):

    def __init__(self, time_estimation, callback=None):
        self._time_estimation = time_estimation
        self.position = Vector()
        self.callback = callback

    @property
    def time_estimation(self):
        return self._time_estimation

    def execute(self):
        if self.callback:
            self.callback()

    def __repr__(self):
        return repr(self.position.__repr__())

class TakeEvent(PointEvent):

    def __init__(self, item, nb, last, ok, ko):
        super().__init__(CmdCost.Take.value * nb)
        self.item = item
        self.nb = nb
        self.last = last
        self.ko = ko
        self.ok = ok

    def execute(self):
        print("take execute ", self.item.value, " in ", self.position)
        for j in range(self.nb):
            if j == self.nb - 1:
                controller.take(self.item, self.last, self.ko)
            else:
                controller.take(self.item, self.ok, self.ko)

    def __repr__(self):
        return repr(self.position.__repr__() + " -> Take " + self.item.value + " x " + str(self.nb))

class SetEvent(PointEvent):

    def __init__(self, item, nb, last, ok, ko):
        super().__init__(CmdCost.Set.value * nb)
        self.item = item
        self.nb = nb
        self.last = last
        self.ko = ko
        self.ok = ok

    def execute(self):
        print("set execute ", self.item.value, " in ", self.position)
        for j in range(self.nb):
            if j == self.nb - 1:
                controller.set(self.item, self.last, self.ko)
            else:
                controller.set(self.item, self.ok, self.ko)

    def __repr__(self):
        return repr(self.position.__repr__() + " -> Set " + self.item.value + " x " + str(self.nb))

class LookEvent(PointEvent):

    def __init__(self, ok):
        super().__init__(CmdCost.Look.value)
        self.ok = ok

    def execute(self):
        controller.look(self.ok)

    def __repr__(self):
        return repr(self.position.__repr__() + " -> Look")
