# coding = utf-8

import select
import ia.src.classes.com.Client as COM
from ia.src.classes.com.Controller import controller


class StateException(Exception):

    def __init__(self, value):
        self.value = value

    def __str__(self):
        return repr("StateException : " + self.value)


class AState(object):

    def __init__(self, name):
        self._name = name

    def update_in(self, cli, inputs):
        raise NotImplementedError("AState update_in : pure virtual fct called")

    def update_out(self, cli):
        raise NotImplementedError("AState update_out : pure virtual fct called")

    def __repr__(self):
        return repr(self._name)


class AAIState(object):

    def __init__(self, name):
        self._name = name

    def update_in(self, cli, inputs):
        del cli
        for elem in inputs:
            controller.applyTop(elem)

    def update_out(self, cli):
        raise NotImplementedError("AState update_out : pure virtual fct called")

    def __repr__(self):
        return repr(self._name)


class StateMachine(object):

    def __init__(self):
        self._stack = []
        self.closure = None

    def push(self, state):
        if not issubclass(type(state), AState) and not issubclass(type(state), AAIState):
            raise Exception("State is not a valid variable type")
        self._stack.insert(0, state)

    def pop(self):
        self._stack.pop(0)

    def replace(self, state):
        print(state)
        if not issubclass(type(state), AState) and not issubclass(type(state), AAIState):
            raise Exception("State is not a valid variable type")
        self.pop()
        self.push(state)

    def update(self):
        if len(self._stack) == 0:
            raise StateException("All the states ends")
        self._stack[0].update_out(COM.cli)
        value = COM.cli.poll()
        if select.POLLIN & value:
            msgs = COM.cli.consult()
            controller.checkEndGame(msgs)
            self._stack[0].update_in(COM.cli, msgs)
        if self.closure:
            self.closure()
            self.closure = None


statemachine = StateMachine()
