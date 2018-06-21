# coding = utf-8
from src.classes.com.Controller import requirement, Resources, controller
from src.classes.ia_res.Ant import ant
from collections import Counter
from src.classes.ia_res.PathEvents import TakeEvent, SetEvent
from src.classes.states.StateMachine import AAIState, statemachine


# look + inventory chained => take set combo
# on failure => ant doesn't lvl up

class IncantationState(AAIState):

    def __init__(self):
        super().__init__("Incantation -> " + str(ant.lvl + 1))
        self.require = requirement[ant.lvl + 1][1]
        self.look = dict()
        self.inventory = dict()
        self.removingItems = list()
        self.addingItems = list()

    # region take callbacks

    def removeResFail(self, res):
        print("Failed to take ", res)

    def removeRes(self, res):
        print("Take ", res)

    def removeLastRes(self, res):
        print("Take ", res)
        self.removingItems.remove(Resources(res))
        if not self.removingItems:
            print("End Take")
            self.add_usefull_items()

    # endregion take callbacks

    # region set callbacks

    def putResFail(self, res):
        print("Failed to set ", res)

    def putRes(self, res):
        print("Set ", res)

    def putLastRes(self, res):
        print("Set ", res)
        self.addingItems.remove(Resources(res))
        if not self.removingItems:
            print("End Put")
            self.castIncantation()

    # endregion take callbacks

    # region incantation callbacks

    def incantationStartFail(self):
        print("Failed to start Incantation")  # TODO ???

    def incantationStartOK(self):
        print("Incantation start successfully")  # TODO ???

    def incantationEndFail(self):
        print("Failed to end Incantation")  # TODO ???
        statemachine.closure = lambda: statemachine.pop()

    def incantationEndOK(self, value):
        print("Incantation end successfully")  # TODO ???
        print(value)
        statemachine.closure = lambda: statemachine.pop()

    # endregion incantation callbacks

    def castIncantation(self):
        controller.incantation(self.incantationStartOK, self.incantationStartFail, self.incantationEndOK,
                               self.incantationEndFail)

    def add_usefull_items(self):
        add_something = False
        for k in self.inventory:
            if k not in self.require:
                continue
            if k in self.look and self.look[k] < self.require[k]:
                event = SetEvent(Resources(k), self.require[k] - self.look[k], self.putLastRes, self.putRes,
                                 self.putResFail)
                self.addingItems.append(k)
                add_something = True
                event.execute()
            elif k not in self.look:
                event = SetEvent(Resources(k), self.require[k], self.putLastRes, self.putRes, self.putResFail)
                self.addingItems.append(k)
                add_something = True
                event.execute()
        if not add_something:
            self.castIncantation()

    def remove_useless_items(self):
        remove_something = False
        for k, v in self.look.items():
            if k not in self.require:
                event = TakeEvent(Resources(k), v, self.removeLastRes, self.removeRes, self.removeResFail)
                self.removingItems.append(k)
                remove_something = True
                event.execute()
            elif k in requirement and self.require[k] < v:
                event = TakeEvent(Resources(k), v - self.require[k], self.removeLastRes, self.removeRes,
                                  self.removeResFail)
                self.removingItems.append(k)
                remove_something = True
                event.execute()
        if not remove_something:
            self.add_usefull_items()

    # region precheck

    def store_inventory(self, inventory):
        ant.inventory = inventory
        self.inventory = inventory

        merge = dict(Counter(self.inventory) + Counter(self.look))
        print(merge)
        print(self.require)
        print(self.look)
        print(self.inventory)
        for k, v in self.require.items():
            if k not in merge or merge[k] < v:
                statemachine.closure = lambda: statemachine.pop()
                return
        self.remove_useless_items()

    def store_look(self, look):
        enum_values = [res.value for res_name, res in Resources.__members__.items()]
        look_converted = dict()
        for i in look[0]:
            look_converted[i] = look_converted.get(i, 0) + 1
        look_converted = {Resources(k): v for k, v in look_converted.items() if k in enum_values}
        self.look = look_converted
        ant.look = look
        controller.inventory(self.store_inventory)

    # endregion precheck

    # region inherit

    def on_push(self, cli):
        super().on_push(cli)
        controller.look(self.store_look)

    # endregion inherit
