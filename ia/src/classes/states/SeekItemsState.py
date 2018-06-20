from ia.src.classes.ia_res.Path import Path, PathManipulator
from ia.src.classes.ia_res.PathEvents import TakeEvent, LookEvent
from ia.src.classes.states.StateMachine import AAIState, statemachine
from ia.src.classes.com.Controller import controller, Resources
from ia.src.classes.ia_res.Ant import ant, Vector


class SeekItemsState(AAIState):

    def findLooksItems(self, look):
        dup_items = dict(self.items_dict)
        path = Path()
        found = False
        for item in dup_items:
            for i in range(len(look)):
                if dup_items[item] != 0 and item.value in look[i]:
                    found = True
                    nb = min(look[i].count(item.value), dup_items[item])
                    if nb < 0:
                        nb = look[i].count(item.value)
                    dup_items[item] -= nb
                    event = TakeEvent(item, nb, nb * 7, self.last_take_ok, self.take_ok, self.take_ko)
                    path.addConePoint(i, event)
        return found, path

    def updateAntLook(self, look):
        import json
        print(json.dumps(look, indent=4))
        found, path = self.findLooksItems(look)
        if found:
            self.pathHandler = PathManipulator(path.generateOpti(True)[0])
        else:
            p = Path()
            p.addPoint(Vector(0, 1), LookEvent(self.updateAntLook))
            self.pathHandler = PathManipulator(path.generateOrder(True)[0])
        self.pathHandler.stepNextPoint(lambda: print(self.pathHandler.isEnded()))

    def take_ko(self, value):
        print("Failed to take ", value)
        self.pathHandler.stepNextPoint(lambda: print(self.pathHandler.isEnded()))

    def last_take_ok(self, value):
        self.items_dict[Resources(value)] -= 1
        controller.inventory(self.updateAntInventory)
        self.pathHandler.stepNextPoint(lambda: print(self.pathHandler.isEnded()))

    def take_ok(self, value):
        self.items_dict[Resources(value)] -= 1

    def on_push(self, cli):
        super().on_push(cli)
        controller.look(self.updateAntLook)

    def update(self, cli, inputs):
        super().update(cli, inputs)
        self.checkEnd()

    def updateAntInventory(self, inventory):
        ant.inventory = inventory

    def checkEnd(self):
        check = True
        if self.pathHandler and not self.pathHandler.isEnded():
            return False

        def closurePop():
            statemachine.pop()

        for k, v in self.items_dict.items():
            if v > 0:
                check = False
        if check:
            statemachine.closure = closurePop
        return check

    def __init__(self, items_dict):
        super().__init__("SeekItems")
        self.pathHandler = None
        self.looked = False
        self.surface = max(ant.map_size.x, ant.map_size.y)
        self.items_dict = items_dict
