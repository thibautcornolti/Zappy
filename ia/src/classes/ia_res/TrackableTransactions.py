# coding = utf-8

from ia.src.classes.com.Controller import controller, CmdCost
from ia.src.classes.com.Transaction import TrackableTransaction
from ia.src.classes.ia_res.Vector import Vector

class CheckTransaction(TrackableTransaction):

    def __init__(self, pos=Vector()):
        super().__init__(0, lambda: None, pos)

    def execute(self):
        pass

class TakeTransaction(TrackableTransaction):

    def __init__(self, item, nb, last, ok, ko, pos=Vector()):
        super().__init__(CmdCost.Take.value * nb, last, pos)
        self.item = item
        self.nb = nb
        self.ko = ko
        self.ok = ok

    def last_item_ko(self, value):
        self.ko(value)
        self.end(value)

    def last_item_ok(self, value):
        self.ok(value)
        self.end(value)

    def execute(self):
        print("take execute ", self.item.value, " in ", self.position)
        for j in range(self.nb):
            if j == self.nb - 1:
                controller.take(self.item, self.last_item_ok, self.last_item_ko)
            else:
                controller.take(self.item, self.ok, self.ko)

    def __repr__(self):
        return repr(
            super().__repr__() + " " + self.position.__repr__() + " -> Take " + self.item.value + " x " + str(self.nb))


class SetTransaction(TrackableTransaction):

    def __init__(self, item, nb, last, ok, ko, pos=Vector()):
        super().__init__(CmdCost.Set.value * nb, last, pos)
        self.item = item
        self.nb = nb
        self.ko = ko
        self.ok = ok

    def last_item_ko(self, value):
        self.ko(value)
        self.end(value)

    def last_item_ok(self, value):
        self.ok(value)
        self.end(value)

    def execute(self):
        print("set execute ", self.item.value, " in ", self.position)
        for j in range(self.nb):
            if j == self.nb - 1:
                controller.set(self.item, self.last_item_ok, self.last_item_ko)
            else:
                controller.set(self.item, self.ok, self.ko)

    def __repr__(self):
        return repr(self.position.__repr__() + " -> Set " + self.item.value + " x " + str(self.nb))


class LookTransaction(TrackableTransaction):

    def __init__(self, ok, pos=Vector()):
        super().__init__(CmdCost.Look.value, ok, pos)

    def execute(self):
        controller.look(self.end)

    def __repr__(self):
        return repr(self.position.__repr__() + " -> Look")


class LeftTransaction(TrackableTransaction):

    def __init__(self, ok, pos=Vector()):
        super().__init__(CmdCost.Left.value, ok, pos)

    def execute(self):
        controller.left(self.end)

    def __repr__(self):
        return repr(self.position.__repr__() + " -> Left")


class RightTransaction(TrackableTransaction):

    def __init__(self, ok, pos=Vector()):
        super().__init__(CmdCost.Right.value, ok, pos)

    def execute(self):
        controller.right(self.end)

    def __repr__(self):
        return repr(self.position.__repr__() + " -> Right")


class ForwardTransaction(TrackableTransaction):

    def __init__(self, ok, pos=Vector()):
        super().__init__(CmdCost.Forward.value, ok, pos)

    def execute(self):
        controller.forward(self.end)

    def __repr__(self):
        return repr(self.position.__repr__() + " -> Forward")


class InventoryTransaction(TrackableTransaction):

    def __init__(self, ok, pos=Vector()):
        super().__init__(CmdCost.Inventory.value, ok, pos)

    def execute(self):
        controller.inventory(self.end)

    def __repr__(self):
        return repr(self.position.__repr__() + " -> Inventory")


class IncantationTransaction(TrackableTransaction):

    def __init__(self, ok, pos=Vector()):
        super().__init__(CmdCost.Incantation.value, ok, pos)

    def execute(self):
        pass
        # controller.incantation(self.end)

    def __repr__(self):
        return repr(self.position.__repr__() + " -> Incantation")
