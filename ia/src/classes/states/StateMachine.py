# coding = utf-8

import select
import src.classes.com.Client as COM
from src.classes.com.Controller import controller, Resources
from src.classes.ia_res.Ant import ant


class StateException(Exception):

    def __init__(self, value):
        self.value = value

    def __str__(self):
        return repr("StateException : " + self.value)


class AState(object):

    def __init__(self, name):
        self._name = name

    def update(self, cli, inputs):
        pass

    def on_push(self, cli):
        pass

    def pushed_over(self):
        pass

    def popped_over(self):
        pass

    def on_pop(self, cli):
        pass

    def __repr__(self):
        return repr(self._name)


class AAIState(AState):

    def __init__(self, name):
        super().__init__(name)
        self.min_food = 10
        self.need_food = 30
        self.actions = 0
        self.actions_max = 7
        self.emergency = False

    def updateInventory(self, inventory):
        from src.classes.states.SeekItemsState import SeekItemsState
        ant.inventory = inventory
        if inventory[Resources.Food] < self.min_food and self.emergency == False:
            statemachine.closure = lambda: statemachine.push(SeekItemsState({Resources.Food: self.need_food - inventory[Resources.Food]}, True))
            self.emergency = True

    def popped_over(self):
        super().popped_over()
        if self.emergency:
            self.emergency = False

    def update(self, cli, inputs):
        del cli
        for elem in inputs:
            oui = controller.applyTop(elem)
            if oui == (True, False):
                self.actions += 1
        if self.actions > self.actions_max:
            controller.inventory(self.updateInventory)
            self.actions = 0


class StateMachine(object):

    def __init__(self):
        self._stack = []
        self.closure = None

    def push(self, state):
        if not issubclass(type(state), AState) and not issubclass(type(state), AAIState):
            raise Exception("State is not a valid variable type")
        if self._stack:
            self._stack[0].pushed_over()
        self._stack.insert(0, state)
        self._stack[0].on_push(COM.cli)

    def pop(self):
        self._stack[0].on_pop(COM.cli)
        self._stack.pop(0)
        if self._stack:
            self._stack[0].popped_over()

    def replace(self, state):
        if not issubclass(type(state), AState) and not issubclass(type(state), AAIState):
            raise Exception("State is not a valid variable type")
        self.pop()
        self.push(state)

    def update(self):
        if len(self._stack) == 0:
            raise StateException("All the states ends")
        value = COM.cli.poll()
        if select.POLLIN & value:
            msgs = COM.cli.consult()
            controller.checkEndGame(msgs)
            self._stack[0].update(COM.cli, msgs)
        if controller.hasBufferizedCmds():
            controller.flushCmds()
        if self.closure:
            self.closure()
            self.closure = None


statemachine = StateMachine()
