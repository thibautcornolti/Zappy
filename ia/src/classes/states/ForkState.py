# coding = utf-8
from ia.src.classes.states.StateMachine import AAIState


class ForkState(AAIState):

    def __init__(self):
        super().__init__("Fork")
