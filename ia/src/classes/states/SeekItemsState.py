from ia.src.classes.ia_res.Path import Path, PathManipulator, PositionTracker
from ia.src.classes.ia_res.PathEvents import TakeEvent, LookEvent, PointEvent
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
                    event = TakeEvent(item, nb, self.last_take_ok, self.take_ok, self.take_ko)
                    path.addConePoint(i, event)
        return found, path

    def goNextPlace(self):
        path = Path()
        if self.progress == self.surface:
            left_dist = ant.lvl // 2
            path.addPoint(Vector(-left_dist, 0), PointEvent(0, lambda: self.pathHandler.stepNextPoint()))
            path.addPoint(Vector(-left_dist, -1), LookEvent(self.updateAntLook))
            move = Vector(-left_dist, -1)
            self.progress = 0
        else:
            path.addPoint(Vector(0, 1), LookEvent(self.updateAntLook))
            move = Vector(0, 1)
            self.progress += 1
        path, look = path.generateOrder(False)
        self.tracker.addMove(move, look)
        self.pathHandler = PathManipulator(path, self.updateAntLook)  # TODO estimate ?

    def updateAntLook(self, look):
        ant.look = look
        found, path = self.findLooksItems(look)
        if found:
            self.pathHandler = PathManipulator(path.generateOpti(True)[0], self.checkEnd) # TODO estimate ?
        else:
            self.goNextPlace()
        self.pathHandler.stepNextPoint()

    def take_ko(self, value):
        del value
        self.pathHandler.stepNextPoint()

    def last_take_ok(self, value):
        self.items_dict[Resources(value)] -= 1
        controller.inventory(self.updateAntInventory)
        self.pathHandler.stepNextPoint()

    def take_ok(self, value):
        self.items_dict[Resources(value)] -= 1

    def on_push(self, cli):
        super().on_push(cli)
        controller.look(self.updateAntLook)

    def update(self, cli, inputs):
        super().update(cli, inputs)

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
        if check and not self.rollback:
            statemachine.closure = closurePop
        elif check and self.rollback:
            self.rollback = False
            look, path = self.tracker.returnHome()
            self.pathHandler = PathManipulator(path, self.checkEnd) # TODO estimate ?
            self.pathHandler.stepNextPoint()
        else:
            controller.look(self.updateAntLook)

    def __init__(self, items_dict, rollback=False):
        super().__init__("SeekItems")
        self.progress = 0
        self.tracker = PositionTracker()
        self.pathHandler = None
        self.looked = False
        self.surface = max(ant.map_size.x, ant.map_size.y)
        self.items_dict = items_dict
        self.rollback = rollback
