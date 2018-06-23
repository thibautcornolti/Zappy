# coding = utf-8
from src.classes.states.StateMachine import AAIState


class FollowQueenState(AAIState):

    def __init__(self):
        super().__init__("FollowQueenState")

    def on_push(self, cli):
        super().on_push(cli)


