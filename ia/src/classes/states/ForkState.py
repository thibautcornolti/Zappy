# coding = utf-8
from src.classes.states.StateMachine import AState


class ForkState(AState):

    def __init__(self):
        super().__init__("Fork")
