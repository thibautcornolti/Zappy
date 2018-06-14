# coding = utf-8


class AState(object):
    pass


class StateMachine(object):

    def push(self, state):
        if type(state) != AState:
            raise Exception("State is not a valid variable type")
        raise NotImplementedError("StateMachine update")

    def pop(self):
        raise NotImplementedError("StateMachine pop")

    def replace(self, state):
        if type(state) != AState:
            raise Exception("State is not a valid variable type")
        self.pop()
        self.push(state)

    def update(self):
        raise NotImplementedError("StateMachine update")
