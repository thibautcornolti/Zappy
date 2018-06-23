# coding = utf-8
from src.classes.states.StateMachine import AAIState


class SeekEmptyTileState(AAIState):

    def __init__(self, replacement_state):
        super().__init__("SeekEmptyTileState")

    def on_push(self, cli):
        super().on_push(cli)
