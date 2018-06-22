# coding = utf-8
from ia.src.classes.com.Controller import requirement, Resources, controller
from ia.src.classes.ia_res.Ant import ant
from collections import Counter
from ia.src.classes.ia_res.TrackableTransactions import TakeTransaction, SetTransaction
from ia.src.classes.states.StateMachine import AAIState, statemachine


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
        pass

    def removeRes(self, res):
        print("Take ", res)
        pass

    def removeLastRes(self, res):
        print("Take ", res)
        self.removingItems.remove(Resources(res))
        if not self.removingItems:
            #print("End Take")
            self.add_usefull_items()

    # endregion take callbacks

    # region set callbacks

    def putResFail(self, res):
        print("Failed to set ", res)#  TODO check cette merde
        pass

    def putRes(self, res):
        #print("Set ", res) TODO check cette merde
        pass

    def putLastRes(self, res):
        #print("Set ", res)
        self.addingItems.remove(Resources(res))
        if not self.removingItems:
            #print("End Put")
            self.castIncantation()

    # endregion take callbacks

    # region incantation callbacks

    def incantationStart(self):
        pass

    def incantationEnd(self, value=None):
        if value:
            ant.lvl = value
        statemachine.closure = lambda: statemachine.pop()

    # endregion incantation callbacks

    def castIncantation(self):
        controller.incantation(self.incantationStart, self.incantationStart, self.incantationEnd, self.incantationEnd)

    def add_usefull_items(self):
        for k in self.inventory:
            if k not in self.require:
                continue
            if k in self.look and self.look[k] < self.require[k]:
                event = SetTransaction(Resources(k), self.require[k] - self.look[k], self.putLastRes, self.putRes, self.putResFail)
                self.addingItems.append(k)
                event.execute()
            elif k not in self.look:
                event = SetTransaction(Resources(k), self.require[k], self.putLastRes, self.putRes, self.putResFail)
                self.addingItems.append(k)
                event.execute()
        self.castIncantation()

    def remove_useless_items(self):
        for k, v in self.look.items():
            if k not in self.require:
                event = TakeTransaction(Resources(k), v, self.removeLastRes, self.removeRes, self.removeResFail)
                self.removingItems.append(k)
                event.execute()
            elif k in requirement and self.require[k] < v:
                event = TakeTransaction(Resources(k), v - self.require[k], self.removeLastRes, self.removeRes, self.removeResFail)
                self.removingItems.append(k)
                event.execute()
        self.add_usefull_items()

    # region precheck

    def store_inventory(self, inventory):
        ant.inventory = inventory
        self.inventory = inventory

        merge = dict(Counter(self.inventory) + Counter(self.look))
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
