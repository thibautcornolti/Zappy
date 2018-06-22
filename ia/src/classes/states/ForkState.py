# coding = utf-8
from ia.src.classes.com.Controller import controller
from ia.src.classes.states.StateMachine import AAIState, statemachine


class ForkState(AAIState):

    def __init__(self):
        super().__init__("Fork")

    def fork_end(self):
        statemachine.pop()

    def on_push(self, cli):
        super().on_push(cli)
        controller.fork(self.fork_end)
