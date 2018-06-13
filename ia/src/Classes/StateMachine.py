class StateMachine:
    def __init__(self):
        self.stack = []

    def push(self, state):
        self.stack.append(state)

    def pop(self):
        self.stack.pop()
